#include "health_monitor.hpp"
#include "logger.hpp"
#include "time.hpp"
#include "types.hpp"

#include <utility>

namespace hyped::core {

std::optional<HealthMonitor> HealthMonitor::create(ILogger &logger,
                                                   ITimeSource &time,
                                                   std::shared_ptr<core::IMqtt> mqtt,
                                                   toml::table &config)
{
  const auto *const optional_hostnames = config["hostnames"].as_array();
  if (optional_hostnames == nullptr) {
    logger.log(core::LogLevel::kFatal, "No hostnames specified in config");
    return std::nullopt;
  }
  const auto hostnames = *optional_hostnames;
  std::vector<std::string> nodes;
  // Get all nodes running across all hosts
  for (const auto &hostname : hostnames) {
    const auto optional_hostname = hostname.value<std::string>();
    if (!optional_hostname) {
      logger.log(core::LogLevel::kFatal, "Invalid hostname in config");
      return std::nullopt;
    }
    const std::string &hostname_str  = *optional_hostname;
    const auto *const optional_nodes = config[hostname_str]["nodes"].as_array();
    if (optional_nodes == nullptr) {
      logger.log(core::LogLevel::kFatal, "No nodes for host %s", hostname_str.c_str());
      return std::nullopt;
    }
    const auto nodes_for_host = *optional_nodes;
    for (const auto &node : nodes_for_host) {
      const auto optional_node = node.value<std::string>();
      if (!optional_node) {
        logger.log(core::LogLevel::kFatal, "Invalid node in config");
        return std::nullopt;
      }
      const auto node_str = hostname_str + "." + *optional_node;
      nodes.push_back(node_str);
    }
  }
  return HealthMonitor(logger, time, std::move(mqtt), nodes);
}

HealthMonitor::HealthMonitor(ILogger &logger,
                             ITimeSource &time,
                             std::shared_ptr<core::IMqtt> mqtt,
                             std::vector<std::string> &nodes)
    : logger_(logger),
      time_(time),
      mqtt_(std::move(mqtt))
{
  for (const auto &node : nodes) {
    checkins_[node] = std::nullopt;
  }
}

void HealthMonitor::run()
{
  {
    const auto result = startup();
    if (result == core::Result::kFailure) {
      logger_.log(core::LogLevel::kFatal, "Failed to start up");
      publishTransitionRequest(state_machine::State::kFailure);
      return;
    }
    publishTransitionRequest(state_machine::State::kReady);
  }
  while (true) {
    const auto result = processBatch();
    if (result == core::Result::kFailure) {
      logger_.log(core::LogLevel::kFatal, "Failed to process batch");
      publishTransitionRequest(state_machine::State::kFailure);
      return;
    }
    const auto current_time = time_.now();
    for (auto &[name, checkin] : checkins_) {
      if (current_time - *checkin > kCheckinTimeout) {
        logger_.log(core::LogLevel::kFatal, "System %s has not checked in", name.c_str());
        publishTransitionRequest(state_machine::State::kFailure);
        return;
      }
    }
  }
}

core::Result HealthMonitor::startup()
{
  const auto start_time = time_.now();
  while (time_.now() - start_time < kStartupTimeout) {
    const auto result = processBatch();
    if (result == core::Result::kFailure) {
      logger_.log(core::LogLevel::kFatal, "Failed to process batch");
      return core::Result::kFailure;
    }
    bool all_nodes_checked_in = true;
    for (auto &[_, checkin] : checkins_) {
      if (!checkin) {
        all_nodes_checked_in = false;
        break;
      }
    }
    if (all_nodes_checked_in) {
      logger_.log(core::LogLevel::kInfo,
                  "All nodes checked in in %d seconds",
                  (time_.now() - start_time).count());
      publishTransitionRequest(state_machine::State::kCalibrate);
      return core::Result::kSuccess;
    }
  }
  std::string missing_nodes;
  for (auto &[name, checkin] : checkins_) {
    if (!checkin) { missing_nodes += name + ", "; }
  }
  logger_.log(core::LogLevel::kFatal, "%s failed to check in", missing_nodes.c_str());
  return core::Result::kFailure;
}

core::Result HealthMonitor::processBatch()
{
  const auto consume_result = mqtt_->consume();
  if (consume_result == core::Result::kFailure) {
    logger_.log(core::LogLevel::kFatal, "Invalid MQTT message received");
    return core::Result::kFailure;
  }
  for (auto i = 0; i < 200; i++) {
    mqtt::const_message_ptr received_msg;
    auto optional_message = mqtt_->getMessage();
    if (!optional_message) { break; }
    const auto message          = *optional_message;
    const auto payload          = message.payload;
    const std::string node_name = payload->GetString();
    if (checkins_.find(node_name) == checkins_.end()) {
      logger_.log(core::LogLevel::kFatal, "Invalid node %s", node_name.c_str());
      return core::Result::kFailure;
    }
    checkins_.emplace(node_name, time_.now());
  }
  return core::Result::kSuccess;
}

void HealthMonitor::publishTransitionRequest(state_machine::State state)
{
  auto message_payload = std::make_shared<rapidjson::Document>();
  message_payload->SetObject();
  const std::string &state_str = state_machine::state_to_string.at(state);
  rapidjson::Value requested_state(state_str.c_str(), message_payload->GetAllocator());
  message_payload->AddMember("transition", requested_state, message_payload->GetAllocator());
  const core::MqttMessage::Header header{time_.now(), core::MqttMessagePriority::kCritical};
  const core::MqttMessage message{core::MqttTopic::kStateRequest, header, message_payload};
  mqtt_->publish(message, kExactlyOnce);
}

}  // namespace hyped::core
