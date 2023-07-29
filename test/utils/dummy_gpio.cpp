#include <iostream>

#include <gtest/gtest.h>

#include <utils/dummy_gpio.hpp>

namespace hyped::test {

TEST(DummyGpio, construct)
{
  utils::DummyGpio dummy_gpio(
    [](const std::uint8_t) { return std::nullopt; },
    [](const std::uint8_t, const core::DigitalSignal) { return hyped::core::Result::kFailure; });
}

void testRead(utils::DummyGpio &dummy_gpio,
              const std::uint8_t pin,
              const std::string expected_output)
{
  testing::internal::CaptureStdout();
  auto dummy_gpio_reader_opt = dummy_gpio.getReader(pin);
  ASSERT_TRUE(dummy_gpio_reader_opt);
  auto dummy_gpio_reader = *dummy_gpio_reader_opt;
  ASSERT_TRUE(dummy_gpio_reader);
  const auto value = dummy_gpio_reader->read();
  ASSERT_EQ(value, core::DigitalSignal::kHigh);
  ASSERT_EQ(testing::internal::GetCapturedStdout(), expected_output);
}

void testWrite(utils::DummyGpio &dummy_gpio,
               const std::uint8_t pin,
               const core::DigitalSignal state,
               const std::string expected_output)
{
  testing::internal::CaptureStdout();
  auto dummy_gpio_writer_opt = dummy_gpio.getWriter(pin);
  ASSERT_TRUE(dummy_gpio_writer_opt);
  auto dummy_gpio_writer = *dummy_gpio_writer_opt;
  ASSERT_TRUE(dummy_gpio_writer);
  const auto result = dummy_gpio_writer->write(state);
  ASSERT_EQ(result, hyped::core::Result::kSuccess);
  ASSERT_EQ(testing::internal::GetCapturedStdout(), expected_output);
}

TEST(DummyGpio, printToStdout)
{
  // dummy GPIO that prints to stdout whenever the interface is accessed
  utils::DummyGpio dummy_gpio(
    [](const std::uint8_t pin) {
      std::cout << "read from " << static_cast<int>(pin) << std::endl;
      return core::DigitalSignal::kHigh;
    },
    [](const std::uint8_t pin, const core::DigitalSignal state) {
      std::cout << "wrote ";
      switch (state) {
        case core::DigitalSignal::kLow:
          std::cout << "low";
          break;
        case core::DigitalSignal::kHigh:
          std::cout << "high";
      }
      std::cout << " to " << static_cast<int>(pin) << std::endl;
      return hyped::core::Result::kSuccess;
    });
  testRead(dummy_gpio, 4, "read from 4\n");
  testRead(dummy_gpio, 42, "read from 42\n");
  testRead(dummy_gpio, 255, "read from 255\n");
  testRead(dummy_gpio, 0, "read from 0\n");
  testWrite(dummy_gpio, 4, core::DigitalSignal::kHigh, "wrote high to 4\n");
  testWrite(dummy_gpio, 42, core::DigitalSignal::kLow, "wrote low to 42\n");
  testWrite(dummy_gpio, 255, core::DigitalSignal::kHigh, "wrote high to 255\n");
  testWrite(dummy_gpio, 0, core::DigitalSignal::kLow, "wrote low to 0\n");
}

}  // namespace hyped::test
