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
  ASSERT_EQ(sent_messages.at(0).topic, core::MqttTopic::kTest);
  ASSERT_EQ(sent_messages.at(0).header.priority, core::MqttMessagePriority::kCritical);
  ASSERT_EQ(sent_messages.at(0).header.timestamp, 0);
  ASSERT_EQ(sent_messages.at(0).payload->GetObject().MemberCount(), 1);
  ASSERT_TRUE(sent_messages.at(0).payload->GetObject().HasMember("log"));
  ASSERT_EQ(sent_messages.at(0).payload->GetObject().FindMember("log")->value.GetString(), "test");
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
  ASSERT_EQ(sent_messages.at(0).topic, core::MqttTopic::kTest);
  ASSERT_EQ(sent_messages.at(0).header.priority, core::MqttMessagePriority::kCritical);
  ASSERT_EQ(sent_messages.at(0).header.timestamp, 0);
  ASSERT_EQ(sent_messages.at(0).payload->GetObject().MemberCount(), 1);
  ASSERT_TRUE(sent_messages.at(0).payload->GetObject().HasMember("log"));
  ASSERT_EQ(sent_messages.at(0).payload->GetObject().FindMember("log")->value.GetString(),
            "test 42");
}

}  // namespace hyped::test
