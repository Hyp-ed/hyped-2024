#pragma once

namespace hyped::state_machine {

enum class State {
  kIdle,
  kCalibrating,
  kPrecharge,
  kReadyForLevitation,
  kBeginLevitation,
  kReady,
  kAccelerate,
  kLIMBrake,
  kFrictionBrake,
  kStopLevitation,
  kStopped,
  kBatteryRecharge,
  kCapacitorDischarge,
  kFailureBrake,
  kFailure,
  kSafe,
};

}  // namespace hyped::state_machine