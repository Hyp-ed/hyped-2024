#include <gtest/gtest.h>

#include <utils/manual_time.hpp>

namespace hyped::test {

void test_set_time(utils::ManualTime &manual_time, const std::time_t time)
{
  const auto time_point = std::chrono::system_clock::from_time_t(time);
  manual_time.set_time(time_point);
  ASSERT_EQ(manual_time.now(), time_point);
}

TEST(ManualTime, set_time)
{
  utils::ManualTime manual_time;
  test_set_time(manual_time, 0);
  test_set_time(manual_time, 1000);
  test_set_time(manual_time, 100);
  test_set_time(manual_time, 2000);
}

void test_set_seconds_since_epoch(utils::ManualTime &manual_time,
                                  const std::uint64_t seconds_since_epoch)
{
  manual_time.set_seconds_since_epoch(seconds_since_epoch);
  ASSERT_EQ(
    std::chrono::duration_cast<std::chrono::seconds>(manual_time.now().time_since_epoch()).count(),
    seconds_since_epoch);
}

TEST(ManualTime, basic)
{
  utils::ManualTime manual_time;
  test_set_seconds_since_epoch(manual_time, 0);
  test_set_seconds_since_epoch(manual_time, 1000);
  test_set_seconds_since_epoch(manual_time, 100);
  test_set_seconds_since_epoch(manual_time, 2000);
}

}  // namespace hyped::test
