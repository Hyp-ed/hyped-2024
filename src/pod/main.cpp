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
  const auto optional_host_ip = host.getIp();
  if (!optional_host_ip) {
    std::cerr << "Failed to get IP" << std::endl;
    return 1;
  }
  const std::string ip = *optional_host_ip;

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
  const auto optional_nodes = config[hostname]["nodes"].as_array();
  if (!optional_nodes) {
    std::cerr << "No nodes for this host in config file" << std::endl;
    return 1;
  }
  const auto nodes            = *optional_nodes;
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
  const uint32_t mqtt_port = *optional_mqtt_port;
  for (const auto &node : nodes) {
    const auto result = fork();
    if (result == -1) {
      std::cerr << "Failed to fork" << std::endl;
      return 1;
    }
    const auto node_name = node.value_or<std::string>("Invalid node");
    if (result == 0) {
      if (node_name == "state_machine") {
        auto state_machine_config = config["state_machine"];
        hyped::state_machine::StateMachine::startNode(state_machine_config, mqtt_ip, mqtt_port);
      } else {
        std::cerr << "Unknown node: " << node_name << std::endl;
        return 1;
      }
      return 0;
    }
  }
  return 0;
}
