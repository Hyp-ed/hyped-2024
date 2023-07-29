#pragma once

#include "uart.hpp"

#include <strings.h>
#include <termios.h>

#include <cstdint>
#include <memory>
#include <optional>

#include <core/logger.hpp>
#include <core/types.hpp>
#ifdef __APPLE__
#define B460800 0010004
#define B500000 0010005
#define B576000 0010006
#define B921600 0010007
#define B1000000 0010010
#define B1152000 0010011
#define B1500000 0010012
#define B2000000 0010013
#define B2500000 0010014
#define B3000000 0010015
#define B3500000 0010016
#endif

namespace hyped::io {

// Uart3 not exposed in BBB headers
enum class UartBus { kUart0 = 0, kUart1 = 1, kUart2 = 2, kUart4 = 4, kUart5 = 5 };
enum class UartBaudRate {
  kB300     = B300,
  kB600     = B600,
  kB1200    = B1200,
  kB2400    = B2400,
  kB4800    = B4800,
  kB9600    = B9600,
  kB19200   = B19200,
  kB38400   = B38400,
  kB57600   = B57600,
  kB115200  = B1152000,
  kB230400  = B230400,
  kB460800  = B460800,
  kB500000  = B500000,
  kB576000  = B576000,
  kB921600  = B921600,
  kB1000000 = B1000000,
  kB1152000 = B1152000,
  kB1500000 = B1500000,
  kB2000000 = B2000000,
  kB2500000 = B2500000,
  kB3000000 = B3000000,
  kB3500000 = B3500000
};
enum class UartBitsPerByte { k5 = CS5, k6 = CS6, k7 = CS7, k8 = CS8 };

class Uart : public IUart {
 public:
  /**
   * @brief  Creates a UART object.
   * A typical setting would be UartBus::kUart1, BaudRate::{TODOLater: Test to find},
   * BitsPerByte::k8
   */
  static std::optional<std::shared_ptr<Uart>> create(
    core::ILogger &logger,
    const UartBus bus,
    const UartBaudRate baud_rate,  // TODOLater: Figure out a default for this by testing
    const UartBitsPerByte bits_per_byte);
  Uart(core::ILogger &logger, const int file_descriptor);
  ~Uart();

  virtual core::Result sendBytes(const char *tx, const std::uint8_t length);
  virtual core::Result readBytes(unsigned char *rx, const std::uint8_t length);

 private:
  /**
   * @brief  Configures the UART file descriptor with the provided masks and pre-set settings
   */
  static core::Result configureFileForOperation(core::ILogger &logger,
                                                const int file_descriptor,
                                                const std::uint32_t baud_mask,
                                                const std::uint32_t bits_per_byte_mask);

 private:
  core::ILogger &logger_;
  const int file_descriptor_;
};

}  // namespace hyped::io