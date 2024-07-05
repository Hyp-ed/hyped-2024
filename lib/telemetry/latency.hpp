#pragma once

#include <core/mqtt.hpp>
#include <core/types.hpp>

namespace hyped::telemetry {

class Latency {
 public:
  Latency(std::shared_ptr<core::IMqtt> mqtt);
  void respond();
  void run();
  static core::Result startNode(const std::string &mqtt_ip, const std::uint32_t mqtt_port);

  const std::shared_ptr<core::IMqtt> mqtt_;
};

}  // namespace hyped::telemetry
