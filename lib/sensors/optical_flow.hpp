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
   * @brief This is proprietary magic numbers that must be sent to the sensor. I'm sorry.
   */
  static core::Result doMagic(core::ILogger &logger, const std::shared_ptr<io::ISpi> &spi);
  /**
   * @brief Set the rotation of the sensor.
   */
  static core::Result setRotation(core::ILogger &logger,
                                  const std::shared_ptr<io::ISpi> &spi,
                                  Rotation rotation);

  /**
   * @brief Write a number of address value pairs to the sensor.
   */
  static core::Result bulkWrite(const std::shared_ptr<io::ISpi> &spi,
                                const std::vector<std::pair<std::uint8_t, std::uint8_t>> &data);

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
  static constexpr std::uint8_t kReadyAddress          = 0x02;
  static constexpr std::uint8_t kMotionBurst           = 0x16;

  static constexpr std::uint8_t kInvertX = 0b00100000;
  static constexpr std::uint8_t kInvertY = 0b01000000;
  static constexpr std::uint8_t kSwapXY  = 0b10000000;
};

}  // namespace hyped::sensors
