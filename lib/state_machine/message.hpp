#pragma once

namespace hyped::state_machine {

// Message class containing messages that prompt transitions
enum class Message {
  kNone,
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