#include "state_machine.hpp"

#include <iostream>

#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

#include <core/mqtt_message.hpp>
#include <core/wall_clock.hpp>

namespace hyped::state_machine {

StateMachine::StateMachine(std::shared_ptr<core::IMqtt> mqtt,
                           const TransitionTable &transition_table)
    : current_state_(State::kIdle),
      mqtt_(std::move(mqtt)),
      transition_to_state_(transition_table)
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
  const auto next_message = mqtt_->getMessage();
  if (!next_message) { return; }

  const auto optional_payload = next_message->state_message;
  if (!optional_payload) { return; }
  const auto message_state = (*optional_payload).state;
  handleTransition(message_state);
}

void StateMachine::publishCurrentState()
{
  const auto state = getCurrentState();
  const auto topic = core::MqttTopic::kTest;
  const core::MqttMessage::Header header{.timestamp = 0,
                                         .priority  = core::MqttMessagePriority::kNormal};
  const core::MqttMessage message(topic, header, core::StateMessage{.state = state});
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
  const auto optional_mqtt = core::Mqtt::create(logger, "state_machine", mqtt_ip, mqtt_port);
  if (!optional_mqtt) {
    logger.log(core::LogLevel::kFatal, "Failed to create MQTT client");
    return core::Result::kFailure;
  }
  auto mqtt = *optional_mqtt;
  const auto optional_transition_list
    = config["state_machine"]["transition_table"].value<std::string>();
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
    logger.log(core::LogLevel::kFatal, "Unknown transition list: %s", transition_list);
    return core::Result::kFailure;
  }
  return core::Result::kSuccess;
}

}  // namespace hyped::state_machine
