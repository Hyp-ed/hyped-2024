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
  Message stringToMessage(const std::string &message_name);
  std::string messageToString(const Message &message);
  State getCurrentState();

 private:
  const std::unordered_map<std::string, Message> string_to_message_
    = {{"kCalibrating", Message::kCalibrating},
       {"kPrecharge", Message::kPrecharge},
       {"kReadyForLeviation", Message::kReadyForLevitation}, 
       {"kBeginLevitation", Message::kBeginLevitation},
       {"kReady", Message::kReady},
       {"kAccelerate", Message::kAccelerate},
       {"kLIMBrake", Message::kLIMBrake},
       {"kFrictionBrake", Message::kFrictionBrake},
       {"kStopLevitation", Message::kStopLevitation},
       {"kStopped", Message::kStopped},
       {"kBatteryRecharge", Message::kBatteryRecharge},
       {"kCapacitorDischarge", Message::kCapacitorDischarge},
       {"kFailureBrake", Message::kFailureBrake},
       {"kFailure", Message::kFailure},
       {"kSafe", Message::kSafe}};
  const std::unordered_map<Message, std::string> message_to_string_
    = {{Message::kCalibrating, "kCalibrating"},
       {Message::kPrecharge, "kPrecharge"},
       {Message::kReadyForLevitation, "kReadyForLevitation"},
       {Message::kBeginLevitation, "kBeginLevitation"},
       {Message::kReady, "kReady"},
       {Message::kAccelerate, "kAccelerate"},
       {Message::kLIMBrake, "kLIMBrake"},
       {Message::kFrictionBrake, "kFrictionBrake"},
       {Message::kStopLevitation, "kStopLevitation"},
       {Message::kStopped, "kStopped"},
       {Message::kBatteryRecharge, "kBatteryRecharge"},
       {Message::kCapacitorDischarge, "kCapacitorDischarge"},
       {Message::kFailureBrake, "kFailureBrake"},
       {Message::kFailure, "kFailure"},
       {Message::kSafe, "kSafe"}};
  const boost::unordered_map<SourceAndMessage, State, source_and_message_hash> transition_to_state_
    = {{{State::kIdle, Message::kCalibrating}, State::kCalibrating},
       {{State::kIdle, Message::kFailure}, State::kFailure},
       {{State::kCalibrating, Message::kPrecharge}, State::kPrecharge},
       {{State::kCalibrating, Message::kFailure}, State::kFailure},
       {{State::kPrecharge, Message::kReadyForLevitation}, State::kReadyForLevitation},
       {{State::kPrecharge, Message::kFailure}, State::kFailure},
       {{State::kReadyForLevitation, Message::kBeginLevitation}, State::kBeginLevitation},
       {{State::kReadyForLevitation, Message::kFailure}, State::kFailure},
       {{State::kBeginLevitation, Message::kReady}, State::kReady},
       {{State::kBeginLevitation, Message::kFailure}, State::kFailure},
       {{State::kReady, Message::kAccelerate}, State::kAccelerate},
       {{State::kAccelerate, Message::kLIMBrake}, State::kLIMBrake},
       {{State::kAccelerate, Message::kFailureBrake}, State::kFailureBrake},
       {{State::kLIMBrake, Message::kFrictionBrake}, State::kFrictionBrake},
       {{State::kLIMBrake, Message::kFailureBrake}, State::kFailureBrake},
       {{State::kFrictionBrake, Message::kStopLevitation}, State::kStopLevitation},
       {{State::kFrictionBrake, Message::kFailureBrake}, State::kFailureBrake},
       {{State::kStopLevitation, Message::kStopped}, State::kStopped},
       {{State::kStopped, Message::kBatteryRecharge}, State::kBatteryRecharge},
       {{State::kBatteryRecharge, Message::kCapacitorDischarge}, State::kCapacitorDischarge},
       {{State::kFailureBrake, Message::kCapacitorDischarge}, State::kCapacitorDischarge},
       {{State::kFailure, Message::kCapacitorDischarge}, State::kCapacitorDischarge},
       {{State::kCapacitorDischarge, Message::kSafe}, State::kSafe}};

  State current_state_;
};

}  // namespace hyped::state_machine