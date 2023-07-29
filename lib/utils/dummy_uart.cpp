#include "dummy_uart.hpp"

namespace hyped::utils {

core::Result DummyUart::sendBytes(const char *tx, const std::uint8_t length)
{
  return core::Result::kFailure;
}

core::Result DummyUart::readBytes(unsigned char *rx, const std::uint8_t length)
{
  return core::Result::kFailure;
}

}  // namespace hyped::utils