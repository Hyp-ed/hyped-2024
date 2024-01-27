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
  // include "magic sauce" optimisation?
 public:
  static std::optional<OpticalFlow> create(core::ILogger &logger,
                                           std::shared_ptr<io::ISpi> spi,
                                           const std::uint8_t channel);
  ~OpticalFlow();

 private:
  OpticalFlow(core::ILogger &logger, std::shared_ptr<io::ISpi> spi, const std::uint8_t channel);
  std::uint8_t getDeltaX(std::shared_ptr<io::ISpi> spi);
  std::uint8_t getDeltaY(std::shared_ptr<io::ISpi> spi);

 private:
  core::ILogger &logger_;
  std::shared_ptr<io::ISpi> spi_;
  const std::uint8_t channel_;

 private:
  // Register addresses for x and y
  static constexpr std::uint8_t kXLowAddress  = 0x03;
  static constexpr std::uint8_t kXHighAddress = 0x04;
  static constexpr std::uint8_t kYLowAddress  = 0x05;
  static constexpr std::uint8_t kYHighAddress = 0x06;
  // static constexpr std::uint16_t kDeltaX = 0;
  // static constexpr std::uint16_t kDeltaY = 0;

  static constexpr std::uint8_t kDeviceIdAddress       = 0x00;
  static constexpr std::uint8_t kExpectedDeviceIdValue = 0x49;  // unsure that's the value
};

}  // namespace hyped::sensors