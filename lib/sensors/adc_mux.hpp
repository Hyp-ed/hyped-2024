#pragma once

#include <cstdio>
#include <optional>

#include <core/types.hpp>

namespace hyped::sensors {
    constexpr kAdcMuxAddress = 0x1D;
    
    constexpr kAdcMuxChannel1 = 0x20;
    constexpr kAdcMuxChannel2 = 0x21;
    constexpr kAdcMuxChannel3 = 0x22;
    constexpr kAdcMuxChannel4 = 0x23;
    constexpr kAdcMuxChannel5 = 0x24;
    constexpr kAdcMuxChannel6 = 0x25;
    constexpr kAdcMuxChannel7 = 0x26;
    constexpr kAdcMuxChannel0 = 0x27;
}