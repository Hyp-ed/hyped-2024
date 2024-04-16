#include "adc_mux.hpp"

#include <inverter_temperature.hpp>

namespace hyped::sensors {

InverterTemperature::InverterTemperature(core::ILogger &logger,
                                         std::shared_ptr<io::II2c> i2c,
                                         const AdcMuxChannel adc_mux_channel)
    : logger_(logger),
      i2c_(i2c),
      adc_mux_channel_(adc_mux_channel)
{
}

InverterTemperature::~InverterTemperature()
{
}

std::optional<core::Float> InverterTemperature::readTemperature()
{
  const auto inverter_temperature
    = i2c_->readByte(kAdcMuxAddress, static_cast<std::uint8_t>(adc_mux_channel_));
  if (!inverter_temperature) {
    logger_.log(core::LogLevel::kFatal, "Failed to read inverter temperature");
    return std::nullopt;
  }
  return 0;  // something
}

}  // namespace hyped::sensors