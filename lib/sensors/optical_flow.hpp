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
  // TODOLater: include "magic sauce" optimisation?
 public:
  static std::optional<std::shared_ptr<OpticalFlow>> create(core::ILogger &logger,
                                                            const std::shared_ptr<io::ISpi> &spi);

  OpticalFlow(core::ILogger &logger, std::shared_ptr<io::ISpi> spi);
  std::optional<std::uint8_t> getDeltaX() const;
  std::optional<std::uint8_t> getDeltaY() const;

 private:
  core::ILogger &logger_;
  std::shared_ptr<io::ISpi> spi_;

  // Register addresses for x and y
  static constexpr std::uint8_t kXLowAddress  = 0x03;
  static constexpr std::uint8_t kXHighAddress = 0x04;
  static constexpr std::uint8_t kYLowAddress  = 0x05;
  static constexpr std::uint8_t kYHighAddress = 0x06;

  static constexpr std::uint8_t kDeviceIdAddress       = 0x00;
  static constexpr std::uint8_t kExpectedDeviceIdValue = 0x49;
};

}  // namespace hyped::sensors
