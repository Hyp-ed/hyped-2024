#pragma once

#include <cstdio>

#include <core/types.hpp>

namespace hyped::io {

class IUart {
 public:
  /**
   * @brief  Sends a byte array over the UART bus.
   * @param  tx  Pointer to the byte array to be sent.
   * @param  length  Length of the byte array.
   */
  virtual core::Result sendBytes(const char *tx, const std::uint8_t length) = 0;

  /**
   * @brief Receives a byte array over the UART bus.
   * @param rx Pointer to the byte array to be received.
   * @param length Length of the byte array.
   */
  virtual core::Result readBytes(unsigned char *rx, const std::uint8_t length) = 0;
};

}  // namespace hyped::io