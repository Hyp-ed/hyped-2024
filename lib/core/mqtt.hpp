#pragma once

#include "logger.hpp"
#include "mqtt_topics.hpp"
#include "types.hpp"

#include <optional>
#include <queue>
#include <vector>

#include <mqtt/client.h>
#include <rapidjson/document.h>

namespace hyped::core {

enum MqttMessagePriority {
  kCritical = 0,
  kNormal   = 1,
  kLow      = 2,
};

enum MqttMessageQos {
  kAtMostOnce  = 0,
  kAtLeastOnce = 1,
  kExactlyOnce = 2,
};

struct MqttMessage {
  struct Header {
    std::uint64_t timestamp;
    MqttMessagePriority priority;
  };
  Header header;
  rapidjson::Document &payload;
  bool operator<(const MqttMessage &other) const
  {
    if (header.priority == other.header.priority) {
      return header.timestamp > other.header.timestamp;
    }
    return header.priority < other.header.priority;
  }
};
class IMqtt {
 public:
  virtual core::Result publish(const core::MqttTopic &topic,
                               const MqttMessage &message,
                               const MqttMessageQos qos)
    = 0;
  virtual core::Result subscribe(const core::MqttTopic topic) = 0;
  // virtual core::Result consume()                                                           = 0;
  // virtual MqttMessage getMessage()                                                         = 0;
};

class Mqtt : public IMqtt {
 public:
  static std::optional<std::shared_ptr<Mqtt>> create(ILogger &logger,
                                                     const std::string &id,
                                                     const std::string &host,
                                                     const std::uint16_t port);
  Mqtt(ILogger &logger, std::unique_ptr<mqtt::client> client);
  ~Mqtt();
  core::Result publish(const MqttTopic &topic,
                       const MqttMessage &message,
                       const MqttMessageQos qos);
  core::Result subscribe(const core::MqttTopic topic);
  // core::Result consume();

 private:
  ILogger &logger_;
  std::unique_ptr<mqtt::client> client_;
  // std::priority_queue<MqttMessage> incoming_message_queue_;
  bool message_available_;
};

// class MqttCallback : public mqtt::callback {
//  public:
//   MqttCallback(ILogger &logger, std::shared_ptr<Mqtt> mqtt);
//   void connection_lost(const std::string &cause) override;
//   void delivery_complete(mqtt::delivery_token_ptr token) override;
//   void message_arrived(mqtt::const_message_ptr msg) override;

//  private:
//   ILogger &logger_;
//   std::shared_ptr<Mqtt> mqtt_;
// };

}  // namespace hyped::core