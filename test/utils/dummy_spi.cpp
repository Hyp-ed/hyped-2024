#include <iostream>

#include <gtest/gtest.h>

#include <utils/dummy_spi.hpp>

namespace hyped::test {

TEST(DummySpi, construction)
{
  utils::DummySpi dummy_spi;
}

}  // namespace hyped::test
