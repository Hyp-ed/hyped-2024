#pragma once

#include <cstdint>
#include <memory>

#include <core/logger.hpp>
#include <core/types.hpp>
#include <io/can.hpp>

namespace hyped::sensors {

enum BatteryState { kSleep = 1, kPowerUp, kStandby, kPreCharge, kRun, kShutdown, kError };
struct BatteryData {
  std::uint16_t voltage;         // in deci-volts
  std::int16_t current;          // in deci-amps
  std::uint8_t state_of_charge;  // in percent
  std::uint8_t state_of_health;  // in percent
  BatteryState state;
  // 0 = no alarm, 1 = most severe alarm, 4 = least severe alarm
  std::uint8_t alarm_level;
  // number of charge/discharge cycles
  std::uint8_t battery_life;
};

struct CellData {
  std::uint16_t maximum_cell_voltage;  // in mV
  std::uint16_t minimum_cell_voltage;  // in mV
  std::uint8_t maximum_cell_voltage_number;
  std::uint8_t minimum_cell_voltage_number;
};

struct TemperatureData {
  std::uint8_t maximum_cell_temperature;  // in degrees Celsius
  std::uint8_t minimum_cell_temperature;  // in degrees Celsius
  std::uint8_t maximum_cell_temperature_number;
  std::uint8_t minimum_cell_temperature_number;
  bool cooling_;
  bool heating_;
};

class Bms : public io::ICanProcessor {
 public:
  static std::shared_ptr<Bms> create(core::ILogger &logger, std::shared_ptr<io::ICan> can);
  Bms(core::ILogger &logger, std::shared_ptr<io::ICan> can);
  core::Result processMessage(const io::CanFrame &message);
  BatteryData getBatteryData();
  CellData getCellData();
  TemperatureData getTemperatureData();

 private:
  void updateBatteryData(const io::CanFrame &message);
  void updateCellData(const io::CanFrame &message);
  void updateTemperatureData(const io::CanFrame &message);

 private:
  static constexpr std::uint32_t kBatteryDataId     = 0x186040F3;
  static constexpr std::uint32_t kCellDataId        = 0x186140F3;
  static constexpr std::uint32_t kTemperatureDataId = 0x186240F3;
  core::ILogger &logger_;
  std::shared_ptr<io::ICan> can_;
  BatteryData battery_data_;
  CellData cell_data_;
  TemperatureData temperature_data_;
};
}  // namespace hyped::sensors