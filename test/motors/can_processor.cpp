#include <iostream>

#include <gtest/gtest.h>

#include <core/logger.hpp>
#include <core/types.hpp>
#include <motors/can_processor.hpp>

namespace hyped::test{
  TEST(CAN_processor, something){
    int alpha = 1;
    ASSERT_TRUE(1 == alpha);
  }
}