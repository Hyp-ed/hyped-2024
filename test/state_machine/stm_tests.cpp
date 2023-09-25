#include <gtest/gtest.h>

#include <state_machine/state_machine.hpp>

namespace hyped::test {

TEST(StateMachine, initalize){
  std::unique_ptr stm = std::make_unique<state_machine::StateMachine>();
}

}  // namespace hyped::test