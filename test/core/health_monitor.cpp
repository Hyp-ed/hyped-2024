#include <pthread.h>

#include <chrono>
#include <thread>

#include <gtest/gtest.h>

#include "core/mqtt.hpp"
#include "core/mqtt_topics.hpp"
#include "core/time.hpp"
#include "state_machine/state.hpp"
#include <core/health_monitor.hpp>
#include <toml++/toml.hpp>
#include <utils/dummy_logger.hpp>
#include <utils/dummy_mqtt.hpp>
#include <utils/manual_time.hpp>

void *run(void *arg)
{
  auto *monitor = reinterpret_cast<hyped::core::HealthMonitor *>(arg);
  monitor->run();
  return nullptr;
}

pthread_t startThread(hyped::core::HealthMonitor &monitor)
{
  pthread_t thread;
  pthread_create(&thread, nullptr, run, &monitor);
  return thread;
}

void killThread(pthread_t thread)
{
  pthread_cancel(thread);
  pthread_join(thread, nullptr);
}

TEST(HealthMonitor, NoCheckins)
{
  auto config = toml::table{{"hostnames", toml::array{"host1", "host2"}},
                            {
                              "host1",
                              toml::table{{"nodes", toml::array{"node1", "node2"}}},
                            },
                            {
                              "host2",
                              toml::table{{"nodes", toml::array{"node3", "node4"}}},
                            }};
  hyped::utils::DummyLogger logger;
  hyped::utils::ManualTime time;
  auto mqtt             = std::make_shared<hyped::utils::DummyMqtt>();
  auto optional_monitor = hyped::core::HealthMonitor::create(logger, time, mqtt, config);
  ASSERT_TRUE(optional_monitor);
  auto monitor     = *optional_monitor;
  pthread_t thread = startThread(monitor);
  std::this_thread::sleep_for(std::chrono::milliseconds(10));
  time.set_seconds_since_epoch(hyped::core::kCheckinTimeout.count() + 1);
  killThread(thread);
  const auto sent_messages = mqtt->getSentMessages();
  ASSERT_EQ(sent_messages.size(), 1);
  const auto &message = sent_messages[0];
  ASSERT_EQ(message.topic, hyped::core::MqttTopic::kStateRequest);
  ASSERT_TRUE(message.payload->IsObject());
  const auto payload = message.payload->GetObject();
  ASSERT_TRUE(payload.HasMember("transition"));
  const std::string requested_transition = payload["transition"].GetString();
  ASSERT_EQ(requested_transition,
            hyped::state_machine::state_to_string.at(hyped::state_machine::State::kFailure));
}

void addCheckins(const std::shared_ptr<hyped::utils::DummyMqtt> &mqtt,
                 const hyped::core::ITimeSource &time)
{
  std::array<std::string, 4> nodes = {{"host1.node1", "host1.node2", "host2.node3", "host2.node4"}};
  for (const std::string &node : nodes) {
    auto payload = std::shared_ptr<rapidjson::Document>();
    payload->SetString(node.c_str(), payload->GetAllocator());
    hyped::core::MqttMessage message(hyped::core::MqttTopic::kNodeCheckin,
                                     {
                                       .timestamp = time.now(),
                                       .priority  = hyped::core::MqttMessagePriority::kCritical,
                                     },
                                     payload);
    mqtt->addMessageToReceive(message);
  }
}

TEST(HealthMonitor, AllUp)
{
  auto config = toml::table{{"hostnames", toml::array{"host1", "host2"}},
                            {
                              "host1",
                              toml::table{{"nodes", toml::array{"node1", "node2"}}},
                            },
                            {
                              "host2",
                              toml::table{{"nodes", toml::array{"node3", "node4"}}},
                            }};
  hyped::utils::DummyLogger logger;
  hyped::utils::ManualTime time;
  auto mqtt = std::make_shared<hyped::utils::DummyMqtt>();
  addCheckins(mqtt, time);
  auto optional_monitor = hyped::core::HealthMonitor::create(logger, time, mqtt, config);
  ASSERT_TRUE(optional_monitor);
  auto monitor     = *optional_monitor;
  pthread_t thread = startThread(monitor);
  std::this_thread::sleep_for(std::chrono::milliseconds(10));
  time.set_seconds_since_epoch(hyped::core::kCheckinTimeout.count() + 1);
  killThread(thread);
  const auto sent_messages = mqtt->getSentMessages();
  ASSERT_EQ(sent_messages.size(), 1);
  const auto &message = sent_messages[0];
  ASSERT_EQ(message.topic, hyped::core::MqttTopic::kStateRequest);
  ASSERT_TRUE(message.payload->IsObject());
  const auto payload = message.payload->GetObject();
  ASSERT_TRUE(payload.HasMember("transition"));
  const std::string requested_transition = payload["transition"].GetString();
  ASSERT_EQ(requested_transition,
            hyped::state_machine::state_to_string.at(hyped::state_machine::State::kFailure));
}
