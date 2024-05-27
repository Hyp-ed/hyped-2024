#pragma once

#include <string>
#include <unordered_map>

namespace hyped::state_machine {

enum class State {
  kIdle,
  kCalibrate,
  kPrecharge,
  kReadyForLevitation,
  kBeginLevitation,
  kLevitating,
  kReady,
  kAccelerate,
  kLimBrake,
  kFrictionBrake,
  kStopLevitation,
  kStopped,
  kBatteryRecharge,
  kCapacitorDischarge,
  kFailureBrake,
  kFailure,
  kSafe,
  kShutdown
};

const std::unordered_map<std::string, State> string_to_state
  = {{"kIdle", State::kIdle},
     {"kCalibrate", State::kCalibrate},
     {"kPrecharge", State::kPrecharge},
     {"kReadyForLeviation", State::kReadyForLevitation},
     {"kBeginLevitation", State::kBeginLevitation},
     {"kLevitating", State::kLevitating},
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

const std::unordered_map<State, std::string> state_to_string
  = {{State::kIdle, "kIdle"},
     {State::kCalibrate, "kCalibrate"},
     {State::kPrecharge, "kPrecharge"},
     {State::kReadyForLevitation, "kReadyForLevitation"},
     {State::kBeginLevitation, "kBeginLevitation"},
     {State::kLevitating, "kLevitating"},
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
}  // namespace hyped::state_machine
