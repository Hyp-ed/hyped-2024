#include "optical_flow.hpp"

#include <cmath>

#include <algorithm>
#include <utility>

#include "core/logger.hpp"

namespace hyped::sensors {

std::optional<std::shared_ptr<OpticalFlow>> OpticalFlow::create(
  core::ILogger &logger, const std::shared_ptr<io::ISpi> &spi, Rotation rotation)
{
  // check we are communicating with the correct sensor
  auto optional_device_id = spi->read(kDeviceIdAddress, 1);
  if (!optional_device_id) {
    logger.log(core::LogLevel::kFatal, "Failed to read the optical flow device ID");
    return std::nullopt;
  }
  const auto device_id = *optional_device_id;
  if (device_id[0] != kExpectedDeviceIdValue) {
    logger.log(core::LogLevel::kFatal, "Failure, mismatched device ID for optical flow sensor");
    return std::nullopt;
  }
  const auto power_up_result = spi->write(kPowerUpResetAddress, {kPowerUpResetValue});
  if (power_up_result != core::Result::kSuccess) {
    logger.log(core::LogLevel::kFatal, "Failed to power up reset the optical flow sensor");
    return std::nullopt;
  }
  const auto rotation_result = setRotation(logger, spi, rotation);
  if (rotation_result != core::Result::kSuccess) {
    logger.log(core::LogLevel::kFatal, "Failed to set the rotation of the optical flow sensor");
    return std::nullopt;
  }
  const auto magic_result = doMagic(logger, spi);
  if (magic_result != core::Result::kSuccess) {
    logger.log(core::LogLevel::kFatal, "Failed to send magic numbers to the optical flow sensor");
    return std::nullopt;
  }
  return std::make_shared<OpticalFlow>(logger, spi);
}

OpticalFlow::OpticalFlow(core::ILogger &logger, std::shared_ptr<io::ISpi> spi)
    : logger_(logger),
      spi_(std::move(spi))
{
}

std::optional<std::uint16_t> OpticalFlow::read()
{
  auto optional_data = spi_->read(kMotionBurst, 12);
  if (!optional_data) {
    logger_.log(core::LogLevel::kFatal, "Failed to read motion burst data");
    return std::nullopt;
  }
  const auto data          = *optional_data;
  const auto ready         = data[1] & 0x80;
  const auto x             = static_cast<std::uint16_t>((data[4] << 8) | data[3]);
  const auto y             = static_cast<std::uint16_t>((data[6] << 8) | data[5]);
  const auto quality       = data[7];
  const auto shutter_upper = data[11];
  logger_.log(core::LogLevel::kDebug,
              "Read data: ready %d, x %d, y %d, quality %d, shutter upper %d",
              ready,
              x,
              y,
              quality,
              shutter_upper);
  return std::sqrt(x * x + y * y);
}

core::Result OpticalFlow::bulkWrite(const std::shared_ptr<io::ISpi> &spi,
                                    const std::vector<std::pair<std::uint8_t, std::uint8_t>> &data)
{
  for (const auto &[address, value] : data) {
    const std::vector<std::uint8_t> tx = {value};
    const auto result                  = spi->write(address, tx);
    if (result != core::Result::kSuccess) { return result; }
  }
  return core::Result::kSuccess;
}

core::Result OpticalFlow::doMagic(core::ILogger &logger, const std::shared_ptr<io::ISpi> &spi)
{
  auto result
    = bulkWrite(spi, {{{0x7f, 0x00}, {0x55, 0x01}, {0x50, 0x07}, {0x7F, 0x0E}, {0x43, 0x10}}});
  if (result != core::Result::kSuccess) { return result; }
  auto magic_read_1 = spi->read(0x67, 0x01);
  if (!magic_read_1) { return core::Result::kFailure; }
  if ((magic_read_1->at(0) & 0b10000000) != 0) {
    result = spi->write(0x48, {0x04});
    if (result != core::Result::kSuccess) { return result; }
  } else {
    result = spi->write(0x48, {0x02});
    if (result != core::Result::kSuccess) { return result; }
  }
  result = bulkWrite(spi, {{{0X7F, 0X00}, {0X51, 0X7B}, {0X50, 0X00}, {0X55, 0X00}, {0X7F, 0X03}}});
  if (result != core::Result::kSuccess) { return result; }
  auto magic_read_2 = spi->read(0x73, 0x01);
  if (!magic_read_2) { return core::Result::kFailure; }
  if (magic_read_2->at(0) == 0x00) {
    const auto optional_constant_1 = spi->read(0x70, 0x01);
    if (!optional_constant_1) { return core::Result::kFailure; }
    auto magic_constant_1          = optional_constant_1->at(0);
    const auto optional_constant_2 = spi->read(0x71, 0x01);
    if (!optional_constant_2) { return core::Result::kFailure; }
    auto magic_constant_2 = optional_constant_2->at(0);
    if (magic_constant_1 <= 28) { magic_constant_1 += 14; }
    if (magic_constant_1 > 28) { magic_constant_1 += 11; }
    magic_constant_1 = std::max(static_cast<std::uint8_t>(0),
                                std::min(static_cast<std::uint8_t>(0x3F), magic_constant_1));
    magic_constant_2 = (magic_constant_2 * 45) / 100;
    result           = bulkWrite(spi, {{{0x7F, 0x00}, {0x61, 0xAD}, {0X51, 0X70}, {0X7F, 0X0E}}});
    if (result != core::Result::kSuccess) { return result; }
    result = spi->write(0x70, {magic_constant_1});
    if (result != core::Result::kSuccess) { return result; }
    result = spi->write(0x71, {magic_constant_2});
    if (result != core::Result::kSuccess) { return result; }
  }
  result = bulkWrite(
    spi, {{{0x7F, 0x00}, {0x61, 0xAD}, {0x7F, 0x03}, {0x40, 0x00}, {0x7F, 0x05}, {0x41, 0xB3},
           {0x43, 0xF1}, {0x45, 0x14}, {0x5B, 0x32}, {0x5F, 0x34}, {0x7B, 0x08}, {0x7F, 0x06},
           {0x44, 0x1B}, {0x40, 0xBF}, {0x4E, 0x3F}, {0x7F, 0x08}, {0x65, 0x20}, {0x6A, 0x18},
           {0x7F, 0x09}, {0x4F, 0xAF}, {0x5F, 0x40}, {0x48, 0x80}, {0x49, 0x80}, {0x57, 0x77},
           {0x60, 0x78}, {0x61, 0x78}, {0x62, 0x08}, {0x63, 0x50}, {0x7F, 0x0A}, {0x45, 0x60},
           {0x7F, 0x00}, {0x4D, 0x11}, {0x55, 0x80}, {0x74, 0x21}, {0x75, 0x1F}, {0x4A, 0x78},
           {0x4B, 0x78}, {0x44, 0x08}, {0x45, 0x50}, {0x64, 0xFF}, {0x65, 0x1F}, {0x7F, 0x14},
           {0x65, 0x67}, {0x66, 0x08}, {0x63, 0x70}, {0x7F, 0x15}, {0x48, 0x48}, {0x7F, 0x07},
           {0x41, 0x0D}, {0x43, 0x14}, {0x4B, 0x0E}, {0x45, 0x0F}, {0x44, 0x42}, {0x4C, 0x80},
           {0x7F, 0x10}, {0x5B, 0x02}, {0x7F, 0x07}, {0x40, 0x41}, {0x70, 0x00}, {-1, 0x0A},
           {0x32, 0x44}, {0x7F, 0x07}, {0x40, 0x40}, {0x7F, 0x06}, {0x62, 0xF0}, {0x63, 0x00},
           {0x7F, 0x0D}, {0x48, 0xC0}, {0x6F, 0xD5}, {0x7F, 0x00}, {0x5B, 0xA0}, {0x4E, 0xA8},
           {0x5A, 0x50}, {0x40, 0x80}, {-1, 0xF0},   {0x7F, 0x14}, {0x6F, 0x1C}, {0x7F, 0x00}}});
  return result;
}

core::Result OpticalFlow::setRotation(core::ILogger &logger,
                                      const std::shared_ptr<io::ISpi> &spi,
                                      Rotation rotation)
{
  std::uint8_t orientation = 0;
  switch (rotation) {
    case Rotation::kNone:
      orientation = kInvertX | kInvertY | kSwapXY;
      break;
    case Rotation::kClockwise90:
      orientation = kInvertY;
      break;
    case Rotation::kClockwise180:
      orientation = 0;
      break;
    case Rotation::kClockwise270:
      orientation = kInvertX;
      break;
  }
  const std::vector<std::uint8_t> tx = {orientation};
  return spi->write(kOrientationAddress, tx);
}

}  // namespace hyped::sensors
