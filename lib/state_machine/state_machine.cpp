#include "state_machine.hpp"

#include <iostream>

namespace hyped::state_machine {

StateMachine::StateMachine() : current_state_{State::kIdle}
{
}

// Transition to next state
bool StateMachine::handleMessage(const Message &message)
{
  previous_message_.push(message);
  const auto transition = transition_to_state_.find({current_state_, message});
  if (transition != transition_to_state_.end()) {
    current_state_ = transition->second;
    return true;
  }
  return false;
}

Message StateMachine::stringToMessage(const std::string &message_name)
{
  return string_to_message_.at(message_name);
}

std::string StateMachine::messageToString(const Message &message)
{
  return message_to_string_.at(message);
}

State StateMachine::getCurrentState()
{
  return current_state_;
}

Message StateMachine::getPreviousMessage()
{
  if (previous_message_.empty()) {
    return Message::kNone;
  } else {
    Message previous_message = previous_message_.front();
    previous_message_.pop();
    return previous_message;
  }
}

}  // namespace hyped::state_machine