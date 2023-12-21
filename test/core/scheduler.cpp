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
  scheduler.schedule(hyped::core::Duration(0), [&task_called]() {
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
  scheduler.schedule(hyped::core::Duration(1000000), [&task_called]() {
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
  scheduler.schedule(hyped::core::Duration(0), [&task_called]() {
    task_called = true;
    return core::Result::kFailure;
  });
  ASSERT_EQ(scheduler.run(), core::Result::kFailure);
  ASSERT_TRUE(task_called);
}
}  // namespace hyped::test