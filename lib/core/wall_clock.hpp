#pragma once

#include "time.hpp"

#include <cstdint>

namespace hyped::core {

/**
 * @brief Wrapper around std::chrono::high_resultion_clock in order to work with
 * the ITimeSource interface.
 */
class WallClock : public ITimeSource {
 public:
  WallClock();
  virtual TimePoint now() const;
};

}  // namespace hyped::core
