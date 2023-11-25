#pragma once

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
};

}  // namespace hyped::state_machine