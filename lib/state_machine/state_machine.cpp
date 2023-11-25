#include "state_machine.hpp"

#include <iostream>

#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

namespace hyped::state_machine {

StateMachine::StateMachine(std::shared_ptr<core::IMqtt> mqtt)
    : current_state_(State::kIdle),
      mqtt_(std::move(mqtt))
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

core::Result StateMachine::updateStateMachine()
{
  mqtt_->consume();
  const auto nextMessage = mqtt_->getMessage();
  if (!nextMessage) { return core::Result::kFailure; }

  const auto doc           = nextMessage->payload;
  const auto message_state = string_to_state_.find((*doc)["transition"].GetString());

  if (message_state != string_to_state_.end()) {
    StateMachine::handleTransition(message_state->second);
    return core::Result::kSuccess;
  }

  return core::Result::kFailure;
}

void StateMachine::publishCurrentState(const State &state)
{
  const auto state_string         = StateMachine::stateToString(state);
  const auto topic                = core::MqttTopic::kTest;
  std::shared_ptr message_payload = std::make_shared<rapidjson::Document>();
  message_payload->SetObject();
  rapidjson::Value state_value(state_string.c_str(), message_payload->GetAllocator());
  message_payload->AddMember("transition", state_value, message_payload->GetAllocator());
  core::MqttMessage::Header header{.timestamp = 0, .priority = core::MqttMessagePriority::kNormal};
  const core::MqttMessage message{topic, header, message_payload};
  mqtt_->publish(message, core::MqttMessageQos::kExactlyOnce);
}

void StateMachine::startStateMachine()
{
  while (true) {
    StateMachine::updateStateMachine();
    StateMachine::publishCurrentState(StateMachine::getCurrentState());
  }
}

}  // namespace hyped::state_machine