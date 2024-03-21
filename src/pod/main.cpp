#include <iostream>

#include <core/host_information.hpp>
#include <core/logger.hpp>
#include <core/mqtt.hpp>
#include <core/timer.hpp>
#include <core/types.hpp>
#include <core/wall_clock.hpp>
#include <io/hardware_gpio.hpp>
#include <state_machine/state_machine.hpp>
#include <state_machine/transition_table.hpp>
#include <toml++/toml.hpp>

int main(int argc, char **argv)
{
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " <config_file>" << std::endl;
    return 1;
  }
  const std::string config_file = argv[1];

  // get the hostname and IP
  const hyped::core::HostInformation host;
  const auto optional_hostname = host.getName();
  if (!optional_hostname) {
    std::cerr << "Failed to get hostname" << std::endl;
    return 1;
  }
  const std::string hostname = *optional_hostname;
  const auto optional_ip     = host.getIp();
  if (!optional_ip) {
    std::cerr << "Failed to get IP" << std::endl;
    return 1;
  }
  const std::string ip = *optional_ip;

  // parse the config file
  const toml::table config = toml::parse_file(config_file);
  const auto optional_ip   = config["hostnames"][hostname].value<std::string>();
  if (!optional_ip) {
    std::cerr << "Failed to get IP from config file" << std::endl;
    return 1;
  }
  const std::string ip_from_config = *optional_ip;
  if (ip != ip_from_config) {
    std::cerr << "IP from config file does not match actual IP" << std::endl;
    return 1;
  }
  const auto optional_nodes = config[hostname]["nodes"].value<std::vector<std::string>>();
  if (!optional_nodes) {
    std::cerr << "No nodes for this host in config file" << std::endl;
    return 1;
  }
  const auto optional_mqtt_ip = config["mqtt"]["ip"].value<std::string>();
  if (!optional_mqtt_ip) {
    std::cerr << "Failed to get MQTT IP from config file" << std::endl;
    return 1;
  }
  const std::string mqtt_ip     = *optional_mqtt_ip;
  const auto optional_mqtt_port = config["mqtt"]["port"].value<uint32_t>();
  if (!optional_mqtt_port) {
    std::cerr << "Failed to get MQTT port from config file" << std::endl;
    return 1;
  }
  const uint32_t mqtt_port             = *optional_mqtt_port;
  const std::vector<std::string> nodes = *optional_nodes;
  for (const auto &node : nodes) {
    const auto result = fork();
    if (result == -1) {
      std::cerr << "Failed to fork" << std::endl;
      return 1;
    }
    if (result == 0) {
      if (node == "state_machine") {
        const auto optional_state_machine = config["state_machine"].value<toml::table>();
        if (!optional_state_machine) {
          std::cerr << "Failed to get state machine config" << std::endl;
          return 1;
        }
        const toml::table state_machine_config = *optional_state_machine;
        state_machine(state_machine_config, mqtt_ip, mqtt_port);
      } else {
        std::cerr << "Unknown node: " << node << std::endl;
        return 1;
      }
      return 0;
    }
  }
  return 0;
}

// TODO: move to state_machine.cpp
void state_machine(toml::table config, const std::string &mqtt_ip, const uint32_t mqtt_port)
{
  hyped::core::WallClock wall_clock;
  hyped::core::Logger logger("STATE_MACHINE", hyped::core::LogLevel::kDebug, wall_clock);
  const auto optional_mqtt = hyped::core::Mqtt::create(logger, "state_machine", mqtt_ip, mqtt_port);
  if (!optional_mqtt) {
    logger.log(hyped::core::LogLevel::kFatal, "Failed to create MQTT client");
    return;
  }
  auto mqtt                           = *optional_mqtt;
  const auto optional_transition_list = config["state_machine"].value<std::string>();
  if (!optional_transition_list) {
    logger.log(hyped::core::LogLevel::kFatal, "Failed to get transition list from config");
    return;
  }
  const std::string transition_list = *optional_transition_list;
  if (transition_list == "full_run") {
    const hyped::state_machine::TransitionTable transition_table
      = hyped::state_machine::transition_to_state_dynamic;
    hyped::state_machine::StateMachine state_machine(mqtt, transition_table);
  } else if (transition_list == "static_run") {
    const hyped::state_machine::TransitionTable transition_table
      = hyped::state_machine::transition_to_state_static;
    hyped::state_machine::StateMachine state_machine(mqtt, transition_table);
  } else {
    logger.log(hyped::core::LogLevel::kFatal, "Unknown transition list: %s", transition_list);
    return;
  }
  // TODO: send state machine online message
  state_machine.run();
}
