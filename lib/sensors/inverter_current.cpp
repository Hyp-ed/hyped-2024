#include "adc_mux.hpp"

#include <inverter_current.hpp>
#include <cstdint>

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
  const auto inverter_current
    = i2c_->readByte(kAdcMuxAddress, static_cast<std::uint8_t>(adc_mux_channel_));
  if (!inverter_current) {
    logger_.log(core::LogLevel::kFatal, "Failed to read inverter current");
    return std::nullopt;
  }

  // The sensor maps -75A to 75A into -3V to 3V
  // Since the ADC isn't differential, we need to differentiate the reference signal and the output signal
  static const int MAX_CURRENT = 75;
  static const int MIN_CURRENT = -75;
  static const core::Float MAX_VOLTAGE = 3.3;
  static const core::Float MIN_VOLTAGE = -3.3;

  // These can be adjusted
  const int reference_voltage = 1.65;
  const core::Float virtual_ground = MAX_VOLTAGE / 2;

  // Calculate the current
  const int inverter_current_voltage = *inverter_current * (MAX_VOLTAGE - virtual_ground) + virtual_ground;
  const core::Float current = sensor_voltage * ((MAX_CURRENT - MIN_CURRENT) / (MAX_VOLTAGE - MIN_VOLTAGE)) + MIN_CURRENT;
  const core::Float reference_voltage_biased = reference_voltage + virtual_ground;
  const core::Float current_difference = current - reference_voltage_biased;
  return current_difference;
}

}  // namespace hyped::sensors