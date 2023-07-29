#include "wall_clock.hpp"

namespace hyped::core {

WallClock::WallClock()
{
}

TimePoint WallClock::now() const
{
  return std::chrono::system_clock::now();
}

}  // namespace hyped::core
