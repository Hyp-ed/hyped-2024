#pragma once

#include "state.hpp"
#include "types.hpp"

#include <optional>
#include <queue>
#include <unordered_map>

#include "core/mqtt.hpp"
#include <boost/unordered_map.hpp>
#include <core/types.hpp>

namespace hyped::state_machine {

class StateMachine {
 public:
  StateMachine(std::shared_ptr<core::Mqtt> mqtt);
  StateMachine();
  State stringToState(const std::string &state_name);
  std::string stateToString(const State &state);
  State getCurrentState();
  core::Result handleTransition(const State &state);
  core::Result updateStateMachine();

 private:
  const std::unordered_map<std::string, State> string_to_state_
    = {{"kCalibrate", State::kCalibrate},
       {"kPrecharge", State::kPrecharge},
       {"kReadyForLeviation", State::kReadyForLevitation},
       {"kBeginLevitation", State::kBeginLevitation},
       {"kReady", State::kReady},
       {"kAccelerate", State::kAccelerate},
       {"kLimBrake", State::kLimBrake},
       {"kFrictionBrake", State::kFrictionBrake},
       {"kStopLevitation", State::kStopLevitation},
       {"kStopped", State::kStopped},
       {"kBatteryRecharge", State::kBatteryRecharge},
       {"kCapacitorDischarge", State::kCapacitorDischarge},
       {"kFailureBrake", State::kFailureBrake},
       {"kFailure", State::kFailure},
       {"kSafe", State::kSafe}};
  const std::unordered_map<State, std::string> state_to_string_
    = {{State::kCalibrate, "kCalibrate"},
       {State::kPrecharge, "kPrecharge"},
       {State::kReadyForLevitation, "kReadyForLevitation"},
       {State::kBeginLevitation, "kBeginLevitation"},
       {State::kReady, "kReady"},
       {State::kAccelerate, "kAccelerate"},
       {State::kLimBrake, "kLimBrake"},
       {State::kFrictionBrake, "kFrictionBrake"},
       {State::kStopLevitation, "kStopLevitation"},
       {State::kStopped, "kStopped"},
       {State::kBatteryRecharge, "kBatteryRecharge"},
       {State::kCapacitorDischarge, "kCapacitorDischarge"},
       {State::kFailureBrake, "kFailureBrake"},
       {State::kFailure, "kFailure"},
       {State::kSafe, "kSafe"}};
  const boost::unordered_map<SourceAndTarget, State, source_and_target_hash> transition_to_state_
    = {{{State::kIdle, State::kCalibrate}, State::kCalibrate},
       {{State::kIdle, State::kFailure}, State::kFailure},
       {{State::kCalibrate, State::kPrecharge}, State::kPrecharge},
       {{State::kCalibrate, State::kFailure}, State::kFailure},
       {{State::kPrecharge, State::kReadyForLevitation}, State::kReadyForLevitation},
       {{State::kPrecharge, State::kFailure}, State::kFailure},
       {{State::kReadyForLevitation, State::kBeginLevitation}, State::kBeginLevitation},
       {{State::kReadyForLevitation, State::kFailure}, State::kFailure},
       {{State::kBeginLevitation, State::kReady}, State::kReady},
       {{State::kBeginLevitation, State::kFailure}, State::kFailure},
       {{State::kReady, State::kAccelerate}, State::kAccelerate},
       {{State::kAccelerate, State::kLimBrake}, State::kLimBrake},
       {{State::kAccelerate, State::kFailureBrake}, State::kFailureBrake},
       {{State::kLimBrake, State::kFrictionBrake}, State::kFrictionBrake},
       {{State::kLimBrake, State::kFailureBrake}, State::kFailureBrake},
       {{State::kFrictionBrake, State::kStopLevitation}, State::kStopLevitation},
       {{State::kFrictionBrake, State::kFailureBrake}, State::kFailureBrake},
       {{State::kStopLevitation, State::kStopped}, State::kStopped},
       {{State::kStopped, State::kBatteryRecharge}, State::kBatteryRecharge},
       {{State::kBatteryRecharge, State::kCapacitorDischarge}, State::kCapacitorDischarge},
       {{State::kFailureBrake, State::kCapacitorDischarge}, State::kCapacitorDischarge},
       {{State::kFailure, State::kCapacitorDischarge}, State::kCapacitorDischarge},
       {{State::kCapacitorDischarge, State::kSafe}, State::kSafe}};

  State current_state_;
  std::shared_ptr<core::Mqtt> mqtt_;
};

}  // namespace hyped::state_machine