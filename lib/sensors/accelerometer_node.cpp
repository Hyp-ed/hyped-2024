#include "accelerometer.hpp"
#include "accelerometer_node.hpp"

#include <utility>

#include "core/logger.hpp"
#include "core/mqtt.hpp"
#include "core/wall_clock.hpp"
#include "io/hardware_i2c.hpp"

namespace hyped::sensors {

core::Result AccelerometerNode::startNode(toml::v3::node_view<const toml::v3::node> config,
                                          const std::string &mqtt_ip,
                                          const std::uint32_t mqtt_port)
{
  auto time          = core::WallClock();
  auto logger        = core::Logger("Accelerometer", core::LogLevel::kDebug, time);
  auto optional_mqtt = core::Mqtt::create(logger, "Accelerometer", mqtt_ip, mqtt_port);
  if (!optional_mqtt) {
    logger.log(core::LogLevel::kFatal, "Failed to create MQTT client");
    return core::Result::kFailure;
  }
  auto mqtt         = *optional_mqtt;
  auto optional_i2c = io::HardwareI2c::create(logger, 2);
  if (!optional_i2c) {
    logger.log(core::LogLevel::kFatal, "Failed to create I2C bus");
    return core::Result::kFailure;
  }
  auto i2c                    = *optional_i2c;
  auto optional_accelerometer = Accelerometer::create(logger, i2c, accelerometerAddress::k1D);
  if (!optional_accelerometer) {
    logger.log(core::LogLevel::kFatal, "Failed to create accelerometer");
    return core::Result::kFailure;
  }
  auto accelerometer = *optional_accelerometer;
  auto node          = AccelerometerNode(logger, time, mqtt, accelerometer);
  node.run();
  return core::Result::kSuccess;
}

AccelerometerNode::AccelerometerNode(core::Logger &logger,
                                     core::ITimeSource &time,
                                     std::shared_ptr<core::Mqtt> mqtt,
                                     std::shared_ptr<Accelerometer> accelerometer)
    : logger_(logger),
      time_(time),
      mqtt_(std::move(mqtt)),
      accelerometer_(std::move(accelerometer))
{
}

void AccelerometerNode::run()
{
  while (true) {
    auto result = accelerometer_->isValueReady();
    if (!result) { continue; }
    if (*result == core::Result::kFailure) { continue; }
    auto optional_acceration = accelerometer_->read();
    if (!optional_acceration) {
      logger_.log(core::LogLevel::kWarn, "Failed to read accelerometer data");
      continue;
    }
    const auto acceleration = *optional_acceration;
    const auto topic        = core::MqttTopic::kAccelerometer;
    auto message_payload    = std::make_shared<rapidjson::Document>();
    message_payload->SetObject();
    rapidjson::Value acceleration_x(acceleration.x);
    rapidjson::Value acceleration_y(acceleration.y);
    rapidjson::Value acceleration_z(acceleration.z);
    message_payload->AddMember("x", acceleration_x, message_payload->GetAllocator());
    message_payload->AddMember("y", acceleration_y, message_payload->GetAllocator());
    message_payload->AddMember("z", acceleration_z, message_payload->GetAllocator());
    const core::MqttMessage::Header header{
      .timestamp = static_cast<std::uint64_t>(time_.now().time_since_epoch().count()),
      .priority  = core::MqttMessagePriority::kNormal};
    const core::MqttMessage message{topic, header, message_payload};
    mqtt_->publish(message, core::MqttMessageQos::kExactlyOnce);
  }
}

}  // namespace hyped::sensors
