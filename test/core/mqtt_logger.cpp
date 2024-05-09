#include <gtest/gtest.h>

#include "core/mqtt.hpp"
#include "utils/dummy_logger.hpp"
#include <core/mqtt_logger.hpp>
#include <utils/dummy_mqtt.hpp>
#include <utils/manual_time.hpp>

namespace hyped::test {

TEST(MqttLogger, sendMqttMessage)
{
  utils::ManualTime manual_time;
  auto mqtt = std::make_shared<utils::DummyMqtt>();
  utils::DummyLogger logger;
  core::MqttLogger mqtt_logger("test", core::LogLevel::kDebug, manual_time, logger, mqtt);
  mqtt_logger.log(core::LogLevel::kDebug, "test");
  const auto sent_messages = mqtt->getSentMessages();
  ASSERT_EQ(sent_messages.size(), 1);
  const auto &message = sent_messages.at(0);
  ASSERT_EQ(message.topic, core::MqttTopic::kLog);
  ASSERT_EQ(message.header.priority, core::MqttMessagePriority::kCritical);
  ASSERT_EQ(message.header.timestamp, 0);
  const auto &payload = message.payload->GetObject();
  ASSERT_EQ(payload.MemberCount(), 1);
  ASSERT_TRUE(payload.HasMember("log"));
  const auto &log = payload.FindMember("log")->value;
  ASSERT_TRUE(log.IsString());
  ASSERT_STREQ(log.GetString(), "test");
}

TEST(MqttLogger, correctTime)
{
  utils::ManualTime manual_time;
  manual_time.set_time(std::chrono::system_clock::from_time_t(3600));
  auto mqtt = std::make_shared<utils::DummyMqtt>();
  utils::DummyLogger logger;
  core::MqttLogger mqtt_logger("test", core::LogLevel::kDebug, manual_time, logger, mqtt);
  mqtt_logger.log(core::LogLevel::kDebug, "test");
  const auto sent_messages = mqtt->getSentMessages();
  ASSERT_EQ(sent_messages.size(), 1);
  const auto &message = sent_messages.at(0);
  ASSERT_EQ(message.topic, core::MqttTopic::kLog);
  ASSERT_EQ(message.header.priority, core::MqttMessagePriority::kCritical);
  ASSERT_EQ(message.header.timestamp,
            std::chrono::system_clock::from_time_t(3600).time_since_epoch().count());
  const auto &payload = message.payload->GetObject();
  ASSERT_EQ(payload.MemberCount(), 1);
  ASSERT_TRUE(payload.HasMember("log"));
  const auto &log = payload.FindMember("log")->value;
  ASSERT_TRUE(log.IsString());
  ASSERT_STREQ(log.GetString(), "test");
}

TEST(MqttLogger, sendFormattedMessage)
{
  utils::ManualTime manual_time;
  auto mqtt = std::make_shared<utils::DummyMqtt>();
  utils::DummyLogger logger;
  core::MqttLogger mqtt_logger("test", core::LogLevel::kDebug, manual_time, logger, mqtt);
  mqtt_logger.log(core::LogLevel::kDebug, "test %d", 42);
  const auto sent_messages = mqtt->getSentMessages();
  ASSERT_EQ(sent_messages.size(), 1);
  const auto &message = sent_messages.at(0);
  ASSERT_EQ(message.topic, core::MqttTopic::kLog);
  ASSERT_EQ(message.header.priority, core::MqttMessagePriority::kCritical);
  ASSERT_EQ(message.header.timestamp, 0);
  const auto &payload = message.payload->GetObject();
  ASSERT_EQ(payload.MemberCount(), 1);
  ASSERT_TRUE(payload.HasMember("log"));
  const auto &log = payload.FindMember("log")->value;
  ASSERT_TRUE(log.IsString());
  ASSERT_STREQ(log.GetString(), "test 42");
}

}  // namespace hyped::test
