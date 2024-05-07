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
    logger.log(core::LogLevel::kFatal, "No I2C bus specified");
    return core::Result::kFailure;
    };
  	const auto bus = *optional_bus;

	//const auto optional_spi = repl->getSpi()
}
}