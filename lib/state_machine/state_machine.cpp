#include "state_machine.hpp"

#include <iostream>

namespace hyped::state_machine {

StateMachine::StateMachine() : current_state_{State::kIdle}
{
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

}  // namespace hyped::state_machine