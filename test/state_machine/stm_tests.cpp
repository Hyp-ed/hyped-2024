#include <gtest/gtest.h>

#include "utils/manual_time.hpp"
#include <core/logger.hpp>
#include <core/mqtt.hpp>
#include <core/wall_clock.hpp>
#include <state_machine/state_machine.hpp>
#include <state_machine/transition_table.hpp>
#include <utils/dummy_mqtt.hpp>

namespace hyped::test {

void testTransition(const std::shared_ptr<state_machine::StateMachine> &stm,
                    state_machine::State transition_state,
                    state_machine::State expected_state)
{
  stm->handleTransition(transition_state);
  ASSERT_EQ(stm->getCurrentState(), expected_state);
}

std::shared_ptr<utils::DummyMqtt> getMockMqtt()
{
  return std::make_shared<utils::DummyMqtt>();
}

TEST(StateMachine, cleanRunDynamic)
{
  utils::ManualTime manual_time;
  auto stm = std::make_shared<state_machine::StateMachine>(
    getMockMqtt(), manual_time, state_machine::transition_to_state_dynamic);
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

TEST(StateMachine, cleanRunDuplicatedMessagesDynamic)
{
  utils::ManualTime manual_time;
  auto stm = std::make_shared<state_machine::StateMachine>(
    getMockMqtt(), manual_time, state_machine::transition_to_state_dynamic);
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

TEST(StateMachine, failureBrakeFromAcceleratingDynamic)
{
  utils::ManualTime manual_time;
  auto stm = std::make_shared<state_machine::StateMachine>(
    getMockMqtt(), manual_time, state_machine::transition_to_state_dynamic);
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

TEST(StateMachine, failureBrakeFromLIMBrakeDynamic)
{
  utils::ManualTime manual_time;
  auto stm = std::make_shared<state_machine::StateMachine>(
    getMockMqtt(), manual_time, state_machine::transition_to_state_dynamic);
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

TEST(StateMachine, failureBrakeFrictionBrakeDynamic)
{
  utils::ManualTime manual_time;
  auto stm = std::make_shared<state_machine::StateMachine>(
    getMockMqtt(), manual_time, state_machine::transition_to_state_dynamic);
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

TEST(StateMachine, duplicatedMessagesFailureStatesDynamic)
{
  utils::ManualTime manual_time;
  auto stm = std::make_shared<state_machine::StateMachine>(
    getMockMqtt(), manual_time, state_machine::transition_to_state_dynamic);
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

TEST(StateMachine, duplicatedMessageAfterStateChangeDynamic)
{
  utils::ManualTime manual_time;
  auto stm = std::make_shared<state_machine::StateMachine>(
    getMockMqtt(), manual_time, state_machine::transition_to_state_dynamic);
  testTransition(stm, state_machine::State::kCalibrate, state_machine::State::kCalibrate);
  testTransition(stm, state_machine::State::kPrecharge, state_machine::State::kPrecharge);
  testTransition(stm, state_machine::State::kCalibrate, state_machine::State::kPrecharge);
  testTransition(
    stm, state_machine::State::kReadyForLevitation, state_machine::State::kReadyForLevitation);
}

TEST(StateMachine, cleanRunStatic)
{
  utils::ManualTime manual_time;
  auto stm = std::make_shared<state_machine::StateMachine>(
    getMockMqtt(), manual_time, state_machine::transition_to_state_static);
  testTransition(stm, state_machine::State::kFailureBrake, state_machine::State::kIdle);
  testTransition(stm, state_machine::State::kCalibrate, state_machine::State::kCalibrate);
  testTransition(stm, state_machine::State::kPrecharge, state_machine::State::kPrecharge);
  testTransition(
    stm, state_machine::State::kReadyForLevitation, state_machine::State::kReadyForLevitation);
  testTransition(
    stm, state_machine::State::kBeginLevitation, state_machine::State::kBeginLevitation);
  testTransition(stm, state_machine::State::kLevitating, state_machine::State::kLevitating);
  testTransition(stm, state_machine::State::kFrictionBrake, state_machine::State::kFrictionBrake);
  testTransition(stm, state_machine::State::kStopLevitation, state_machine::State::kStopLevitation);
  testTransition(stm, state_machine::State::kStopped, state_machine::State::kStopped);
  testTransition(
    stm, state_machine::State::kCapacitorDischarge, state_machine::State::kCapacitorDischarge);
  testTransition(stm, state_machine::State::kSafe, state_machine::State::kSafe);
}

TEST(StateMachine, cleanRunDuplicatedMessagesStatic)
{
  utils::ManualTime manual_time;
  auto stm = std::make_shared<state_machine::StateMachine>(
    getMockMqtt(), manual_time, state_machine::transition_to_state_static);
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
  testTransition(stm, state_machine::State::kLevitating, state_machine::State::kLevitating);
  testTransition(stm, state_machine::State::kLevitating, state_machine::State::kLevitating);
  testTransition(stm, state_machine::State::kFrictionBrake, state_machine::State::kFrictionBrake);
  testTransition(stm, state_machine::State::kFrictionBrake, state_machine::State::kFrictionBrake);
  testTransition(stm, state_machine::State::kStopLevitation, state_machine::State::kStopLevitation);
  testTransition(stm, state_machine::State::kStopLevitation, state_machine::State::kStopLevitation);
  testTransition(stm, state_machine::State::kStopped, state_machine::State::kStopped);
  testTransition(stm, state_machine::State::kStopped, state_machine::State::kStopped);
  testTransition(
    stm, state_machine::State::kCapacitorDischarge, state_machine::State::kCapacitorDischarge);
  testTransition(
    stm, state_machine::State::kCapacitorDischarge, state_machine::State::kCapacitorDischarge);
  testTransition(stm, state_machine::State::kSafe, state_machine::State::kSafe);
  testTransition(stm, state_machine::State::kSafe, state_machine::State::kSafe);
}

TEST(StateMachine, failureBrakeFromLevitatingStatic)
{
  utils::ManualTime manual_time;
  auto stm = std::make_shared<state_machine::StateMachine>(
    getMockMqtt(), manual_time, state_machine::transition_to_state_static);
  testTransition(stm, state_machine::State::kFailureBrake, state_machine::State::kIdle);
  testTransition(stm, state_machine::State::kCalibrate, state_machine::State::kCalibrate);
  testTransition(stm, state_machine::State::kPrecharge, state_machine::State::kPrecharge);
  testTransition(
    stm, state_machine::State::kReadyForLevitation, state_machine::State::kReadyForLevitation);
  testTransition(
    stm, state_machine::State::kBeginLevitation, state_machine::State::kBeginLevitation);
  testTransition(stm, state_machine::State::kLevitating, state_machine::State::kLevitating);
  testTransition(stm, state_machine::State::kFailureBrake, state_machine::State::kFailureBrake);
  testTransition(
    stm, state_machine::State::kCapacitorDischarge, state_machine::State::kCapacitorDischarge);
  testTransition(stm, state_machine::State::kSafe, state_machine::State::kSafe);
}

TEST(StateMachine, failurePriorToLevitationStatic)
{
  utils::ManualTime manual_time;
  auto stm = std::make_shared<state_machine::StateMachine>(
    getMockMqtt(), manual_time, state_machine::transition_to_state_static);
  testTransition(stm, state_machine::State::kFailureBrake, state_machine::State::kIdle);
  testTransition(stm, state_machine::State::kCalibrate, state_machine::State::kCalibrate);
  testTransition(stm, state_machine::State::kPrecharge, state_machine::State::kPrecharge);
  testTransition(stm, state_machine::State::kFailure, state_machine::State::kFailure);
  testTransition(
    stm, state_machine::State::kCapacitorDischarge, state_machine::State::kCapacitorDischarge);
  testTransition(stm, state_machine::State::kSafe, state_machine::State::kSafe);
}

TEST(StateMachine, duplicatedMessagesFailureStatesStatic)
{
  utils::ManualTime manual_time;
  auto stm = std::make_shared<state_machine::StateMachine>(
    getMockMqtt(), manual_time, state_machine::transition_to_state_static);
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
  testTransition(stm, state_machine::State::kFailureBrake, state_machine::State::kFailureBrake);
  testTransition(stm, state_machine::State::kFailureBrake, state_machine::State::kFailureBrake);
  testTransition(
    stm, state_machine::State::kCapacitorDischarge, state_machine::State::kCapacitorDischarge);
  testTransition(
    stm, state_machine::State::kCapacitorDischarge, state_machine::State::kCapacitorDischarge);
  testTransition(stm, state_machine::State::kSafe, state_machine::State::kSafe);
  testTransition(stm, state_machine::State::kSafe, state_machine::State::kSafe);
}

TEST(StateMachine, duplicatedMessageAfterStateChangeStatic)
{
  utils::ManualTime manual_time;
  auto stm = std::make_shared<state_machine::StateMachine>(
    getMockMqtt(), manual_time, state_machine::transition_to_state_static);
  testTransition(stm, state_machine::State::kCalibrate, state_machine::State::kCalibrate);
  testTransition(stm, state_machine::State::kPrecharge, state_machine::State::kPrecharge);
  testTransition(stm, state_machine::State::kCalibrate, state_machine::State::kPrecharge);
  testTransition(
    stm, state_machine::State::kReadyForLevitation, state_machine::State::kReadyForLevitation);
}

}  // namespace hyped::test
