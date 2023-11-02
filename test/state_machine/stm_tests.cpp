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
  testTransition(stm, state_machine::Message::kFailureBrake, state_machine::State::kIdle);
  testTransition(stm, state_machine::Message::kCalibrating, state_machine::State::kCalibrating);
  testTransition(stm, state_machine::Message::kPrecharge, state_machine::State::kPrecharge);
  testTransition(stm, state_machine::Message::kReadyForLevitation, state_machine::State::kReadyForLevitation);
  testTransition(stm, state_machine::Message::kBeginLevitation, state_machine::State::kBeginLevitation);
  testTransition(stm, state_machine::Message::kReady, state_machine::State::kReady);
  testTransition(stm, state_machine::Message::kAccelerate, state_machine::State::kAccelerate);
  testTransition(stm, state_machine::Message::kLIMBrake, state_machine::State::kLIMBrake);
  testTransition(stm, state_machine::Message::kFrictionBrake, state_machine::State::kFrictionBrake);
  testTransition(stm, state_machine::Message::kStopLevitation, state_machine::State::kStopLevitation);
  testTransition(stm, state_machine::Message::kStopped, state_machine::State::kStopped);
  testTransition(stm, state_machine::Message::kBatteryRecharge, state_machine::State::kBatteryRecharge);
  testTransition(stm, state_machine::Message::kCapacitorDischarge, state_machine::State::kCapacitorDischarge);
  testTransition(stm, state_machine::Message::kSafe, state_machine::State::kSafe);
}

TEST(StateMachine, cleanRunDuplicatedMessages)
{
  std::unique_ptr stm = std::make_unique<state_machine::StateMachine>();
  testTransition(stm, state_machine::Message::kFailureBrake, state_machine::State::kIdle);
  testTransition(stm, state_machine::Message::kFailureBrake, state_machine::State::kIdle);
  testTransition(stm, state_machine::Message::kCalibrating, state_machine::State::kCalibrating);
  testTransition(stm, state_machine::Message::kCalibrating, state_machine::State::kCalibrating);
  testTransition(stm, state_machine::Message::kPrecharge, state_machine::State::kPrecharge);
  testTransition(stm, state_machine::Message::kPrecharge, state_machine::State::kPrecharge);
  testTransition(stm, state_machine::Message::kReadyForLevitation, state_machine::State::kReadyForLevitation);
  testTransition(stm, state_machine::Message::kReadyForLevitation, state_machine::State::kReadyForLevitation);
  testTransition(stm, state_machine::Message::kBeginLevitation, state_machine::State::kBeginLevitation);
  testTransition(stm, state_machine::Message::kBeginLevitation, state_machine::State::kBeginLevitation);
  testTransition(stm, state_machine::Message::kReady, state_machine::State::kReady);
  testTransition(stm, state_machine::Message::kReady, state_machine::State::kReady);
  testTransition(stm, state_machine::Message::kAccelerate, state_machine::State::kAccelerate);
  testTransition(stm, state_machine::Message::kAccelerate, state_machine::State::kAccelerate);
  testTransition(stm, state_machine::Message::kLIMBrake, state_machine::State::kLIMBrake);
  testTransition(stm, state_machine::Message::kLIMBrake, state_machine::State::kLIMBrake);
  testTransition(stm, state_machine::Message::kFrictionBrake, state_machine::State::kFrictionBrake);
  testTransition(stm, state_machine::Message::kFrictionBrake, state_machine::State::kFrictionBrake);
  testTransition(stm, state_machine::Message::kStopLevitation, state_machine::State::kStopLevitation);
  testTransition(stm, state_machine::Message::kStopLevitation, state_machine::State::kStopLevitation);
  testTransition(stm, state_machine::Message::kStopped, state_machine::State::kStopped);
  testTransition(stm, state_machine::Message::kStopped, state_machine::State::kStopped);
  testTransition(stm, state_machine::Message::kBatteryRecharge, state_machine::State::kBatteryRecharge);
  testTransition(stm, state_machine::Message::kBatteryRecharge, state_machine::State::kBatteryRecharge);
  testTransition(stm, state_machine::Message::kCapacitorDischarge, state_machine::State::kCapacitorDischarge);
  testTransition(stm, state_machine::Message::kCapacitorDischarge, state_machine::State::kCapacitorDischarge);
  testTransition(stm, state_machine::Message::kSafe, state_machine::State::kSafe);
  testTransition(stm, state_machine::Message::kSafe, state_machine::State::kSafe);
}

TEST(StateMachine, failureBrakeFromAccelerating)
{
  std::unique_ptr stm = std::make_unique<state_machine::StateMachine>();
  testTransition(stm, state_machine::Message::kFailureBrake, state_machine::State::kIdle);
  testTransition(stm, state_machine::Message::kCalibrating, state_machine::State::kCalibrating);
  testTransition(stm, state_machine::Message::kPrecharge, state_machine::State::kPrecharge);
  testTransition(stm, state_machine::Message::kReadyForLevitation, state_machine::State::kReadyForLevitation);
  testTransition(stm, state_machine::Message::kBeginLevitation, state_machine::State::kBeginLevitation);
  testTransition(stm, state_machine::Message::kReady, state_machine::State::kReady);
  testTransition(stm, state_machine::Message::kAccelerate, state_machine::State::kAccelerate);
  testTransition(stm, state_machine::Message::kFailureBrake, state_machine::State::kFailureBrake);
  testTransition(stm, state_machine::Message::kCapacitorDischarge, state_machine::State::kCapacitorDischarge);
  testTransition(stm, state_machine::Message::kSafe, state_machine::State::kSafe);
}

