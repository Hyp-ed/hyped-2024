#include <memory>

#include <gtest/gtest.h>

#include <core/logger.hpp>
#include <sensors/i2c_mux.hpp>
#include <utils/dummy_i2c.hpp>
#include <utils/dummy_logger.hpp>

namespace hyped::test {

TEST(I2cMux, construction)
{
  const auto i2c = std::make_shared<utils::DummyI2c>();
  utils::DummyLogger logger;
  sensors::I2cMux mux(logger, i2c, sensors::kDefaultMuxAddress, 0);
}

}  // namespace hyped::test
