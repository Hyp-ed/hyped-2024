#include "CanCommands.hpp"

namespace hyped::debug {
std::optional<std::vector<std::unique_ptr<ICommand>>> CanCommands::initialise(
  std::vector<io::ICan> buses)
{
  return std::nullopt;
}

}  // namespace hyped::debug