#include "mqtt.hpp"

#include <rapidjson/writer.h>

#include "rapidjson/stringbuffer.h"

namespace hyped::core {

std::optional<std::shared_ptr<Mqtt>> Mqtt::create(ILogger &logger,
                                                  const std::string &id,
                                                  const std::string &host,
                                                  const std::uint16_t port)
{
  std::string address = "tcp://" + host + ":" + std::to_string(port);
  mqtt::connect_options connection_options;
  connection_options.set_keep_alive_interval(1);
  connection_options.set_clean_session(true);
  auto mqtt_client = std::make_unique<mqtt::client>(address, id);
  auto cb          = std::make_shared<MqttCallback>(logger);
  mqtt_client->set_callback(*cb);
  mqtt_client->connect(connection_options);
  if (!mqtt_client->is_connected()) {
    logger.log(core::LogLevel::kFatal, "Failed to connect to MQTT broker");
    return std::nullopt;
  }
  logger.log(core::LogLevel::kInfo, "Successfully created MQTT client and connected to broker");
  return std::make_shared<Mqtt>(logger, std::move(mqtt_client), std::move(cb));
}

Mqtt::Mqtt(ILogger &logger, std::unique_ptr<mqtt::client> client, mqtt::callback_ptr cb)
    : logger_(logger),
      client_(std::move(client)),
      cb(std::move(cb))
{
}

Mqtt::~Mqtt()
{
  client_->disconnect();
}

void Mqtt::publish(const MqttMessage &message, const MqttMessageQos qos)
{
  auto constructed_message = messageToMessagePtr(message);
  logger_.log(core::LogLevel::kDebug,
              "Publishing message %s",
              constructed_message->get_payload_str().c_str());
  constructed_message->set_qos(qos);
  client_->publish(constructed_message);
}

core::Result Mqtt::subscribe(const core::MqttTopic topic)
{
  const auto topic_string = mqtt_topic_to_string.find(topic);
  if (topic_string == mqtt_topic_to_string.end()) {
    logger_.log(core::LogLevel::kFatal, "Attempted to subscribe to nonexistent MQTT topic");
    return core::Result::kFailure;
  }
  const auto result       = client_->subscribe(topic_string->second);
  const auto reason_codes = result.get_reason_codes();
  for (mqtt::ReasonCode code : reason_codes) {
    if (code != mqtt::ReasonCode::SUCCESS && code != mqtt::ReasonCode::GRANTED_QOS_0
        && code != mqtt::ReasonCode::GRANTED_QOS_1 && code != mqtt::ReasonCode::GRANTED_QOS_2) {
      logger_.log(core::LogLevel::kFatal,
                  "Failed to subscribe to MQTT topic [%s] with error code: %i",
                  topic_string->second.c_str(),
                  code);
      return core::Result::kFailure;
    }
  }
  logger_.log(core::LogLevel::kInfo, "Subscribed to topic: %s", topic_string->second.c_str());
  return core::Result::kSuccess;
}

core::Result Mqtt::consume()
{
  for (std::uint32_t i = 0; i < 100; i++) {
    mqtt::const_message_ptr received_msg;
    auto received = client_->try_consume_message(&received_msg);
    if (!received) {
      logger_.log(core::LogLevel::kDebug, "Consumed %i message(s)", i);
      return core::Result::kSuccess;
    }
    auto parsed_message = messagePtrToMessage(received_msg);
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
  rapidjson::Document header;
  header.SetObject();
  header.AddMember("priority", message.header.priority, header.GetAllocator());
  header.AddMember("timestamp", message.header.timestamp, header.GetAllocator());
  payload.AddMember("header", header, payload.GetAllocator());
  rapidjson::Document payload_json;
  payload_json.CopyFrom(*message.payload, payload_json.GetAllocator());
  payload.AddMember("payload", payload_json, payload.GetAllocator());
  rapidjson::StringBuffer buffer;
  auto writer = rapidjson::Writer<rapidjson::StringBuffer>(buffer);
  payload.Accept(writer);
  const auto topic_string = mqtt_topic_to_string.find(message.topic);
  if (topic_string == mqtt_topic_to_string.end()) {
    logger_.log(core::LogLevel::kFatal, "Attempted to publish to nonexistent MQTT topic");
    return nullptr;
  }
  return mqtt::make_message(topic_string->second, buffer.GetString());
}

std::optional<MqttMessage> Mqtt::messagePtrToMessage(std::shared_ptr<const mqtt::message> message)
{
  const auto topic      = message->get_topic();
  const auto mqtt_topic = mqtt_string_to_topic.find(topic);
  if (mqtt_topic == mqtt_string_to_topic.end()) {
    logger_.log(
      core::LogLevel::kFatal, "Received message on nonexistent MQTT topic: %s", topic.c_str());
    return std::nullopt;
  }
  const auto message_contents = message->get_payload().c_str();
  rapidjson::Document message_contents_json;
  message_contents_json.Parse(message_contents);
  if (message_contents_json.HasParseError()) {
    logger_.log(core::LogLevel::kFatal, "Failed to parse MQTT message: parse error");
    return std::nullopt;
  }
  if (!message_contents_json.HasMember("header")) {
    logger_.log(core::LogLevel::kFatal, "Failed to parse MQTT message: missing header");
    return std::nullopt;
  }
  const auto header = message_contents_json["header"].GetObject();  // error here
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
  return MqttMessage{mqtt_topic->second, mqtt_header, mqtt_payload};
}

MqttCallback::MqttCallback(ILogger &logger) : logger_(logger)
{
}

void MqttCallback::connection_lost(const std::string &cause)
{
  if (!cause.empty()) {
    logger_.log(core::LogLevel::kFatal, "Connection lost with MQTT broker: %s", cause.c_str());
  }
}

void MqttCallback::delivery_complete(mqtt::delivery_token_ptr token)
{
  logger_.log(core::LogLevel::kDebug, "Message delivery complete");
}

void MqttCallback::message_arrived(mqtt::const_message_ptr msg)
{
  logger_.log(core::LogLevel::kDebug, "Message arrived");
}
}  // namespace hyped::core
