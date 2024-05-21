#include "keyence_commands.hpp"

#include <cstdint>
#include <thread>

#include "core/logger.hpp"
#include "core/time.hpp"
#include "core/types.hpp"
#include "sensors/keyence.hpp"

namespace hyped::debug {

core::Result KeyenceCommands::addCommands(core::ILogger &logger,
                                          std::shared_ptr<Repl> &repl,
                                          core::ITimeSource &time,
                                          toml::v3::node_view<toml::v3::node> config)
{
  const auto optional_pin = config["pin"].value<std::uint8_t>();
  if (!optional_pin) {
    logger.log(core::LogLevel::kFatal, "Invalid pin");
    return core::Result::kFailure;
  }
  const auto pin     = *optional_pin;
  auto optional_gpio = repl->getGpio();
  if (!optional_gpio) {
    logger.log(core::LogLevel::kFatal, "Failed to get gpio");
    return core::Result::kFailure;
  }
  const auto gpio       = *optional_gpio;
  auto optional_keyence = sensors::Keyence::create(logger, gpio, pin);
  if (!optional_keyence) {
    logger.log(core::LogLevel::kFatal, "Failed to create keyence sensor");
    return core::Result::kFailure;
  }
  auto keyence                            = *optional_keyence;
  const auto *const keyence_command_name  = "keyence read";
  const auto *const keyence_command_usage = "keyence read <time> <interval>";
  const auto *const keyence_command_description
    = "Read from the keyence sensor every <interval> seconds for <time> seconds";
  const auto keyence_command_handler = [&logger, keyence, &time](std::vector<std::string> args) {
    if (args.size() != 2) {
      logger.log(core::LogLevel::kFatal, "Invalid number of arguments");
      return;
    }
    const std::uint32_t run_time = std::stoul(args[0], nullptr, 10);
    const std::uint32_t interval = std::stof(args[1]);
    const auto start_time        = time.now();
    while (time.now() - start_time < std::chrono::seconds(run_time)) {
      keyence->updateStripeCount();
      const auto stripe_count = keyence->getStripeCount();
      logger.log(core::LogLevel::kInfo, "Stripe count: %d", stripe_count);
      std::this_thread::sleep_for(std::chrono::seconds(interval));
    }
  };
  auto keyence_command = std::make_unique<Command>(keyence_command_name,
                                                   keyence_command_description,
                                                   keyence_command_usage,
                                                   keyence_command_handler);
  repl->addCommand(std::move(keyence_command));
  return core::Result::kSuccess;
}

}  // namespace hyped::debug
