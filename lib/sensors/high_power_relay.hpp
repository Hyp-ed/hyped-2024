#pragma once

#include <cstdint>
#include <memory>
#include <optional>

#include <core/logger.hpp>
#include <io/gpio.hpp>

namespace hyped::sensors {
class HpRelay {
  public:
  static std::optional<HpRelay> create(core::ILogger &logger,
                                      std::shared_ptr<io::IGpio> gpio,
                                      const std::uint8_t new_pin);
  
                                      
};
} // namespace::sensors