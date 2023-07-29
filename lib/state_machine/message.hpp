#pragma once

namespace hyped::state_machine {

// Message class containing messages that prompt transitions
enum class Message {
  kNone,
  kCalibrating,
  kReady,
  kAccelerating,
  kCruising,
  kMotorBrake,
  kPreFrictionBrake,
  kFrictionBrake,
  kStopped,
  kFailureBrake,
  kPreFrictionBrakeFail,
  kFrictionBrakeFail,
  kOff,
  kFailureStopped,
  kFailureOff,
};

}  // namespace hyped::state_machine