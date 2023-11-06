#include <gtest/gtest.h>

#include <state_machine/state_machine.hpp>

namespace hyped::test {

void testTransition(std::unique_ptr<state_machine::StateMachine> &stm,
                    state_machine::State transition_state,
                    state_machine::State expected_state)
{
  stm->handleTransition(transition_state);
  ASSERT_TRUE(stm->getCurrentState() == expected_state);
}

TEST(StateMachine, cleanRun)
{
  std::unique_ptr stm = std::make_unique<state_machine::StateMachine>();
  testTransition(stm, state_machine::State::kFailureBrake, state_machine::State::kIdle);
  testTransition(stm, state_machine::State::kCalibrate, state_machine::State::kCalibrate);
  testTransition(stm, state_machine::State::kPrecharge, state_machine::State::kPrecharge);
  testTransition(
    stm, state_machine::State::kReadyForLevitation, state_machine::State::kReadyForLevitation);
  testTransition(
    stm, state_machine::State::kBeginLevitation, state_machine::State::kBeginLevitation);
  testTransition(stm, state_machine::State::kReady, state_machine::State::kReady);
  testTransition(stm, state_machine::State::kAccelerate, state_machine::State::kAccelerate);
  testTransition(stm, state_machine::State::kLimBrake, state_machine::State::kLimBrake);
  testTransition(stm, state_machine::State::kFrictionBrake, state_machine::State::kFrictionBrake);
  testTransition(stm, state_machine::State::kStopLevitation, state_machine::State::kStopLevitation);
  testTransition(stm, state_machine::State::kStopped, state_machine::State::kStopped);
  testTransition(
    stm, state_machine::State::kBatteryRecharge, state_machine::State::kBatteryRecharge);
  testTransition(
    stm, state_machine::State::kCapacitorDischarge, state_machine::State::kCapacitorDischarge);
  testTransition(stm, state_machine::State::kSafe, state_machine::State::kSafe);
}

TEST(StateMachine, cleanRunDuplicatedMessages)
{
  std::unique_ptr stm = std::make_unique<state_machine::StateMachine>();
  testTransition(stm, state_machine::State::kFailureBrake, state_machine::State::kIdle);
  testTransition(stm, state_machine::State::kFailureBrake, state_machine::State::kIdle);
  testTransition(stm, state_machine::State::kCalibrate, state_machine::State::kCalibrate);
  testTransition(stm, state_machine::State::kCalibrate, state_machine::State::kCalibrate);
  testTransition(stm, state_machine::State::kPrecharge, state_machine::State::kPrecharge);
  testTransition(stm, state_machine::State::kPrecharge, state_machine::State::kPrecharge);
  testTransition(
    stm, state_machine::State::kReadyForLevitation, state_machine::State::kReadyForLevitation);
  testTransition(
    stm, state_machine::State::kReadyForLevitation, state_machine::State::kReadyForLevitation);
  testTransition(
    stm, state_machine::State::kBeginLevitation, state_machine::State::kBeginLevitation);
  testTransition(
    stm, state_machine::State::kBeginLevitation, state_machine::State::kBeginLevitation);
  testTransition(stm, state_machine::State::kReady, state_machine::State::kReady);
  testTransition(stm, state_machine::State::kReady, state_machine::State::kReady);
  testTransition(stm, state_machine::State::kAccelerate, state_machine::State::kAccelerate);
  testTransition(stm, state_machine::State::kAccelerate, state_machine::State::kAccelerate);
  testTransition(stm, state_machine::State::kLimBrake, state_machine::State::kLimBrake);
  testTransition(stm, state_machine::State::kLimBrake, state_machine::State::kLimBrake);
  testTransition(stm, state_machine::State::kFrictionBrake, state_machine::State::kFrictionBrake);
  testTransition(stm, state_machine::State::kFrictionBrake, state_machine::State::kFrictionBrake);
  testTransition(stm, state_machine::State::kStopLevitation, state_machine::State::kStopLevitation);
  testTransition(stm, state_machine::State::kStopLevitation, state_machine::State::kStopLevitation);
  testTransition(stm, state_machine::State::kStopped, state_machine::State::kStopped);
  testTransition(stm, state_machine::State::kStopped, state_machine::State::kStopped);
  testTransition(
    stm, state_machine::State::kBatteryRecharge, state_machine::State::kBatteryRecharge);
  testTransition(
    stm, state_machine::State::kBatteryRecharge, state_machine::State::kBatteryRecharge);
  testTransition(
    stm, state_machine::State::kCapacitorDischarge, state_machine::State::kCapacitorDischarge);
  testTransition(
    stm, state_machine::State::kCapacitorDischarge, state_machine::State::kCapacitorDischarge);
  testTransition(stm, state_machine::State::kSafe, state_machine::State::kSafe);
  testTransition(stm, state_machine::State::kSafe, state_machine::State::kSafe);
}

TEST(StateMachine, failureBrakeFromAccelerating)
{
  std::unique_ptr stm = std::make_unique<state_machine::StateMachine>();
  testTransition(stm, state_machine::State::kFailureBrake, state_machine::State::kIdle);
  testTransition(stm, state_machine::State::kCalibrate, state_machine::State::kCalibrate);
  testTransition(stm, state_machine::State::kPrecharge, state_machine::State::kPrecharge);
  testTransition(
    stm, state_machine::State::kReadyForLevitation, state_machine::State::kReadyForLevitation);
  testTransition(
    stm, state_machine::State::kBeginLevitation, state_machine::State::kBeginLevitation);
  testTransition(stm, state_machine::State::kReady, state_machine::State::kReady);
  testTransition(stm, state_machine::State::kAccelerate, state_machine::State::kAccelerate);
  testTransition(stm, state_machine::State::kFailureBrake, state_machine::State::kFailureBrake);
  testTransition(
    stm, state_machine::State::kCapacitorDischarge, state_machine::State::kCapacitorDischarge);
  testTransition(stm, state_machine::State::kSafe, state_machine::State::kSafe);
}

TEST(StateMachine, failureBrakeFromLIMBrake)
{
  std::unique_ptr stm = std::make_unique<state_machine::StateMachine>();
  testTransition(stm, state_machine::State::kFailureBrake, state_machine::State::kIdle);
  testTransition(stm, state_machine::State::kCalibrate, state_machine::State::kCalibrate);
  testTransition(stm, state_machine::State::kPrecharge, state_machine::State::kPrecharge);
  testTransition(
    stm, state_machine::State::kReadyForLevitation, state_machine::State::kReadyForLevitation);
  testTransition(
    stm, state_machine::State::kBeginLevitation, state_machine::State::kBeginLevitation);
  testTransition(stm, state_machine::State::kReady, state_machine::State::kReady);
  testTransition(stm, state_machine::State::kAccelerate, state_machine::State::kAccelerate);
  testTransition(stm, state_machine::State::kLimBrake, state_machine::State::kLimBrake);
  testTransition(stm, state_machine::State::kFailureBrake, state_machine::State::kFailureBrake);
  testTransition(
    stm, state_machine::State::kCapacitorDischarge, state_machine::State::kCapacitorDischarge);
  testTransition(stm, state_machine::State::kSafe, state_machine::State::kSafe);
}

TEST(StateMachine, failureBrakeFrictionBrake)
{
  std::unique_ptr stm = std::make_unique<state_machine::StateMachine>();
  testTransition(stm, state_machine::State::kFailureBrake, state_machine::State::kIdle);
  testTransition(stm, state_machine::State::kCalibrate, state_machine::State::kCalibrate);
  testTransition(stm, state_machine::State::kPrecharge, state_machine::State::kPrecharge);
  testTransition(
    stm, state_machine::State::kReadyForLevitation, state_machine::State::kReadyForLevitation);
  testTransition(
    stm, state_machine::State::kBeginLevitation, state_machine::State::kBeginLevitation);
  testTransition(stm, state_machine::State::kReady, state_machine::State::kReady);
  testTransition(stm, state_machine::State::kAccelerate, state_machine::State::kAccelerate);
  testTransition(stm, state_machine::State::kLimBrake, state_machine::State::kLimBrake);
  testTransition(stm, state_machine::State::kFrictionBrake, state_machine::State::kFrictionBrake);
  testTransition(stm, state_machine::State::kFailureBrake, state_machine::State::kFailureBrake);
  testTransition(
    stm, state_machine::State::kCapacitorDischarge, state_machine::State::kCapacitorDischarge);
  testTransition(stm, state_machine::State::kSafe, state_machine::State::kSafe);
}

TEST(StateMachine, duplicatedMessagesFailureStates)
{
  std::unique_ptr stm = std::make_unique<state_machine::StateMachine>();
  testTransition(stm, state_machine::State::kFailureBrake, state_machine::State::kIdle);
  testTransition(stm, state_machine::State::kFailureBrake, state_machine::State::kIdle);
  testTransition(stm, state_machine::State::kCalibrate, state_machine::State::kCalibrate);
  testTransition(stm, state_machine::State::kCalibrate, state_machine::State::kCalibrate);
  testTransition(stm, state_machine::State::kPrecharge, state_machine::State::kPrecharge);
  testTransition(stm, state_machine::State::kPrecharge, state_machine::State::kPrecharge);
  testTransition(
    stm, state_machine::State::kReadyForLevitation, state_machine::State::kReadyForLevitation);
  testTransition(
    stm, state_machine::State::kReadyForLevitation, state_machine::State::kReadyForLevitation);
  testTransition(
    stm, state_machine::State::kBeginLevitation, state_machine::State::kBeginLevitation);
  testTransition(
    stm, state_machine::State::kBeginLevitation, state_machine::State::kBeginLevitation);
  testTransition(stm, state_machine::State::kReady, state_machine::State::kReady);
  testTransition(stm, state_machine::State::kReady, state_machine::State::kReady);
  testTransition(stm, state_machine::State::kAccelerate, state_machine::State::kAccelerate);
  testTransition(stm, state_machine::State::kAccelerate, state_machine::State::kAccelerate);
  testTransition(stm, state_machine::State::kLimBrake, state_machine::State::kLimBrake);
  testTransition(stm, state_machine::State::kLimBrake, state_machine::State::kLimBrake);
  testTransition(stm, state_machine::State::kFailureBrake, state_machine::State::kFailureBrake);
  testTransition(stm, state_machine::State::kFailureBrake, state_machine::State::kFailureBrake);
  testTransition(
    stm, state_machine::State::kCapacitorDischarge, state_machine::State::kCapacitorDischarge);
  testTransition(
    stm, state_machine::State::kCapacitorDischarge, state_machine::State::kCapacitorDischarge);
  testTransition(stm, state_machine::State::kSafe, state_machine::State::kSafe);
  testTransition(stm, state_machine::State::kSafe, state_machine::State::kSafe);
}
}  // namespace hyped::test