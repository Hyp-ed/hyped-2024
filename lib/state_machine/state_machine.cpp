#include "state_machine.hpp"

#include <iostream>

#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

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
  const auto nextMessage = mqtt_->getMessage();
  if (!nextMessage) { return; }

  const auto payload           = nextMessage->payload;
  const auto message_state = string_to_state_.find((*payload)["transition"].GetString());

  if (message_state != string_to_state_.end()) { handleTransition(message_state->second); }
}

void StateMachine::publishCurrentState()
{
  const auto state_string         = stateToString(getCurrentState());
  const auto topic                = core::MqttTopic::kTest;
  auto message_payload = std::make_shared<rapidjson::Document>();
  message_payload->SetObject();
  rapidjson::Value state_value(state_string.c_str(), message_payload->GetAllocator());
  message_payload->AddMember("transition", state_value, message_payload->GetAllocator());
  const core::MqttMessage::Header header{.timestamp = 0, .priority = core::MqttMessagePriority::kNormal};
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

}  // namespace hyped::state_machine