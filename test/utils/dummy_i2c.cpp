#include <iostream>

#include <gtest/gtest.h>

#include <utils/dummy_i2c.hpp>

namespace hyped::test {

TEST(DummyI2c, construction)
{
  utils::DummyI2c dummy_i2c;
}

}  // namespace hyped::test
