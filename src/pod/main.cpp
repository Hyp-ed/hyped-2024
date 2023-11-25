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
#include <state_machine/state_machine.hpp>

int main(int argc, char **argv)
{
  hyped::core::WallClock time;
  hyped::core::Timer timer(time);
  const auto execution_time = timer.measureExecutionTime([time]() {
    hyped::core::Logger logger("MQTT", hyped::core::LogLevel::kDebug, time);
    const std::string id     = "test";
    const std::uint16_t port = 1883;
    const std::string host   = "localhost";
    auto optional_mqtt       = hyped::core::Mqtt::create(logger, id, host, port);
    if (!optional_mqtt) {
      std::cout << "Failed to connect to MQTT broker" << std::endl;
      return;
    }
    std::cout << "Connected to MQTT broker" << std::endl;
    auto mqtt                       = *optional_mqtt;
    //const auto topic                = hyped::core::MqttTopic::kTest;
    //std::shared_ptr message_payload = std::make_unique<rapidjson::Document>();
    //message_payload->SetObject();
    //message_payload->AddMember("word", "Hello world!", message_payload->GetAllocator());
    //message_payload->AddMember("number", 42, message_payload->GetAllocator());
    //hyped::core::MqttMessage::Header header{
    //  .timestamp = 0, .priority = hyped::core::MqttMessagePriority::kCritical};
    //const hyped::core::MqttMessage message{topic, header, message_payload};
    //mqtt->subscribe(hyped::core::MqttTopic::kTest);
    //mqtt->publish(message, hyped::core::MqttMessageQos::kAtLeastOnce);
    //mqtt->consume();
   
    auto stm = std::make_unique<hyped::state_machine::StateMachine>(mqtt);
    stm->startStateMachine();
  });
  std::cout << "Ran for " << execution_time.count() << " ns" << std::endl;
}
