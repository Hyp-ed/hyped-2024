#include "wall_clock.hpp"

namespace hyped::core {

TimePoint WallClock::now() const
{
  return std::chrono::system_clock::now();
}

}  // namespace hyped::core
