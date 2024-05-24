#pragma once

#include "state.hpp"
#include "transition_table.hpp"

#include <unordered_map>

#include "core/time.hpp"
#include <boost/unordered_map.hpp>
#include <core/mqtt.hpp>
#include <core/types.hpp>
#include <toml++/toml.hpp>

namespace hyped::state_machine {

class StateMachine {
 public:
  StateMachine(std::shared_ptr<core::IMqtt> mqtt,
               core::ITimeSource &time,
               TransitionTable transition_table);
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

  core::ITimeSource &time_;
  State current_state_;
  const std::shared_ptr<core::IMqtt> mqtt_;
  TransitionTable transition_to_state_;
};

}  // namespace hyped::state_machine
