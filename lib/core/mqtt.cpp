#include "mqtt.hpp"

#include <rapidjson/writer.h>

#include "rapidjson/stringbuffer.h"

namespace hyped::core {

std::optional<std::shared_ptr<Mqtt>> Mqtt::create(ILogger &logger,
                                                  const std::string &id,
                                                  const std::string &host,
                                                  const uint16_t port)
{
  std::string address = "tcp://" + host + ":" + std::to_string(port);
  mqtt::connect_options connection_options;
  connection_options.set_keep_alive_interval(1);
  connection_options.set_clean_session(true);
  auto mqtt_client = std::make_unique<mqtt::client>(address, id);
  mqtt_client->connect(connection_options);
  if (!mqtt_client->is_connected()) { return std::nullopt; }
  return std::make_shared<Mqtt>(logger, std::move(mqtt_client));
}

Mqtt::Mqtt(ILogger &logger, std::unique_ptr<mqtt::client> client)
    : logger_(logger),
      client_(std::move(client))
{
}

Mqtt::~Mqtt()
{
  client_->disconnect();
}

core::Result Mqtt::publish(const MqttTopic &topic,
                           const MqttMessage &message,
                           const MqttMessageQos qos)
{
  rapidjson::Document payload;
  payload.SetObject();
  rapidjson::Value header;
  header.SetObject();
  header.AddMember("priority", message.header.priority, payload.GetAllocator());
  header.AddMember("timestamp", message.header.timestamp, payload.GetAllocator());
  payload.AddMember("header", header, payload.GetAllocator());
  rapidjson::Value payload_json;
  payload_json.CopyFrom(message.payload, payload.GetAllocator());
  payload.AddMember("payload", payload_json, payload.GetAllocator());
  rapidjson::StringBuffer buffer;
  auto writer = rapidjson::Writer<rapidjson::StringBuffer>(buffer);
  payload.Accept(writer);
  logger_.log(core::LogLevel::kDebug, "Publishing message %s", buffer.GetString());
  auto pubmsg = mqtt::make_message(mqtt_topic_strings[topic], buffer.GetString());
  pubmsg->set_qos(qos);
  client_->publish(pubmsg);
  return core::Result::kSuccess;
}

// core::Result Mqtt::consume()
// {
//   const auto message = client_->consume_message();
// }

// MqttCallback::MqttCallback(ILogger &logger) : logger_(logger)
// {
// }

// void MqttCallback::connection_lost(const std::string &cause)
// {
//   logger_.log(core::LogLevel::kFatal, "Connection lost with MQTT broker: %s", cause.c_str());
// }

// void MqttCallback::delivery_complete(mqtt::delivery_token_ptr token)
// {
//   logger_.log(core::LogLevel::kDebug, "Message delivery complete");
// }

// void MqttCallback::message_arrived(mqtt::const_message_ptr msg)
// {
//   logger_.log(core::LogLevel::kDebug, "Message arrived");
// }
}  // namespace hyped::core