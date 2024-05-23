#pragma once

#include <cstdint>
#include <cstring>
#include <memory>
#include <optional>

#include <core/logger.hpp>
#include <core/types.hpp>
#include <io/spi.hpp>

namespace hyped::sensors {

class OpticalFlow {
 public:
  static std::optional<std::shared_ptr<OpticalFlow>> create(core::ILogger &logger,
                                                            const std::shared_ptr<io::ISpi> &spi);

  OpticalFlow(core::ILogger &logger, std::shared_ptr<io::ISpi> spi);
  std::optional<std::uint16_t> getDeltaX() const;
  std::optional<std::uint16_t> getDeltaY() const;

 private:
  /**
   * @brief This is proprietary magic numbers that must be sent to the sensor.
   */
  static core::Result doMagic(core::ILogger &logger, const std::shared_ptr<io::ISpi> &spi);
  core::ILogger &logger_;
  std::shared_ptr<io::ISpi> spi_;

  // Register addresses for x and y
  static constexpr std::uint8_t kXLowAddress  = 0x03;
  static constexpr std::uint8_t kXHighAddress = 0x04;
  static constexpr std::uint8_t kYLowAddress  = 0x05;
  static constexpr std::uint8_t kYHighAddress = 0x06;

  static constexpr std::uint8_t kDeviceIdAddress       = 0x00;
  static constexpr std::uint8_t kExpectedDeviceIdValue = 0x49;

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
