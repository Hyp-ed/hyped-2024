#include "dummy_gpio.hpp"

namespace hyped::utils {

DummyGpioReader::DummyGpioReader(const std::uint8_t pin, DummyGpioReader::ReadHandler read_handler)
    : pin_(pin),
      read_handler_(read_handler)
{
}

std::optional<core::DigitalSignal> DummyGpioReader::read()
{
  return read_handler_(pin_);
}

DummyGpioWriter::DummyGpioWriter(const std::uint8_t pin,
                                 DummyGpioWriter::WriteHandler write_handler)
    : pin_(pin),
      write_handler_(write_handler)
{
}

core::Result DummyGpioWriter::write(const core::DigitalSignal state)
{
  return write_handler_(pin_, state);
}

DummyGpio::DummyGpio(DummyGpioReader::ReadHandler read_handler,
                     DummyGpioWriter::WriteHandler write_handler)
    : read_handler_(read_handler),
      write_handler_(write_handler)
{
}

std::optional<std::shared_ptr<io::IGpioReader>> DummyGpio::getReader(const std::uint8_t pin)
{
  return std::make_shared<DummyGpioReader>(DummyGpioReader(pin, read_handler_));
}

std::optional<std::shared_ptr<io::IGpioWriter>> DummyGpio::getWriter(const std::uint8_t pin)
{
  return std::make_shared<DummyGpioWriter>(DummyGpioWriter(pin, write_handler_));
}

}  // namespace hyped::utils
