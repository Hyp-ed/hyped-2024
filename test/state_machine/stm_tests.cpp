#include <gtest/gtest.h>

#include <core/logger.hpp>
#include <core/mqtt.hpp>
#include <core/wall_clock.hpp>
#include <state_machine/state_machine.hpp>

namespace hyped::test {

void testTransition(std::unique_ptr<state_machine::StateMachine> &stm,
                    state_machine::Message transition_message,
                    state_machine::State expected_state)
{
  stm->handleMessage(transition_message);
  ASSERT_TRUE(stm->getCurrentState() == expected_state);
}

std::shared_ptr<core::Mqtt> getMqtt()
{
  hyped::core::WallClock time;
  hyped::core::Logger logger("MQTT", hyped::core::LogLevel::kDebug, time);
  const std::string id     = "test";
  const std::uint16_t port = 8080;
  const std::string host   = "localhost";
  auto optional_mqtt       = hyped::core::Mqtt::create(logger, id, host, port);
  if (!optional_mqtt) {
    std::cout << "Failed to connect to MQTT broker" << std::endl;
    return nullptr;
  }
  std::cout << "Connected to MQTT broker" << std::endl;
  auto mqtt = *optional_mqtt;
  return mqtt;
}

TEST(StateMachine, cleanRun)
{
  std::unique_ptr stm = std::make_unique<state_machine::StateMachine>(getMqtt());
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
  std::unique_ptr stm = std::make_unique<state_machine::StateMachine>(getMqtt());
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
  std::unique_ptr stm = std::make_unique<state_machine::StateMachine>(getMqtt());
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
  std::unique_ptr stm = std::make_unique<state_machine::StateMachine>(getMqtt());
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
  std::unique_ptr stm = std::make_unique<state_machine::StateMachine>(getMqtt());
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
  std::unique_ptr stm = std::make_unique<state_machine::StateMachine>(getMqtt());
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
  std::unique_ptr stm = std::make_unique<state_machine::StateMachine>(getMqtt());
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
  std::unique_ptr stm = std::make_unique<state_machine::StateMachine>(getMqtt());
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
  std::unique_ptr stm = std::make_unique<state_machine::StateMachine>(getMqtt());
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
  std::unique_ptr stm = std::make_unique<state_machine::StateMachine>(getMqtt());
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