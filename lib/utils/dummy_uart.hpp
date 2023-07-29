#pragma once

#include <io/uart.hpp>

namespace hyped::utils {

/**
 * A basic implementation of Uart that does not require any hardware. This allow us to
 *
 * 1. demonstrate how to implement the IUart interface, and
 * 2. allow for quick test implementations.
 */
class DummyUart : public io::IUart {
 public:
  DummyUart()  = default;
  ~DummyUart() = default;

  virtual core::Result sendBytes(const char *tx, const std::uint8_t length);
  virtual core::Result readBytes(unsigned char *rx, const std::uint8_t length);
};

}  // namespace hyped::utils