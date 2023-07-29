#pragma once

#include <functional>

#include <io/gpio.hpp>

namespace hyped::utils {

/**
 * IGpioReader implementation for DummyGpio.
 */
class DummyGpioReader : public io::IGpioReader {
 public:
  using ReadHandler = std::function<std::optional<core::DigitalSignal>(const std::uint8_t pin)>;
  virtual std::optional<core::DigitalSignal> read();

 private:
  DummyGpioReader(const std::uint8_t pin, ReadHandler read_handler);
  friend class DummyGpio;

  const std::uint8_t pin_;
  ReadHandler read_handler_;
};

/**
 * IGpioWriter implementation for DummyGpio.
 */
class DummyGpioWriter : public io::IGpioWriter {
 public:
  using WriteHandler
    = std::function<core::Result(const std::uint8_t pin, const core::DigitalSignal state)>;
  virtual core::Result write(const core::DigitalSignal state);

 private:
  DummyGpioWriter(const std::uint8_t pin, WriteHandler write_handler);
  friend class DummyGpio;

  const std::uint8_t pin_;
  WriteHandler write_handler_;
};

/**
 * A basic implementation of IGpio that does not require any hardware. The purpose of this
 * class is to
 *
 *  1. demonstrate how to implement the IGpio interface, and
 *  2. allow for quick test implementations using lambdas.
 */
class DummyGpio : public io::IGpio {
 public:
  DummyGpio(DummyGpioReader::ReadHandler read_handler, DummyGpioWriter::WriteHandler write_handler);

  std::optional<std::shared_ptr<io::IGpioReader>> getReader(const std::uint8_t pin);
  std::optional<std::shared_ptr<io::IGpioWriter>> getWriter(const std::uint8_t pin);

 private:
  DummyGpioReader::ReadHandler read_handler_;
  DummyGpioWriter::WriteHandler write_handler_;
};

}  // namespace hyped::utils
