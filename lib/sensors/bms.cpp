#include "bms.hpp"

namespace hyped::sensors {
std::shared_ptr<Bms> Bms::create(core::ILogger &logger, std::shared_ptr<io::ICan> can)
{
  std::shared_ptr<Bms> bms = std::make_shared<Bms>(logger, can);
  can->addProcessor(kBatteryDataId, bms);
  can->addProcessor(kCellDataId, bms);
  can->addProcessor(kTemperatureDataId, bms);
  return bms;
}

Bms::Bms(core::ILogger &logger, std::shared_ptr<io::ICan> can)
    : logger_(logger),
      can_(can),
      battery_data_(),
      cell_data_(),
      temperature_data_()
{
}

core::Result Bms::processMessage(const io::CanFrame &message)
{
  const std::uint32_t id = message.can_id;
  switch (id) {
    case kBatteryDataId:
      updateBatteryData(message);
      break;
    case kCellDataId:
      updateCellData(message);
      break;
    case kTemperatureDataId:
      updateTemperatureData(message);
      break;
    default:
      logger_.log(core::LogLevel::kFatal, "Unknown message ID: %d", id);
      return core::Result::kFailure;
  }
  return core::Result::kSuccess;
}

void Bms::updateBatteryData(const io::CanFrame &message)
{
  battery_data_.voltage         = message.data[0] << 8 | message.data[1];
  battery_data_.current         = (message.data[2] << 8 | message.data[3]) - 1000;
  battery_data_.state_of_charge = message.data[4];
  battery_data_.state_of_health = message.data[5];
  battery_data_.state           = static_cast<BatteryState>((message.data[6] & 0xF0) >> 4);
  battery_data_.alarm_level     = message.data[6] & 0x0F;
  battery_data_.battery_life    = message.data[7];
  logger_.log(core::LogLevel::kInfo,
              "Voltage: %ddV, current: %ddV, state of charge: %d%, "
              "state of health: %d%, state: %d, alarm level: %d, "
              "battery life: %d",
              battery_data_.voltage,
              battery_data_.current,
              battery_data_.state_of_charge,
              battery_data_.state_of_health,
              battery_data_.state,
              battery_data_.alarm_level,
              battery_data_.battery_life);
}

void Bms::updateCellData(const io::CanFrame &message)
{
  cell_data_.maximum_cell_voltage        = message.data[0] << 8 | message.data[1];
  cell_data_.minimum_cell_voltage        = message.data[2] << 8 | message.data[3];
  cell_data_.maximum_cell_voltage_number = message.data[4];
  cell_data_.minimum_cell_voltage_number = message.data[5];
  logger_.log(core::LogLevel::kInfo,
              "Maximum cell voltage: %dmV, minimum cell voltage: %dmV, "
              "maximum cell voltage number: %d, minimum cell voltage number: %d",
              cell_data_.maximum_cell_voltage,
              cell_data_.minimum_cell_voltage,
              cell_data_.maximum_cell_voltage_number,
              cell_data_.minimum_cell_voltage_number);
}

void Bms::updateTemperatureData(const io::CanFrame &message)
{
  temperature_data_.maximum_cell_temperature        = message.data[0];
  temperature_data_.minimum_cell_temperature        = message.data[1];
  temperature_data_.maximum_cell_temperature_number = message.data[2];
  temperature_data_.minimum_cell_temperature_number = message.data[3];
  temperature_data_.cooling_                        = message.data[4];
  temperature_data_.heating_                        = message.data[5];
  logger_.log(core::LogLevel::kInfo,
              "Maximum cell temperature: %dC, minimum cell temperature: %dC, "
              "maximum cell temperature number: %d, minimum cell temperature number: %d, "
              "cooling: %d, heating: %d",
              temperature_data_.maximum_cell_temperature,
              temperature_data_.minimum_cell_temperature,
              temperature_data_.maximum_cell_temperature_number,
              temperature_data_.minimum_cell_temperature_number,
              temperature_data_.cooling_,
              temperature_data_.heating_);
}

BatteryData Bms::getBatteryData()
{
  return battery_data_;
}

CellData Bms::getCellData()
{
  return cell_data_;
}

TemperatureData Bms::getTemperatureData()
{
  return temperature_data_;
}

}  // namespace hyped::sensors