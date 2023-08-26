#include "logger.hpp"
#include "mqtt_topics.hpp"
#include "types.hpp"

#include <mqtt/client.h>

namespace hyped::core {
class IMqtt {
 public:
  virtual core::Result connect()                                                           = 0;
  virtual core::Result publish(const core::MqttTopic &topic, const mqtt::message &message) = 0;
  virtual core::Result subscribe(const core::MqttTopic &topic)                             = 0;
};

class Mqtt : IMqtt {
 public:
  Mqtt(ILogger &log, const char *id, const char *host, int port);
  core::Result connect() override;
  core::Result publish(const core::MqttTopic &topic, const mqtt::message &message) override;
  core::Result subscribe(const core::MqttTopic &topic) override;

 private:
  ILogger &log_;
  mqtt::client client_;
  const char *id_;
  const char *host_;
  int port_;
};

}  // namespace hyped::core