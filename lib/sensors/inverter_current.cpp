#include "adc_mux.hpp"

#include <inverter_current.hpp>

namespace hyped::sensors {

InverterCurrent::InverterCurrent(core::ILogger &logger,
                                 std::shared_ptr<io::II2c> i2c,
                                 const AdcMuxChannel adc_mux_channel)
    : logger_(logger),
      i2c_(i2c),
      adc_mux_channel_(adc_mux_channel)
{
}

InverterCurrent::~InverterCurrent()
{
}

std::optional<core::Float> InverterCurrent::readCurrent()
{
  const auto inverter_current = i2c_->readByte(kAdcMuxAddress, adc_mux_channel_);
  if (!inverter_current) {
    logger_.log(core::LogLevel::kFatal, "Failed to read inverter current");
    return std::nullopt;
  }
  // Output is 0-5V, which corresponds to 50-130A
  return static_cast<core::Float>(*inverter_current) * 16 + 50;
}

}  // namespace hyped::sensors