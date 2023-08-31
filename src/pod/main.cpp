#include <iostream>

#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

#include <core/logger.hpp>
#include <core/mqtt.hpp>
#include <core/timer.hpp>
#include <core/types.hpp>
#include <core/wall_clock.hpp>
#include <io/hardware_gpio.hpp>

int main(int argc, char **argv)
{
  hyped::core::WallClock time;
  hyped::core::Timer timer(time);
  const auto execution_time = timer.measureExecutionTime([time]() {
    hyped::core::Logger logger("MQTT", hyped::core::LogLevel::kDebug, time);
    const std::string id   = "test";
    const uint16_t port    = 8080;
    const std::string host = "localhost";
    auto optional_mqtt     = hyped::core::Mqtt::create(logger, id, host, port);
    if (!optional_mqtt) {
      std::cout << "Failed to connect to MQTT broker" << std::endl;
      return;
    }
    auto mqtt        = *optional_mqtt;
    const auto topic = hyped::core::MqttTopic::kTest;
    rapidjson::Document message_payload;
    message_payload.SetObject();
    message_payload.AddMember("word", "Hello world!", message_payload.GetAllocator());
    message_payload.AddMember("number", 42, message_payload.GetAllocator());
    const auto message = hyped::core::MqttMessage{
      .header  = {.timestamp = 0, .priority = hyped::core::MqttMessagePriority::kCritical},
      .payload = message_payload,
    };
    mqtt->publish(topic, message, hyped::core::MqttMessageQos::kAtLeastOnce);
    mqtt->subscribe(hyped::core::MqttTopic::kTest);
  });
  std::cout << "Ran for " << execution_time.count() << " ns" << std::endl;
}
