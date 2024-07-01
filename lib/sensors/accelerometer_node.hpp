#pragma once

#include "accelerometer.hpp"

#include "core/mqtt.hpp"
#include "core/types.hpp"
#include <toml++/toml.hpp>

namespace hyped::sensors {

class AccelerometerNode {
 public:
  static core::Result startNode(toml::v3::node_view<const toml::v3::node> config,
                                const std::string &mqtt_ip,
                                const std::uint32_t mqtt_port);
  AccelerometerNode(core::Logger &logger,
                    core::ITimeSource &time,
                    std::shared_ptr<core::Mqtt> mqtt,
                    std::shared_ptr<Accelerometer> accelerometer);
  void run();

 private:
  core::Logger logger_;
  core::ITimeSource &time_;
  std::shared_ptr<core::Mqtt> mqtt_;
  std::shared_ptr<Accelerometer> accelerometer_;
};

}  // namespace hyped::sensors
