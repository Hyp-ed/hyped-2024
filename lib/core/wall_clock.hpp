#pragma once

#include "time.hpp"

namespace hyped::core {

/**
 * @brief Wrapper around std::chrono::high_resultion_clock in order to work with
 * the ITimeSource interface.
 */
class WallClock : public ITimeSource {
 public:
  WallClock() = default;

  TimePoint now() const override;
};

}  // namespace hyped::core
