#include "optical_flow_commands.hpp"

#include <cstdint>

#include "io/hardware_spi.hpp"
#include <io/spi.hpp>

namespace hyped::debug {

core::Result OpticalFlowCommands::addCommands(core::ILogger &logger,
                                              const std::shared_ptr<Repl> &repl,
                                              toml::v3::node_view<toml::v3::node> config)
{
  // get SPI bus, mode, word size, bit order, and clock
  const auto optional_bus = config["bus"].value<std::uint8_t>();
  if (!optional_bus) {
    logger.log(core::LogLevel::kFatal, "No SPI bus specified");
    return core::Result::kFailure;
  }
  const auto bus           = static_cast<io::SpiBus>(*optional_bus);
  const auto optional_mode = config["mode"].value<std::uint8_t>();
  if (!optional_mode) {
    logger.log(core::LogLevel::kFatal, "No SPI mode specified");
    return core::Result::kFailure;
  }
  const auto mode               = static_cast<io::SpiMode>(*optional_mode);
  const auto optional_word_size = config["word_size"].value<std::uint8_t>();
  if (!optional_word_size) {
    logger.log(core::LogLevel::kFatal, "No SPI word size specified");
    return core::Result::kFailure;
  }
  const auto word_size          = static_cast<io::SpiWordSize>(*optional_word_size);
  const auto optional_bit_order = config["bit_order"].value<std::uint8_t>();
  if (!optional_bit_order) {
    logger.log(core::LogLevel::kFatal, "No SPI bit order specified");
    return core::Result::kFailure;
  }
  const auto bit_order      = static_cast<io::SpiBitOrder>(*optional_bit_order);
  const auto optional_clock = config["clock"].value<std::uint32_t>();
  if (!optional_clock) {
    logger.log(core::LogLevel::kFatal, "No SPI clock specified");
    return core::Result::kFailure;
  }
  const auto clock = static_cast<io::SpiClock>(*optional_clock);
  // get SPI instance
  auto optional_spi = repl->getSpi(bus, mode, word_size, bit_order, clock);
  if (!optional_spi) {
    logger.log(core::LogLevel::kFatal, "Error getting spi bus");
    return core::Result::kFailure;
  };
  const auto spi             = std::move(*optional_spi);
  auto optional_optical_flow = sensors::OpticalFlow::create(logger, spi);
  if (!optional_optical_flow) {
    logger.log(core::LogLevel::kFatal, "Error creating optical flow sensor");
    return core::Result::kFailure;
  }
  const auto optical_flow                         = std::move(*optional_optical_flow);
  const auto *const read_optical_flow_name        = "optical flow read";
  const auto *const read_optical_flow_usage       = "optical flow read";
  const auto *const read_optical_flow_description = "Read the optical flow sensor";
  const auto read_optical_flow_handler = [&logger, optical_flow](const std::vector<std::string> &) {
    const auto optional_delta_x = optical_flow->getDeltaX();
    if (!optional_delta_x) {
      logger.log(core::LogLevel::kFatal, "Error reading delta x");
      return;
    }
    const auto delta_x          = *optional_delta_x;
    const auto optional_delta_y = optical_flow->getDeltaY();
    if (!optional_delta_y) {
      logger.log(core::LogLevel::kFatal, "Error reading delta y");
      return;
    }
    const auto delta_y = *optional_delta_y;
    logger.log(core::LogLevel::kInfo, "Delta x: %d, Delta y: %d", delta_x, delta_y);
  };
  auto read_optical_flow_command = std::make_unique<Command>(read_optical_flow_name,
                                                             read_optical_flow_description,
                                                             read_optical_flow_usage,
                                                             read_optical_flow_handler);
  repl->addCommand(std::move(read_optical_flow_command));
  return core::Result::kSuccess;
}

}  // namespace hyped::debug