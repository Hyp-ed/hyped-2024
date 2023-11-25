#pragma once

#include "types.hpp"

#include <optional>
#include <queue>
#include <unordered_map>

#include <boost/unordered_map.hpp>

namespace hyped::state_machine {
typedef boost::unordered_map<SourceAndTarget, State, source_and_target_hash> TransitionTable;

const TransitionTable transition_to_state_dynamic
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
     {{State::kStopLevitation, State::kFailure}, State::kFailure},
     {{State::kStopped, State::kBatteryRecharge}, State::kBatteryRecharge},
     {{State::kStopped, State::kFailure}, State::kFailure},
     {{State::kBatteryRecharge, State::kCapacitorDischarge}, State::kCapacitorDischarge},
     {{State::kBatteryRecharge, State::kFailure}, State::kFailure},
     {{State::kFailureBrake, State::kCapacitorDischarge}, State::kCapacitorDischarge},
     {{State::kFailure, State::kCapacitorDischarge}, State::kCapacitorDischarge},
     {{State::kCapacitorDischarge, State::kSafe}, State::kSafe}};

const TransitionTable transition_to_state_static
  = {{{State::kIdle, State::kCalibrate}, State::kCalibrate},
     {{State::kIdle, State::kFailure}, State::kFailure},
     {{State::kCalibrate, State::kPrecharge}, State::kPrecharge},
     {{State::kCalibrate, State::kFailure}, State::kFailure},
     {{State::kPrecharge, State::kReadyForLevitation}, State::kReadyForLevitation},
     {{State::kPrecharge, State::kFailure}, State::kFailure},
     {{State::kReadyForLevitation, State::kBeginLevitation}, State::kBeginLevitation},
     {{State::kReadyForLevitation, State::kFailure}, State::kFailure},
     {{State::kBeginLevitation, State::kLevitating}, State::kLevitating},
     {{State::kBeginLevitation, State::kFailureBrake}, State::kFailureBrake},
     {{State::kLevitating, State::kFrictionBrake}, State::kFrictionBrake},
     {{State::kLevitating, State::kFailureBrake}, State::kFailureBrake},
     {{State::kFrictionBrake, State::kStopLevitation}, State::kStopLevitation},
     {{State::kFrictionBrake, State::kFailureBrake}, State::kFailureBrake},
     {{State::kStopLevitation, State::kStopped}, State::kStopped},
     {{State::kStopLevitation, State::kFailureBrake}, State::kFailureBrake},
     {{State::kStopped, State::kCapacitorDischarge}, State::kCapacitorDischarge},
     {{State::kStopped, State::kFailure}, State::kFailure},
     {{State::kFailureBrake, State::kCapacitorDischarge}, State::kCapacitorDischarge},
     {{State::kFailure, State::kCapacitorDischarge}, State::kCapacitorDischarge},
     {{State::kCapacitorDischarge, State::kSafe}, State::kSafe}};
}  // namespace hyped::state_machine
