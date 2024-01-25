#include "optical_flow.hpp"

namespace hyped::sensors {
	
std::optional<OpticalFlow> OpticalFlow::create(core::ILogger &logger,
  										   	   std::shared_ptr<io::ISpi> spi,
										   	   const std::uint8_t channel,
										   	   const std::uint8_t device_address)
{
  if (device_address != kOpticalFlowAddress) {
    logger.log(core::LogLevel::kFatal, "Invalid device address for optical flow sensor");
	return std::nullopt;
  }
  // check we are communicating with the correct sensor
  const auto device_id = spi->read(device_address, kDeviceIdAddress, 8);
  if (!device_id) {
	logger.log(core::LogLevel::kFatal, "Failed to read the optical flow device");
	return std::nullopt;
  }
  if (*device_id != kExpectedDeviceIdValue) {
	logger.log(core::LogLevel::kFatal, "Failure, mismatched device ID for optical flow sensor");
	return std::nullopt;
  }
}

OpticalFlow::OpticalFlow(core::ILogger &logger,
  						 std::shared_ptr<io::ISpi> spi,
						 const std::uint8_t channel,
						 const std::uint8_t device_address);
	: logger_(logger),
	  i2c_(i2c),
	  channel_(channel),
	  device_address_(device_address)
{
}

OpticalFlow::~OpticalFlow()
{
}

std::uint8_t OpticalFlow::getPosition()
{
  	//io::ISpi read(kXLowReg, *kXLow, 8); // abstract class
	// do same for Y
}
}