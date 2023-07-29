#include <iostream>

#include <gtest/gtest.h>

#include <utils/dummy_adc.hpp>

namespace hyped::test {

TEST(DummyAdc, construction)
{
  utils::DummyAdc dummy_adc;
}

}  // namespace hyped::test
