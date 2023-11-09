#pragma once
#include "ICommand.hpp"

#include <memory>
#include <vector>

#include <io/can.hpp>

namespace hyped::debug {
class CanCommands {
 public:
  static std::optional<std::vector<std::unique_ptr<ICommand>>> initialise(
    std::vector<io::ICan> buses);
};
}  // namespace hyped::debug