TEST(StateMachine, failureBrakeFromLIMBrake)
{
  std::unique_ptr stm = std::make_unique<state_machine::StateMachine>();
  testTransition(stm, state_machine::Message::kFailureBrake, state_machine::State::kIdle);
  testTransition(stm, state_machine::Message::kCalibrating, state_machine::State::kCalibrating);
  testTransition(stm, state_machine::Message::kPrecharge, state_machine::State::kPrecharge);
  testTransition(stm, state_machine::Message::kReadyForLevitation, state_machine::State::kReadyForLevitation);
  testTransition(stm, state_machine::Message::kBeginLevitation, state_machine::State::kBeginLevitation);
  testTransition(stm, state_machine::Message::kReady, state_machine::State::kReady);
  testTransition(stm, state_machine::Message::kAccelerate, state_machine::State::kAccelerate);
  testTransition(stm, state_machine::Message::kLIMBrake, state_machine::State::kLIMBrake);
  testTransition(stm, state_machine::Message::kFailureBrake, state_machine::State::kFailureBrake);
  testTransition(stm, state_machine::Message::kCapacitorDischarge, state_machine::State::kCapacitorDischarge);
  testTransition(stm, state_machine::Message::kSafe, state_machine::State::kSafe);
}

TEST(StateMachine, failureBrakeFrictionBrake)
{
  std::unique_ptr stm = std::make_unique<state_machine::StateMachine>();
  testTransition(stm, state_machine::Message::kFailureBrake, state_machine::State::kIdle);
  testTransition(stm, state_machine::Message::kCalibrating, state_machine::State::kCalibrating);
  testTransition(stm, state_machine::Message::kPrecharge, state_machine::State::kPrecharge);
  testTransition(stm, state_machine::Message::kReadyForLevitation, state_machine::State::kReadyForLevitation);
  testTransition(stm, state_machine::Message::kBeginLevitation, state_machine::State::kBeginLevitation);
  testTransition(stm, state_machine::Message::kReady, state_machine::State::kReady);
  testTransition(stm, state_machine::Message::kAccelerate, state_machine::State::kAccelerate);
  testTransition(stm, state_machine::Message::kLIMBrake, state_machine::State::kLIMBrake);
  testTransition(stm, state_machine::Message::kFrictionBrake, state_machine::State::kFrictionBrake);
  testTransition(stm, state_machine::Message::kFailureBrake, state_machine::State::kFailureBrake);
  testTransition(stm, state_machine::Message::kCapacitorDischarge, state_machine::State::kCapacitorDischarge);
  testTransition(stm, state_machine::Message::kSafe, state_machine::State::kSafe);
}


TEST(StateMachine, duplicatedMessagesFailureStates)
{
  std::unique_ptr stm = std::make_unique<state_machine::StateMachine>();
  testTransition(stm, state_machine::Message::kFailureBrake, state_machine::State::kIdle);
  testTransition(stm, state_machine::Message::kFailureBrake, state_machine::State::kIdle);
  testTransition(stm, state_machine::Message::kCalibrating, state_machine::State::kCalibrating);
  testTransition(stm, state_machine::Message::kCalibrating, state_machine::State::kCalibrating);
  testTransition(stm, state_machine::Message::kPrecharge, state_machine::State::kPrecharge);
  testTransition(stm, state_machine::Message::kPrecharge, state_machine::State::kPrecharge);
  testTransition(stm, state_machine::Message::kReadyForLevitation, state_machine::State::kReadyForLevitation);
  testTransition(stm, state_machine::Message::kReadyForLevitation, state_machine::State::kReadyForLevitation);
  testTransition(stm, state_machine::Message::kBeginLevitation, state_machine::State::kBeginLevitation);
  testTransition(stm, state_machine::Message::kBeginLevitation, state_machine::State::kBeginLevitation);
  testTransition(stm, state_machine::Message::kReady, state_machine::State::kReady);
  testTransition(stm, state_machine::Message::kReady, state_machine::State::kReady);
  testTransition(stm, state_machine::Message::kAccelerate, state_machine::State::kAccelerate);
  testTransition(stm, state_machine::Message::kAccelerate, state_machine::State::kAccelerate);
  testTransition(stm, state_machine::Message::kLIMBrake, state_machine::State::kLIMBrake);
  testTransition(stm, state_machine::Message::kLIMBrake, state_machine::State::kLIMBrake);
  testTransition(stm, state_machine::Message::kFailureBrake, state_machine::State::kFailureBrake);
  testTransition(stm, state_machine::Message::kFailureBrake, state_machine::State::kFailureBrake);
  testTransition(stm, state_machine::Message::kCapacitorDischarge, state_machine::State::kCapacitorDischarge);
  testTransition(stm, state_machine::Message::kCapacitorDischarge, state_machine::State::kCapacitorDischarge);
  testTransition(stm, state_machine::Message::kSafe, state_machine::State::kSafe);
  testTransition(stm, state_machine::Message::kSafe, state_machine::State::kSafe);
}
}

// namespace hyped::test