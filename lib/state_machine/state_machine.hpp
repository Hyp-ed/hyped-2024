#pragma once

#include "state.hpp"
#include "transition_table.hpp"

#include <unordered_map>

#include <boost/unordered_map.hpp>
#include <core/mqtt.hpp>
#include <core/types.hpp>
#include <toml++/toml.hpp>

namespace hyped::state_machine {

class StateMachine {
 public:
  StateMachine(std::shared_ptr<core::IMqtt> mqtt, TransitionTable transition_table);
  void run();
  State getCurrentState();
  core::Result handleTransition(const State &state);
  static core::Result startNode(toml::v3::node_view<const toml::v3::node> config,
                                const std::string &mqtt_ip,
                                const std::uint32_t mqtt_port);

 private:
  void publishCurrentState();
  State stringToState(const std::string &state_name);
  std::string stateToString(const State &state);
  void update();
  const std::unordered_map<std::string, State> string_to_state_
    = {{"kIdle", State::kIdle},
       {"kCalibrate", State::kCalibrate},
       {"kPrecharge", State::kPrecharge},
       {"kReadyForLeviation", State::kReadyForLevitation},
       {"kBeginLevitation", State::kBeginLevitation},
       {"kLevitating", State::kLevitating},
       {"kReady", State::kReady},
       {"kAccelerate", State::kAccelerate},
       {"kLimBrake", State::kLimBrake},
       {"kFrictionBrake", State::kFrictionBrake},
       {"kStopLevitation", State::kStopLevitation},
       {"kStopped", State::kStopped},
       {"kBatteryRecharge", State::kBatteryRecharge},
       {"kCapacitorDischarge", State::kCapacitorDischarge},
       {"kFailureBrake", State::kFailureBrake},
       {"kFailure", State::kFailure},
       {"kSafe", State::kSafe}};
  const std::unordered_map<State, std::string> state_to_string_
    = {{State::kIdle, "kIdle"},
       {State::kCalibrate, "kCalibrate"},
       {State::kPrecharge, "kPrecharge"},
       {State::kReadyForLevitation, "kReadyForLevitation"},
       {State::kBeginLevitation, "kBeginLevitation"},
       {State::kLevitating, "kLevitating"},
       {State::kReady, "kReady"},
       {State::kAccelerate, "kAccelerate"},
       {State::kLimBrake, "kLimBrake"},
       {State::kFrictionBrake, "kFrictionBrake"},
       {State::kStopLevitation, "kStopLevitation"},
       {State::kStopped, "kStopped"},
       {State::kBatteryRecharge, "kBatteryRecharge"},
       {State::kCapacitorDischarge, "kCapacitorDischarge"},
       {State::kFailureBrake, "kFailureBrake"},
       {State::kFailure, "kFailure"},
       {State::kSafe, "kSafe"}};

  State current_state_;
  const std::shared_ptr<core::IMqtt> mqtt_;
  TransitionTable transition_to_state_;
};

}  // namespace hyped::state_machine
