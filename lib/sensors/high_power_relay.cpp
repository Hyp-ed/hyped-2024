#include "high_power_relay.hpp"

namespace hyped::sensors {

std::optional<HpRelay> HpRelay::create(core::ILogger &logger,
                                       std::shared_ptr<io::IGpio> gpio,
                                       const std::uint8_t new_pin)
{
  const auto reader = gpio->getReader(new_pin, io::Edge::kNone);
  if (!reader) {
    logger.log(core::LogLevel::kFatal, "Failed to create HpRelay instance");
    return std::nullopt;
  }
  logger.log(core::LogLevel::kDebug, "Successfully created HpRelay instance");
  return Keyence(logger, *reader);
}

HpRelay::HpRelay(core::ILogger &logger, std::shared_ptr<io::IGpioReader> gpio_reader)
    : gpio_reader_(gpio_reader),
      logger_(logger)
{
}

HpRelay::~HpRelay()
{
}

}  // namespace hyped::sensors