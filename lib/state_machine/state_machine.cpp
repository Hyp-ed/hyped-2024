#include "state_machine.hpp"

#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

#include <core/wall_clock.hpp>

namespace hyped::state_machine {

StateMachine::StateMachine(std::shared_ptr<core::IMqtt> mqtt, TransitionTable transition_table)
    : current_state_(State::kIdle),
      mqtt_(std::move(mqtt)),
      transition_to_state_(std::move(transition_table))
{
  mqtt_->subscribe(core::MqttTopic::kTest);
}

State StateMachine::stringToState(const std::string &state_name)
{
  return string_to_state_.at(state_name);
}

std::string StateMachine::stateToString(const State &state)
{
  return state_to_string_.at(state);
}

State StateMachine::getCurrentState()
{
  return current_state_;
}

core::Result StateMachine::handleTransition(const State &state)
{
  const auto next_state = transition_to_state_.find({current_state_, state});
  if (next_state != transition_to_state_.end()) {
    current_state_ = next_state->second;
    return core::Result::kSuccess;
  }
  return core::Result::kFailure;
}

void StateMachine::update()
{
  const auto nextMessage = mqtt_->getMessage();
  if (!nextMessage) { return; }

  const auto payload       = nextMessage->payload;
  const auto message_state = string_to_state_.find((*payload)["transition"].GetString());

  if (message_state != string_to_state_.end()) { handleTransition(message_state->second); }
}

void StateMachine::publishCurrentState()
{
  const auto state_string = stateToString(getCurrentState());
  const auto topic        = core::MqttTopic::kTest;
  auto message_payload    = std::make_shared<rapidjson::Document>();
  message_payload->SetObject();
  rapidjson::Value state_value(state_string.c_str(), message_payload->GetAllocator());
  message_payload->AddMember("transition", state_value, message_payload->GetAllocator());
  const core::MqttMessage::Header header{.timestamp = 0,
                                         .priority  = core::MqttMessagePriority::kNormal};
  const core::MqttMessage message{topic, header, message_payload};
  mqtt_->publish(message, core::MqttMessageQos::kExactlyOnce);
}

void StateMachine::run()
{
  while (getCurrentState() != State::kShutdown) {
    mqtt_->consume();
    update();
    publishCurrentState();
  }
  publishCurrentState();
}

core::Result StateMachine::startNode(toml::v3::node_view<const toml::v3::node> config,
                                     const std::string &mqtt_ip,
                                     const std::uint32_t mqtt_port)
{
  core::WallClock wall_clock;
  core::Logger logger("STATE_MACHINE", core::LogLevel::kDebug, wall_clock);
  auto optional_mqtt = core::Mqtt::create(logger, "state_machine", mqtt_ip, mqtt_port);
  if (!optional_mqtt) {
    logger.log(core::LogLevel::kFatal, "Failed to create MQTT client");
    return core::Result::kFailure;
  }
  auto mqtt                     = *optional_mqtt;
  auto optional_transition_list = config["state_machine"]["transition_table"].value<std::string>();
  if (!optional_transition_list) {
    logger.log(core::LogLevel::kFatal, "Failed to get transition list from config");
    return core::Result::kFailure;
  }
  const std::string transition_list = *optional_transition_list;
  if (transition_list == "full_run") {
    const state_machine::TransitionTable transition_table
      = state_machine::transition_to_state_dynamic;
    state_machine::StateMachine state_machine(mqtt, transition_table);
    state_machine.run();
  } else if (transition_list == "static_run") {
    const state_machine::TransitionTable transition_table
      = state_machine::transition_to_state_static;
    state_machine::StateMachine state_machine(mqtt, transition_table);
    state_machine.run();
  } else {
    logger.log(core::LogLevel::kFatal, "Unknown transition list: %s", transition_list.c_str());
    return core::Result::kFailure;
  }
  return core::Result::kSuccess;
}

}  // namespace hyped::state_machine
