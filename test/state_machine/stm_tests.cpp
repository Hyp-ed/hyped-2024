#include <gtest/gtest.h>

#include <state_machine/state_machine.hpp>

namespace hyped::test {

void testTransition(std::unique_ptr<state_machine::StateMachine> &stm,
                    state_machine::Message transition_message,
                    state_machine::State expected_state)
{
  stm->handleMessage(transition_message);
  ASSERT_TRUE(stm->getCurrentState() == expected_state);
}

TEST(StateMachine, cleanRun)
{
  std::unique_ptr stm = std::make_unique<state_machine::StateMachine>();
  testTransition(stm, state_machine::Message::kFrictionBrakeFail, state_machine::State::kIdle);
  testTransition(stm, state_machine::Message::kCalibrating, state_machine::State::kCalibrating);
  testTransition(stm, state_machine::Message::kReady, state_machine::State::kReady);
  testTransition(stm, state_machine::Message::kAccelerating, state_machine::State::kAccelerating);
  testTransition(stm, state_machine::Message::kCruising, state_machine::State::kCruising);
  testTransition(stm, state_machine::Message::kMotorBrake, state_machine::State::kMotorBraking);
  testTransition(
    stm, state_machine::Message::kPreFrictionBrake, state_machine::State::kPreFrictionBraking);
  testTransition(
    stm, state_machine::Message::kFrictionBrake, state_machine::State::kFrictionBraking);
  testTransition(stm, state_machine::Message::kStopped, state_machine::State::kStopped);
  testTransition(stm, state_machine::Message::kOff, state_machine::State::kOff);
}

TEST(StateMachine, cleanRunDuplicatedMessages)
{
  std::unique_ptr stm = std::make_unique<state_machine::StateMachine>();
  testTransition(stm, state_machine::Message::kFrictionBrakeFail, state_machine::State::kIdle);
  testTransition(stm, state_machine::Message::kCalibrating, state_machine::State::kCalibrating);
  testTransition(stm, state_machine::Message::kCalibrating, state_machine::State::kCalibrating);
  testTransition(stm, state_machine::Message::kReady, state_machine::State::kReady);
  testTransition(stm, state_machine::Message::kReady, state_machine::State::kReady);
  testTransition(stm, state_machine::Message::kAccelerating, state_machine::State::kAccelerating);
  testTransition(stm, state_machine::Message::kAccelerating, state_machine::State::kAccelerating);
  testTransition(stm, state_machine::Message::kCruising, state_machine::State::kCruising);
  testTransition(stm, state_machine::Message::kCruising, state_machine::State::kCruising);
  testTransition(stm, state_machine::Message::kMotorBrake, state_machine::State::kMotorBraking);
  testTransition(stm, state_machine::Message::kMotorBrake, state_machine::State::kMotorBraking);
  testTransition(
    stm, state_machine::Message::kPreFrictionBrake, state_machine::State::kPreFrictionBraking);
  testTransition(
    stm, state_machine::Message::kPreFrictionBrake, state_machine::State::kPreFrictionBraking);
  testTransition(
    stm, state_machine::Message::kFrictionBrake, state_machine::State::kFrictionBraking);
  testTransition(
    stm, state_machine::Message::kFrictionBrake, state_machine::State::kFrictionBraking);
  testTransition(stm, state_machine::Message::kStopped, state_machine::State::kStopped);
  testTransition(stm, state_machine::Message::kStopped, state_machine::State::kStopped);
  testTransition(stm, state_machine::Message::kOff, state_machine::State::kOff);
  testTransition(stm, state_machine::Message::kOff, state_machine::State::kOff);
}

TEST(StateMachine, failureBrakeFromAccelerating)
{
  std::unique_ptr stm = std::make_unique<state_machine::StateMachine>();
  testTransition(stm, state_machine::Message::kFrictionBrakeFail, state_machine::State::kIdle);
  testTransition(stm, state_machine::Message::kCalibrating, state_machine::State::kCalibrating);
  testTransition(stm, state_machine::Message::kReady, state_machine::State::kReady);
  testTransition(stm, state_machine::Message::kAccelerating, state_machine::State::kAccelerating);
  testTransition(stm, state_machine::Message::kFailureBrake, state_machine::State::kFailureBraking);
  testTransition(
    stm, state_machine::Message::kFailureStopped, state_machine::State::kFailureStopped);
  testTransition(stm, state_machine::Message::kFailureOff, state_machine::State::kOff);
}

TEST(StateMachine, failureBrakeFromCruising)
{
  std::unique_ptr stm = std::make_unique<state_machine::StateMachine>();
  testTransition(stm, state_machine::Message::kFrictionBrakeFail, state_machine::State::kIdle);
  testTransition(stm, state_machine::Message::kCalibrating, state_machine::State::kCalibrating);
  testTransition(stm, state_machine::Message::kReady, state_machine::State::kReady);
  testTransition(stm, state_machine::Message::kAccelerating, state_machine::State::kAccelerating);
  testTransition(stm, state_machine::Message::kCruising, state_machine::State::kCruising);
  testTransition(stm, state_machine::Message::kFailureBrake, state_machine::State::kFailureBraking);
  testTransition(
    stm, state_machine::Message::kFailureStopped, state_machine::State::kFailureStopped);
  testTransition(stm, state_machine::Message::kFailureOff, state_machine::State::kOff);
}

TEST(StateMachine, failureBrakeFromMotorBraking)
{
  std::unique_ptr stm = std::make_unique<state_machine::StateMachine>();
  testTransition(stm, state_machine::Message::kFrictionBrakeFail, state_machine::State::kIdle);
  testTransition(stm, state_machine::Message::kCalibrating, state_machine::State::kCalibrating);
  testTransition(stm, state_machine::Message::kReady, state_machine::State::kReady);
  testTransition(stm, state_machine::Message::kAccelerating, state_machine::State::kAccelerating);
  testTransition(stm, state_machine::Message::kCruising, state_machine::State::kCruising);
  testTransition(stm, state_machine::Message::kMotorBrake, state_machine::State::kMotorBraking);
  testTransition(stm, state_machine::Message::kFailureBrake, state_machine::State::kFailureBraking);
  testTransition(
    stm, state_machine::Message::kFailureStopped, state_machine::State::kFailureStopped);
  testTransition(stm, state_machine::Message::kFailureOff, state_machine::State::kOff);
}

TEST(StateMachine, frictionBrakeFailFromAccelerating)
{
  std::unique_ptr stm = std::make_unique<state_machine::StateMachine>();
  testTransition(stm, state_machine::Message::kFrictionBrakeFail, state_machine::State::kIdle);
  testTransition(stm, state_machine::Message::kCalibrating, state_machine::State::kCalibrating);
  testTransition(stm, state_machine::Message::kReady, state_machine::State::kReady);
  testTransition(stm, state_machine::Message::kAccelerating, state_machine::State::kAccelerating);
  testTransition(stm,
                 state_machine::Message::kPreFrictionBrakeFail,
                 state_machine::State::kPreFrictionBrakingFail);
  testTransition(
    stm, state_machine::Message::kFrictionBrakeFail, state_machine::State::kFrictionBrakingFail);
  testTransition(
    stm, state_machine::Message::kFailureStopped, state_machine::State::kFailureStopped);
  testTransition(stm, state_machine::Message::kFailureOff, state_machine::State::kOff);
}

TEST(StateMachine, frictionBrakeFailFromCruising)
{
  std::unique_ptr stm = std::make_unique<state_machine::StateMachine>();
  testTransition(stm, state_machine::Message::kFrictionBrakeFail, state_machine::State::kIdle);
  testTransition(stm, state_machine::Message::kCalibrating, state_machine::State::kCalibrating);
  testTransition(stm, state_machine::Message::kReady, state_machine::State::kReady);
  testTransition(stm, state_machine::Message::kAccelerating, state_machine::State::kAccelerating);
  testTransition(stm, state_machine::Message::kCruising, state_machine::State::kCruising);
  testTransition(stm,
                 state_machine::Message::kPreFrictionBrakeFail,
                 state_machine::State::kPreFrictionBrakingFail);
  testTransition(
    stm, state_machine::Message::kFrictionBrakeFail, state_machine::State::kFrictionBrakingFail);
  testTransition(
    stm, state_machine::Message::kFailureStopped, state_machine::State::kFailureStopped);
  testTransition(stm, state_machine::Message::kFailureOff, state_machine::State::kOff);
}

TEST(StateMachine, frictionBrakeFailFromMotorBraking)
{
  std::unique_ptr stm = std::make_unique<state_machine::StateMachine>();
  testTransition(stm, state_machine::Message::kFrictionBrakeFail, state_machine::State::kIdle);
  testTransition(stm, state_machine::Message::kCalibrating, state_machine::State::kCalibrating);
  testTransition(stm, state_machine::Message::kReady, state_machine::State::kReady);
  testTransition(stm, state_machine::Message::kAccelerating, state_machine::State::kAccelerating);
  testTransition(stm, state_machine::Message::kCruising, state_machine::State::kCruising);
  testTransition(stm, state_machine::Message::kMotorBrake, state_machine::State::kMotorBraking);
  testTransition(stm,
                 state_machine::Message::kPreFrictionBrakeFail,
                 state_machine::State::kPreFrictionBrakingFail);
  testTransition(
    stm, state_machine::Message::kFrictionBrakeFail, state_machine::State::kFrictionBrakingFail);
  testTransition(
    stm, state_machine::Message::kFailureStopped, state_machine::State::kFailureStopped);
  testTransition(stm, state_machine::Message::kFailureOff, state_machine::State::kOff);
}

TEST(StateMachine, failureBrakeFromFrictionBraking)
{
  std::unique_ptr stm = std::make_unique<state_machine::StateMachine>();
  testTransition(stm, state_machine::Message::kFrictionBrakeFail, state_machine::State::kIdle);
  testTransition(stm, state_machine::Message::kCalibrating, state_machine::State::kCalibrating);
  testTransition(stm, state_machine::Message::kReady, state_machine::State::kReady);
  testTransition(stm, state_machine::Message::kAccelerating, state_machine::State::kAccelerating);
  testTransition(stm,
                 state_machine::Message::kPreFrictionBrakeFail,
                 state_machine::State::kPreFrictionBrakingFail);
  testTransition(
    stm, state_machine::Message::kFrictionBrakeFail, state_machine::State::kFrictionBrakingFail);
  testTransition(stm, state_machine::Message::kFailureBrake, state_machine::State::kFailureBraking);
  testTransition(
    stm, state_machine::Message::kFailureStopped, state_machine::State::kFailureStopped);
  testTransition(stm, state_machine::Message::kFailureOff, state_machine::State::kOff);
}

TEST(StateMachine, duplicatedMessagesFailureStates)
{
  std::unique_ptr stm = std::make_unique<state_machine::StateMachine>();
  testTransition(stm, state_machine::Message::kFrictionBrakeFail, state_machine::State::kIdle);
  testTransition(stm, state_machine::Message::kFrictionBrakeFail, state_machine::State::kIdle);
  testTransition(stm, state_machine::Message::kCalibrating, state_machine::State::kCalibrating);
  testTransition(stm, state_machine::Message::kCalibrating, state_machine::State::kCalibrating);
  testTransition(stm, state_machine::Message::kReady, state_machine::State::kReady);
  testTransition(stm, state_machine::Message::kReady, state_machine::State::kReady);
  testTransition(stm, state_machine::Message::kAccelerating, state_machine::State::kAccelerating);
  testTransition(stm, state_machine::Message::kAccelerating, state_machine::State::kAccelerating);
  testTransition(stm,
                 state_machine::Message::kPreFrictionBrakeFail,
                 state_machine::State::kPreFrictionBrakingFail);
  testTransition(stm,
                 state_machine::Message::kPreFrictionBrakeFail,
                 state_machine::State::kPreFrictionBrakingFail);
  testTransition(
    stm, state_machine::Message::kFrictionBrakeFail, state_machine::State::kFrictionBrakingFail);
  testTransition(
    stm, state_machine::Message::kFrictionBrakeFail, state_machine::State::kFrictionBrakingFail);
  testTransition(stm, state_machine::Message::kFailureBrake, state_machine::State::kFailureBraking);
  testTransition(stm, state_machine::Message::kFailureBrake, state_machine::State::kFailureBraking);
  testTransition(
    stm, state_machine::Message::kFailureStopped, state_machine::State::kFailureStopped);
  testTransition(
    stm, state_machine::Message::kFailureStopped, state_machine::State::kFailureStopped);
  testTransition(stm, state_machine::Message::kFailureOff, state_machine::State::kOff);
  testTransition(stm, state_machine::Message::kFailureOff, state_machine::State::kOff);
}

}  // namespace hyped::test