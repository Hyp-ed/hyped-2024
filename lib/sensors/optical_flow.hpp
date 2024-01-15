#pragma once

#include <cstdint>
#include <cstring>
#include <memory>
#include <optional>

#include <core/logger.hpp>
#include <core/types.hpp>
#include <io/spi.hpp>

namespace hyped::io {

class OpticalFLow {
// start with initialising the sensor (method 1)
// gets the value of x_low and of y_low and return it, so read from 0x03 and 0x05 (method 2)
// this will use read, addr: is register (either x or y), pointer: pointer to head of read buffer ??
// and len: number of bytes to be read, 16 bits?

private:
std::uint8_t getPosition();
//Register addresses for x and y
static constexpr std::uint8_t kXLowReg = 0x03;
static constexpr std::uint8_t kYLowReg = 0x05;
static constexpr std::uint8_t kXLow = 0;
static constexpr std::uint8_t kYLow = 0;
};
}