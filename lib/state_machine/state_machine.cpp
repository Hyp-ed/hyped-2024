#include "state_machine.hpp"

#include <iostream>

#include "rapidjson/document.h"

namespace hyped::state_machine {

StateMachine::StateMachine(std::shared_ptr<core::Mqtt> mqtt)
    : current_state_(State::kIdle),
      mqtt_(std::move(mqtt))
{
  mqtt_->subscribe(core::MqttTopic::kTest);
}

StateMachine::StateMachine() : current_state_(State::kIdle)
{
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

void StateMachine::startStateMachine()
{
  while (true) {
    StateMachine::updateStateMachine();
    // TODO: function to publish state we are currently in, mqtt message.
  }
}

}  // namespace hyped::state_machine