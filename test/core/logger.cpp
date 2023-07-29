#include <gtest/gtest.h>

#include <core/logger.hpp>
#include <utils/manual_time.hpp>

namespace hyped::test {

void testStdoutLog(const core::LogLevel level,
                   const utils::ManualTime &manual_time,
                   const std::string expected_output)
{
  setenv("TZ", "Europe/London", 1);
  tzset();
  testing::internal::CaptureStdout();
  core::Logger logger("test", level, manual_time);
  logger.log(level, "test");
  ASSERT_EQ(testing::internal::GetCapturedStdout(), expected_output);
}

void testStderrLog(const core::LogLevel level,
                   const utils::ManualTime &manual_time,
                   const std::string expected_output)
{
  setenv("TZ", "Europe/London", 1);
  tzset();
  testing::internal::CaptureStderr();
  core::Logger logger("test", level, manual_time);
  logger.log(level, "test");
  ASSERT_EQ(testing::internal::GetCapturedStderr(), expected_output);
}

TEST(Logger, Stdout)
{
  utils::ManualTime manual_time;
  testStdoutLog(core::LogLevel::kDebug, manual_time, "01:00:00.000 DEBUG[test] test\n");
  testStdoutLog(core::LogLevel::kInfo, manual_time, "01:00:00.000 INFO[test] test\n");
  testStdoutLog(core::LogLevel::kFatal, manual_time, "");
}

TEST(Logger, Stderr)
{
  utils::ManualTime manual_time;
  testStderrLog(core::LogLevel::kDebug, manual_time, "");
  testStderrLog(core::LogLevel::kInfo, manual_time, "");
  testStderrLog(core::LogLevel::kFatal, manual_time, "01:00:00.000 FATAL[test] test\n");
}

TEST(Logger, VaryingTimes)
{
  utils::ManualTime manual_time;
  manual_time.set_time(std::chrono::system_clock::from_time_t(3600));
  testStdoutLog(core::LogLevel::kDebug, manual_time, "02:00:00.000 DEBUG[test] test\n");
  manual_time.set_time(std::chrono::system_clock::from_time_t(24 * 3600));
  testStdoutLog(core::LogLevel::kDebug, manual_time, "01:00:00.000 DEBUG[test] test\n");
  manual_time.set_time(std::chrono::system_clock::from_time_t(60));
  testStdoutLog(core::LogLevel::kDebug, manual_time, "01:01:00.000 DEBUG[test] test\n");
  manual_time.set_time(std::chrono::system_clock::from_time_t(1));
  testStdoutLog(core::LogLevel::kDebug, manual_time, "01:00:01.000 DEBUG[test] test\n");
}

TEST(Logger, logNTimes)
{
  utils::ManualTime manual_time;
  setenv("TZ", "Europe/London", 1);
  tzset();
  testing::internal::CaptureStdout();
  core::Logger logger("test", core::LogLevel::kDebug, manual_time);
  for (std::size_t i = 0; i < 5; ++i) {
    logNTimes(logger, 3, core::LogLevel::kDebug, "test");
  }
  ASSERT_EQ(testing::internal::GetCapturedStdout(),
            "01:00:00.000 DEBUG[test] test\n01:00:00.000 DEBUG[test] test\n01:00:00.000 "
            "DEBUG[test] test\n");
}

TEST(Logger, logEveryNth)
{
  utils::ManualTime manual_time;
  setenv("TZ", "Europe/London", 1);
  tzset();
  testing::internal::CaptureStdout();
  core::Logger logger("test", core::LogLevel::kDebug, manual_time);
  for (std::size_t i = 0; i < 15; ++i) {
    logEveryNth(logger, 5, core::LogLevel::kDebug, "test");
    manual_time.set_time(std::chrono::system_clock::from_time_t(3600 * i));
  }
  ASSERT_EQ(testing::internal::GetCapturedStdout(),
            "01:00:00.000 DEBUG[test] test\n05:00:00.000 DEBUG[test] test\n10:00:00.000 "
            "DEBUG[test] test\n");
}

}  // namespace hyped::test
