#include <iostream>

#include <gtest/gtest.h>

#include <utils/dummy_uart.hpp>

namespace hyped::test {

TEST(DummyUart, construction)
{
  utils::DummyUart dummy_uart;
}

}  // namespace hyped::test
