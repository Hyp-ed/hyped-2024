#include <memory>

#include <gtest/gtest.h>

#include <core/logger.hpp>
#include <sensors/i2c_mux.hpp>
#include <utils/dummy_i2c.hpp>
#include <utils/dummy_i2c_sensor.hpp>
#include <utils/dummy_logger.hpp>

namespace hyped::test {

TEST(I2cMux, construction)
{
  static constexpr std::uint8_t kSize = 8;
  std::array<std::unique_ptr<sensors::II2cMuxSensor<std::uint8_t>>, kSize> sensors;
  for (auto &sensor : sensors) {
    sensor = std::make_unique<utils::DummyI2cSensor>();
  }
  const auto i2c = std::make_shared<utils::DummyI2c>();
  utils::DummyLogger logger;
  sensors::I2cMux<std::uint8_t, kSize> mux(logger, i2c, 0, sensors);
}

}  // namespace hyped::test
