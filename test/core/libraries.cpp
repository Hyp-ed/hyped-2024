#include <gtest/gtest.h>

#include <Eigen/Dense>
#include <boost/algorithm/string.hpp>

TEST(Libraries, Eigen)
{
  Eigen::Matrix3d m;
  m << 1, 2, 3, 4, 5, 6, 7, 8, 9;
  Eigen::Vector3d v(1, 2, 3);
  Eigen::Vector3d result = m * v;
  ASSERT_EQ(result(0), 14);
  ASSERT_EQ(result(1), 32);
  ASSERT_EQ(result(2), 50);
}

TEST(Libraries, Boost)
{
  std::string s = "Hello World";
  boost::to_upper(s);
  ASSERT_EQ(s, "HELLO WORLD");
}
