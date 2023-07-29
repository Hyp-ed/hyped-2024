#include <iostream>

#include <gtest/gtest.h>

#include <utils/dummy_i2c_sensor.hpp>

namespace hyped::test {

TEST(DummyI2cSensor, construction)
{
  utils::DummyI2cSensor dummy_i2c_sensor;
}

}  // namespace hyped::test
