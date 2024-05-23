#pragma once

#include <cstdint>
#include <cstring>
#include <memory>
#include <optional>

#include <core/logger.hpp>
#include <core/types.hpp>
#include <io/spi.hpp>

namespace hyped::sensors {

enum class Rotation { kNone, kClockwise90, kClockwise180, kClockwise270 };

class OpticalFlow {
  // This is all derived from https://github.com/pimoroni/pmw3901-python/ as the datasheet is no use
 public:
  static std::optional<std::shared_ptr<OpticalFlow>> create(core::ILogger &logger,
                                                            const std::shared_ptr<io::ISpi> &spi,
                                                            Rotation rotation);

  OpticalFlow(core::ILogger &logger, std::shared_ptr<io::ISpi> spi);
  std::optional<std::uint16_t> read();
  std::optional<std::uint16_t> getDeltaX() const;
  std::optional<std::uint16_t> getDeltaY() const;

 private:
  /**
   * @brief This is proprietary magic numbers that must be sent to the sensor.
   */
  static core::Result doMagic(core::ILogger &logger, const std::shared_ptr<io::ISpi> &spi);
  /**
   * @brief Set the rotation of the sensor.
   */
  static core::Result setRotation(core::ILogger &logger,
                                  const std::shared_ptr<io::ISpi> &spi,
                                  Rotation rotation);
  static std::uint8_t getOrientation(Rotation rotation);

  core::ILogger &logger_;
  std::shared_ptr<io::ISpi> spi_;

  // Register addresses for x and y
  static constexpr std::uint8_t kXLowAddress  = 0x03;
  static constexpr std::uint8_t kXHighAddress = 0x04;
  static constexpr std::uint8_t kYLowAddress  = 0x05;
  static constexpr std::uint8_t kYHighAddress = 0x06;

  static constexpr std::uint8_t kDeviceIdAddress       = 0x00;
  static constexpr std::uint8_t kExpectedDeviceIdValue = 0x49;
  static constexpr std::uint8_t kPowerUpResetAddress   = 0x3A;
  static constexpr std::uint8_t kPowerUpResetValue     = 0x5A;
  static constexpr std::uint8_t kOrientationAddress    = 0x5B;
  static constexpr std::uint8_t kMotionBurst           = 0x02;

  static constexpr std::uint8_t kInvertX = 0b00100000;
  static constexpr std::uint8_t kInvertY = 0b01000000;
  static constexpr std::uint8_t kSwapXY  = 0b10000000;

  // Magic numbers for "performance optimisation"
  static constexpr std::array<std::pair<std::uint8_t, std::uint8_t>, 15> kMagicNumbers = {{
    {0x7f, 0x00},
    {0x61, 0xAD},
    {0x7F, 0x03},
    {0x40, 0x00},
    {0x7F, 0x05},
    {0x41, 0xB3},
    {0x43, 0xF1},
    {0x45, 0x14},
    {0x5B, 0x32},
    {0x5F, 0x34},
    {0x7B, 0x08},
    {0x7F, 0x06},
    {0x44, 0x1B},
    {0x40, 0xBF},
    {0x4E, 0x3F},
  }};
};

}  // namespace hyped::sensors
