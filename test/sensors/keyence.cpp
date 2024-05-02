#include <gtest/gtest.h>

#include <core/logger.hpp>
#include <sensors/i2c_mux.hpp>
#include <sensors/keyence.hpp>
#include <utils/dummy_gpio.hpp>
#include <utils/dummy_logger.hpp>

namespace hyped::test {

TEST(Keyence, count)
{
  std::shared_ptr<utils::DummyGpio> gpio = std::make_shared<utils::DummyGpio>(
    [](const std::uint8_t) {
      return std::optional<core::DigitalSignal>(core::DigitalSignal::kHigh);
    },
    [](const std::uint8_t, core::DigitalSignal) { return core::Result::kSuccess; });
  utils::DummyLogger logger;
  auto keyence = sensors::Keyence::create(logger, gpio, 0);
  for (int i = 1; i < 10; i++) {
    keyence->updateStripeCount();
    ASSERT_EQ(keyence->getStripeCount(), i);
  }
}

}  // namespace hyped::test
