#pragma once

#include "logger.hpp"
#include "mqtt.hpp"
#include "time.hpp"

#include <optional>
#include <string>
#include <unordered_map>

#include <toml++/toml.hpp>

namespace hyped::core {

/**
 * @brief HealthMonitor tracks the status of all nodes running on the pod, and if any fail to check
 * in within a certain period it will send a critical failure message. It also ensures all nodes
 * have started up before the pod leaves the initialisation state.
 */
class HealthMonitor {
 public:
  static std::optional<HealthMonitor> create(ILogger &logger,
                                             ITimeSource &time,
                                             std::shared_ptr<core::IMqtt> mqtt,
                                             toml::table &config);

  HealthMonitor(core::ILogger &logger,
                core::ITimeSource &time,
                std::shared_ptr<core::IMqtt> mqtt,
                std::vector<std::string> &nodes);

  void run();

 private:
  /**
   * @brief Checks if all nodes have checked in, then publishes a message saying we are safe to
   * start
   */
  core::Result startup();
  /**
   * @brief Updates the checkin times for all pending checkin messages
   */
  core::Result processBatch();
  void sendCriticalFailure();

  core::ILogger &logger_;
  core::ITimeSource &time_;
  std::shared_ptr<core::IMqtt> mqtt_;
  std::unordered_map<std::string, std::optional<TimePoint>> checkins_;

  static constexpr Duration kCheckinTimeout = TimePoint::duration(1);
  static constexpr Duration kStartupTimeout = TimePoint::duration(60);
};

}  // namespace hyped::core
