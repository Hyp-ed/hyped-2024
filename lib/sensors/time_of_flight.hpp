#pragma once

#include "i2c_sensors.hpp"

#include <cstdint>
#include <memory>
#include <optional>

#include <core/logger.hpp>
#include <io/i2c.hpp>

namespace hyped::sensors {
constexpr std::uint8_t kDefaultTimeOfFlightAddress = 0x29;

// TODOLater - Implement II2cMuxSensor
class TimeOfFlight {
 public:
  static std::optional<TimeOfFlight> create(core::ILogger &logger,
                                            std::shared_ptr<io::II2c> i2c,
                                            const std::uint8_t channel,
                                            const std::uint8_t device_address
                                            = kDefaultTimeOfFlightAddress);

  ~TimeOfFlight();

  /**
   * @brief Reads the measured range in Single-Shot mode
   * @return Range value in millimetres
   * @note Implementation based on ST Application Note AN4545
   */
  std::optional<std::uint8_t> getRangeSingleShot();

  std::optional<std::uint8_t> getRangeContinuous();

  std::optional<std::uint8_t> getStatus();

  std::uint8_t getChannel();

  // TODOLater - Implement any other functions of ToF sensor

 private:
  TimeOfFlight(core::ILogger &logger,
               std::shared_ptr<io::II2c> i2c,
               const std::uint8_t channel,
               const std::uint8_t device_address);

  /**
   * @brief Set registers in device for default use
   * @return kSuccess if successful; kFailure otherwise
   * @note See ST Application Note AN4545 Section 1.3 for details
   */
  core::Result initialise();

  /**
   * @brief Reads the measured range
   * @return Range value in millimetres
   * @note Implementation based on ST Application Note AN4545
   */
  std::optional<std::uint8_t> getRange();

  core::Result checkSensorMode(std::uint8_t mode_value);

 private:
  // TODOLater - Confirm these addresses are correct
  // Register addresses/values taken from the VL6180X datasheet
  static constexpr std::uint8_t kCtrl                 = 0x00;
  static constexpr std::uint8_t kDataHigh             = 0x00;
  static constexpr std::uint8_t kDataLow              = 0x00;
  static constexpr std::uint8_t kStatus               = 0x00;
  static constexpr std::uint8_t kBusy                 = 0x00;
  static constexpr std::uint8_t kConfigurationSetting = 0x00;

  static constexpr std::uint8_t kModeSingleShot = 0x01;
  static constexpr std::uint8_t kModeContinuous = 0x03;

  // TODOLater - std::uint8_t or std::uint16_t for all regs?
  static constexpr std::uint8_t kSystemFreshOutOfReset  = 0x016;
  static constexpr std::uint8_t kSystemModeGpioOne      = 0x011;
  static constexpr std::uint16_t kReadoutSamplingPeriod = 0x010a;
  static constexpr std::uint8_t kSysAlsAnalogueGain     = 0x03f;
  static constexpr std::uint8_t kSysRangeVhvRepeatRate  = 0x031;
  // TODOLater - Confirm SYSALS__INTEGRATION_PERIOD (application note vs datasheet)
  static constexpr std::uint8_t kSysAlsIntegrationPeriod        = 0x040;
  static constexpr std::uint8_t kSysRangeVhvRecalibrate         = 0x02e;
  static constexpr std::uint8_t kSysRangeIntermeasurementPeriod = 0x01b;
  static constexpr std::uint8_t kSysAlsIntermeasurementPeriod   = 0x03e;
  static constexpr std::uint8_t kSystemInterruptConfigGpio      = 0x014;

  static constexpr std::uint8_t kSysRangeStart             = 0x018;
  static constexpr std::uint8_t kResultInterruptStatusGpio = 0x04f;
  static constexpr std::uint8_t kResultRangeVal            = 0x062;
  static constexpr std::uint16_t kInterleavedModeEnable    = 0x2A3;

  static constexpr std::uint8_t kSystemInterruptClear = 0x015;

  core::ILogger &logger_;
  std::shared_ptr<io::II2c> i2c_;
  const std::uint8_t channel_;
  const std::uint8_t device_address_;
};

}  // namespace hyped::sensors
