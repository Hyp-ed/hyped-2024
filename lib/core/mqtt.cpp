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
      client_(std::move(client)),
      messages_in_queue(0)
{
}

Mqtt::~Mqtt()
{
  client_->disconnect();
}

void Mqtt::publish(const MqttMessage &message, const MqttMessageQos qos)
{
  auto constructed_message = messageToMessagePtr(message);
  logger_.log(
    core::LogLevel::kDebug, "Publishing message %s", constructed_message->get_payload_str());
  constructed_message->set_qos(qos);
  client_->publish(constructed_message);
}

core::Result Mqtt::subscribe(const core::MqttTopic topic)
{
  const auto topic_string = mqtt_topic_to_string.find(topic);
  if (topic_string == mqtt_topic_to_string.end()) {
    logger_.log(core::LogLevel::kFatal, "Attempted to subscribe to nonexistent MQTT topic");
    return core::Result::kSuccess;
  }
  const auto result       = client_->subscribe(topic_string->second);
  const auto reason_codes = result.get_reason_codes();
  for (mqtt::ReasonCode code : reason_codes) {
    if (code != mqtt::ReasonCode::SUCCESS && code != mqtt::ReasonCode::GRANTED_QOS_0
        && code != mqtt::ReasonCode::GRANTED_QOS_1 && code != mqtt::ReasonCode::GRANTED_QOS_2) {
      logger_.log(
        core::LogLevel::kFatal, "Failed to subscribe to MQTT topic with error code %i", code);
      return core::Result::kFailure;
    }
  }
  logger_.log(core::LogLevel::kInfo, "Subscribed to topic");
  return core::Result::kSuccess;
}

core::Result Mqtt::consume()
{
  for (std::uint32_t i; i < 100; i++) {
    mqtt::const_message_ptr *message;
    const bool message_received = client_->try_consume_message(message);
    if (!message_received) {
      logger_.log(core::LogLevel::kDebug, "Consumed %i MQTT messages", i);
      break;
    }
    const auto parsed_message = messagePtrToMessage(message);
    if (!parsed_message) {
      logger_.log(core::LogLevel::kFatal, "Failed to parse MQTT message");
      return core::Result::kFailure;
    }
    incoming_message_queue_.push(*parsed_message);
  }
  return core::Result::kSuccess;
}

std::optional<MqttMessage> Mqtt::getMessage()
{
  if (incoming_message_queue_.empty()) { return std::nullopt; }
  const auto message = incoming_message_queue_.top();
  incoming_message_queue_.pop();
  return message;
}

mqtt::message_ptr Mqtt::messageToMessagePtr(const MqttMessage &message)
{
  rapidjson::Document payload;
  payload.SetObject();
  rapidjson::Value header;
  header.SetObject();
  header.AddMember("priority", message.header.priority, payload.GetAllocator());
  header.AddMember("timestamp", message.header.timestamp, payload.GetAllocator());
  payload.AddMember("header", header, payload.GetAllocator());
  payload.CopyFrom(*message.payload, payload.GetAllocator());
  rapidjson::StringBuffer buffer;
  auto writer = rapidjson::Writer<rapidjson::StringBuffer>(buffer);
  payload.Accept(writer);
  return mqtt::make_message(mqtt_topic_to_string[message.topic], buffer.GetString());
}

std::optional<MqttMessage> Mqtt::messagePtrToMessage(mqtt::const_message_ptr *message)
{
  const auto topic            = message->get()->get_topic();
  const auto mqtt_topic       = mqtt_string_to_topic[topic];
  const auto message_contents = message->get()->to_string().c_str();
  rapidjson::Document message_contents_json;
  message_contents_json.Parse(message_contents);
  if (!message_contents_json.HasMember("header")) {
    logger_.log(core::LogLevel::kFatal, "Failed to parse MQTT message: missing header");
    return std::nullopt;
  }
  const auto header = message_contents_json["header"].GetObject();
  if (!header.HasMember("timestamp")) {
    logger_.log(core::LogLevel::kFatal, "Failed to parse MQTT message: missing timestamp");
  }
  if (!header.HasMember("priority")) {
    logger_.log(core::LogLevel::kFatal, "Failed to parse MQTT message: missing priority");
  }
  const auto timestamp = header["timestamp"].GetUint64();
  const auto priority  = header["priority"].GetUint();
  if (!message_contents_json.HasMember("payload")) {
    logger_.log(core::LogLevel::kFatal, "Failed to parse MQTT message: missing payload");
  }
  if (priority > static_cast<std::uint8_t>(MqttMessagePriority::kLow)) {
    logger_.log(core::LogLevel::kFatal, "Failed to parse MQTT message: invalid priority");
    return std::nullopt;
  }
  MqttMessagePriority mqtt_priority = static_cast<MqttMessagePriority>(priority);
  std::shared_ptr mqtt_payload      = std::make_shared<rapidjson::Document>();
  mqtt_payload->CopyFrom(message_contents_json["payload"], mqtt_payload->GetAllocator());
  MqttMessage::Header mqtt_header{timestamp, mqtt_priority};
  return MqttMessage{mqtt_topic, mqtt_header, mqtt_payload};
}

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