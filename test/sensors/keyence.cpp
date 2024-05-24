#include <gtest/gtest.h>

#include <core/logger.hpp>
#include <sensors/i2c_mux.hpp>
#include <sensors/keyence.hpp>
#include <utils/dummy_gpio.hpp>
#include <utils/dummy_logger.hpp>

namespace hyped::test {

TEST(Keyence, count)
{
  static bool output_low                 = true;
  std::shared_ptr<utils::DummyGpio> gpio = std::make_shared<utils::DummyGpio>(
    [](const std::uint8_t) {
      if (output_low) {
        output_low = false;
        return core::DigitalSignal::kLow;
      }
      output_low = true;
      return core::DigitalSignal::kHigh;
    },
    [](const std::uint8_t, core::DigitalSignal) { return core::Result::kSuccess; });
  utils::DummyLogger logger;
  auto optional_keyence = sensors::Keyence::create(logger, gpio, 0);
  ASSERT_NE(optional_keyence, std::nullopt);
  auto keyence = optional_keyence.value();
  for (int i = 1; i < 10; i++) {
    keyence->updateStripeCount();
    ASSERT_EQ(keyence->getStripeCount(), i / 2);
  }
}

}  // namespace hyped::test
