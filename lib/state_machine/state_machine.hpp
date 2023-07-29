#pragma once

#include "state.hpp"
#include "types.hpp"

#include <optional>
#include <queue>
#include <unordered_map>

#include <boost/unordered_map.hpp>

namespace hyped::state_machine {

class StateMachine {
 public:
  StateMachine();
  bool handleMessage(const Message &message);
  Message stringToMessage(const std::string &message_name);
  std::string messageToString(const Message &message);
  State getCurrentState();
  Message getPreviousMessage();

 private:
  const std::unordered_map<std::string, Message> string_to_message_
    = {{"kCalibrating", Message::kCalibrating},
       {"kReady", Message::kReady},
       {"kAccelerating", Message::kAccelerating},
       {"kCruising", Message::kCruising},
       {"kMotorBrake", Message::kMotorBrake},
       {"kPreFrictionBrake", Message::kPreFrictionBrake},
       {"kFrictionBrake", Message::kFrictionBrake},
       {"kStopped", Message::kStopped},
       {"kFailureBrake", Message::kFailureBrake},
       {"kPreFrictionBrakeFail", Message::kPreFrictionBrakeFail},
       {"kFrictionBrakeFail", Message::kFrictionBrakeFail},
       {"kOff", Message::kOff},
       {"kFailureStopped", Message::kFailureStopped},
       {"kFailureOff", Message::kFailureOff}};
  const std::unordered_map<Message, std::string> message_to_string_
    = {{Message::kCalibrating, "kCalibrating"},
       {Message::kReady, "kReady"},
       {Message::kAccelerating, "kAccelerating"},
       {Message::kCruising, "kCruising"},
       {Message::kMotorBrake, "kMotorBrake"},
       {Message::kPreFrictionBrake, "kPreFrictionBrake"},
       {Message::kFrictionBrake, "kFrictionBrake"},
       {Message::kStopped, "kStopped"},
       {Message::kFailureBrake, "kFailureBrake"},
       {Message::kPreFrictionBrakeFail, "kPreFrictionBrakeFail"},
       {Message::kFrictionBrakeFail, "kFrictionBrakeFail"},
       {Message::kOff, "kOff"},
       {Message::kFailureStopped, "kFailureStopped"},
       {Message::kFailureOff, "kFailureOff"}};
  const boost::unordered_map<SourceAndMessage, State, source_and_message_hash> transition_to_state_
    = {{{State::kIdle, Message::kCalibrating}, State::kCalibrating},
       {{State::kCalibrating, Message::kReady}, State::kReady},
       {{State::kReady, Message::kAccelerating}, State::kAccelerating},
       {{State::kAccelerating, Message::kCruising}, State::kCruising},
       {{State::kCruising, Message::kMotorBrake}, State::kMotorBraking},
       {{State::kMotorBraking, Message::kPreFrictionBrake}, State::kPreFrictionBraking},
       {{State::kPreFrictionBraking, Message::kFrictionBrake}, State::kFrictionBraking},
       {{State::kFrictionBraking, Message::kStopped}, State::kStopped},
       {{State::kStopped, Message::kOff}, State::kOff},
       {{State::kAccelerating, Message::kFailureBrake}, State::kFailureBraking},
       {{State::kAccelerating, Message::kPreFrictionBrakeFail}, State::kPreFrictionBrakingFail},
       {{State::kPreFrictionBrakingFail, Message::kFrictionBrakeFail}, State::kFrictionBrakingFail},
       {{State::kCruising, Message::kFailureBrake}, State::kFailureBraking},
       {{State::kCruising, Message::kPreFrictionBrakeFail}, State::kPreFrictionBrakingFail},
       {{State::kMotorBraking, Message::kFailureBrake}, State::kFailureBraking},
       {{State::kMotorBraking, Message::kPreFrictionBrakeFail}, State::kPreFrictionBrakingFail},
       {{State::kFrictionBrakingFail, Message::kFailureBrake}, State::kFailureBraking},
       {{State::kFrictionBrakingFail, Message::kFailureStopped}, State::kFailureStopped},
       {{State::kFailureBraking, Message::kFailureStopped}, State::kFailureStopped},
       {{State::kFailureStopped, Message::kFailureOff}, State::kOff}};

  State current_state_;
  std::queue<Message> previous_message_;
};

}  // namespace hyped::state_machine