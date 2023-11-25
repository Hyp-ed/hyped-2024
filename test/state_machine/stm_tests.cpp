#include <gtest/gtest.h>

#include <core/logger.hpp>
#include <core/mqtt.hpp>
#include <core/wall_clock.hpp>
#include <state_machine/state_machine.hpp>
#include <utils/dummy_mqtt.hpp>

namespace hyped::test {

void testTransition(std::shared_ptr<state_machine::StateMachine> stm,
                    state_machine::State transition_state,
                    state_machine::State expected_state)
{
  stm->handleTransition(transition_state);
  ASSERT_TRUE(stm->getCurrentState() == expected_state);
}

std::shared_ptr<utils::MockMqtt> getMqtt()
{
  return std::make_shared<utils::MockMqtt>();
}

TEST(StateMachine, cleanRun)
{
  auto stm = std::make_shared<state_machine::StateMachine>(getMqtt());
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
  auto stm = std::make_shared<state_machine::StateMachine>(getMqtt());
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
  auto stm = std::make_shared<state_machine::StateMachine>(getMqtt());
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
  auto stm = std::make_shared<state_machine::StateMachine>(getMqtt());
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
  auto stm = std::make_shared<state_machine::StateMachine>(getMqtt());
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
  auto stm = std::make_shared<state_machine::StateMachine>(getMqtt());
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

TEST(StateMachine, duplicatedMessageAfterStateChange)
{
  auto stm = std::make_shared<state_machine::StateMachine>(getMqtt());
  testTransition(stm, state_machine::State::kCalibrate, state_machine::State::kCalibrate);
  testTransition(stm, state_machine::State::kPrecharge, state_machine::State::kPrecharge);
  testTransition(stm, state_machine::State::kCalibrate, state_machine::State::kPrecharge);
  testTransition(
    stm, state_machine::State::kReadyForLevitation, state_machine::State::kReadyForLevitation);
}
}  // namespace hyped::test