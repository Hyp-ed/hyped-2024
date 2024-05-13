#include "optical_flow_commands.hpp"

#include <cstdint>

namespace hyped::debug {
core::Result OpticalFlowCommands::addCommands(core::ILogger &logger,
                                  std::shared_ptr<Repl> repl,
                                  toml::v3::node_view<toml::v3::node> config)
{
	//get bus
	const auto optional_bus = config["bus"].value<std::uint8_t>();
    if (!optional_bus) {
    logger.log(core::LogLevel::kFatal, "No SPI bus specified");
    return core::Result::kFailure;
    };
  	const auto bus = *optional_bus;

	// get SPI instance
  const auto optional_spi = repl->getSpi(bus, mode, word_size, bit_order, clock);
    if (!optional_spi) {
    logger.log(core::LogLevel::kFatal, "Error creating I2C bus");
    return core::Result::kFailure;
    };
    const auto i2c = std::move(*optional_spi);
}
}