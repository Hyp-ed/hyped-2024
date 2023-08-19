#include <gtest/gtest.h>

#include <core/logger.hpp>
#include <core/scheduler.hpp>
#include <utils/manual_time.hpp>

namespace hyped::test {

TEST(Scheduler, noTasks)
{
  utils::ManualTime manual_time;
  core::Logger logger("test", core::LogLevel::kDebug, manual_time);
  core::Scheduler scheduler(logger, manual_time);
  ASSERT_EQ(scheduler.run(), core::Result::kSuccess);
}

TEST(Scheduler, immediateTask)
{
  utils::ManualTime manual_time;
  core::Logger logger("test", core::LogLevel::kDebug, manual_time);
  core::Scheduler scheduler(logger, manual_time);
  bool task_called = false;
  scheduler.addTask(0, [&task_called]() {
    task_called = true;
    return core::Result::kSuccess;
  });
  ASSERT_EQ(scheduler.run(), core::Result::kSuccess);
  ASSERT_TRUE(task_called);
}

TEST(Scheduler, delayedTask)
{
  utils::ManualTime manual_time;
  core::Logger logger("test", core::LogLevel::kDebug, manual_time);
  core::Scheduler scheduler(logger, manual_time);
  bool task_called = false;
  scheduler.addTask(1000, [&task_called]() {
    task_called = true;
    return core::Result::kSuccess;
  });
  ASSERT_EQ(scheduler.run(), core::Result::kSuccess);
  ASSERT_FALSE(task_called);
  manual_time.set_time(std::chrono::system_clock::from_time_t(1));
  ASSERT_EQ(scheduler.run(), core::Result::kSuccess);
  ASSERT_TRUE(task_called);
}

TEST(Scheduler, failingTask)
{
  utils::ManualTime manual_time;
  core::Logger logger("test", core::LogLevel::kDebug, manual_time);
  core::Scheduler scheduler(logger, manual_time);
  bool task_called = false;
  scheduler.addTask(0, [&task_called]() {
    task_called = true;
    return core::Result::kFailure;
  });
  ASSERT_EQ(scheduler.run(), core::Result::kFailure);
  ASSERT_TRUE(task_called);
}

TEST(Scheduler, multipleTasksCorrectOrder)
{
  utils::ManualTime manual_time;
  core::Logger logger("test", core::LogLevel::kDebug, manual_time);
  core::Scheduler scheduler(logger, manual_time);
  bool task1_called = false;
  bool task2_called = false;
  scheduler.addTask(0, [&task1_called]() {
    task1_called = true;
    return core::Result::kSuccess;
  });
  scheduler.addTask(0, [&task2_called]() {
    task2_called = true;
    return core::Result::kSuccess;
  });
  manual_time.set_time(std::chrono::system_clock::from_time_t(1));
  ASSERT_EQ(scheduler.run(), core::Result::kSuccess);
  ASSERT_TRUE(task1_called);
  ASSERT_EQ(scheduler.run(), core::Result::kSuccess);
  ASSERT_TRUE(task2_called);
}
}  // namespace hyped::test