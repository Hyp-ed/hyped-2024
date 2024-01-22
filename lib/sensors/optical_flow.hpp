#pragma once

#include <cstdint>
#include <cstring>
#include <memory>
#include <optional>

#include <core/logger.hpp>
#include <core/types.hpp>
#include <io/spi.hpp>

namespace hyped::sensors {

constexpr std::uint8_t kOpticalFlowAddress = 0x00; //ToDo: determine the address of the sensor

class OpticalFlow {
// include "magic sauce" optimisation?
 public:
  static std::optional<OpticalFlow> create(core::ILogger &logger,
  										   std::shared_ptr<io::ISpi> spi,
										   const std::uint8_t channel,
										   const std::uint8_t device_address);
  ~OpticalFlow();

 private:
 OpticalFlow(core::ILogger &logger,
  			 std::shared_ptr<io::ISpi> spi,
			 const std::uint8_t channel,
			 const std::uint8_t device_address);
 
 private:
 core::ILogger &logger_;
 std::shared_ptr<io::ISpi> spi_;
 const std::uint8_t channel_;
 const std::uint8_t device_address_;

 private:
std::uint8_t getPosition();
//Register addresses for x and y
static constexpr std::uint8_t kXLowReg = 0x03;
static constexpr std::uint8_t kYLowReg = 0x05;
static constexpr std::uint8_t kXLow = 0;
static constexpr std::uint8_t kYLow = 0;

static constexpr std::uint8_t kDeviceIdAddress = 0x00;
static constexpr std::uint8_t kExpectedDeviceIdValue = 0x49;
};

} //namespace hyped::sensors