#include "relay.hpp"

namespace hyped::sensors {

std::optional<Relay> Relay::create(core::ILogger &logger,
                                   std::shared_ptr<io::IGpio> gpio,
                                   const std::uint8_t new_pin)
{
  int fd = open("/sys/class/gpio/gpio115/value", O_WRONLY);
  if (fd == -1) {
    logger.log(core::LogLevel::kFatal, "Failed to open GPIO file");
    return std::nullopt;
  }

  logger.log(core::LogLevel::kDebug, "Relay created successfully");

  return Relay(logger, fd, pin);
}

core::Result Relay::open()
{
  return writeGpio(core::DigitalSignal::kHigh);
}

core::Result Relay::close()
{
  return writeGpio(core::DigitalSignal::kLow);
}

Relay::Relay(core::ILogger &logger, int fd, std::uint8_t pin)
    : logger_(logger),
      write_file_descriptor_(fd),
      pin_(pin)
{
}

core::Result Relay::writeGpio(core::DigitalSignal signal)
{
  int result = write(write_file_descriptor_, signal);
  if (result == -1) {
    logger_.log(core::LogLevel::kError, "Failed to write to GPIO");
    return core::Result::kFailure;
  }

  return core::Result::kSuccess;
}

}  // namespace hyped::sensors