#pragma once

#include <cstdint>

namespace hyped::sensors {

constexpr std::uint8_t kAdcMuxAddress = 0x1D;

enum class AdcMuxChannel : std::uint8_t {
  kAdcMuxChannel1 = 0x20,
  kAdcMuxChannel2 = 0x21,
  kAdcMuxChannel3 = 0x22,
  kAdcMuxChannel4 = 0x23,
  kAdcMuxChannel5 = 0x24,
  kAdcMuxChannel6 = 0x25,
  kAdcMuxChannel7 = 0x26,
  kAdcMuxChannel0 = 0x27,
};

}  // namespace hyped::sensors
