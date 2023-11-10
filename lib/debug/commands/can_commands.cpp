#include "can_commands.hpp"

namespace hyped::debug {
core::Result CanCommands::addCommands(core::ILogger &logger,
                                      std::shared_ptr<Repl> repl,
                                      toml::v3::node_view<toml::v3::node> config)
{
  const auto buses = config["buses"].as_array();
  if (!buses) {
    logger.log(core::LogLevel::kFatal, "No CAN buses specified");
    return core::Result::kFailure;
  }
  for (const auto &can_bus : *buses) {
    const auto optional_bus = can_bus.value<std::string>();
    if (!optional_bus) {
      logger.log(core::LogLevel::kFatal, "Invalid CAN bus name");
      return core::Result::kFailure;
    }
    const auto bus          = *optional_bus;
    const auto optional_can = repl->getCan(bus);
    if (!optional_can) {
      logger.log(core::LogLevel::kFatal, "Error creating CAN bus");
      return core::Result::kFailure;
    };
    const auto can                       = std::move(*optional_can);
    const auto write_command_name        = bus + " read";
    const auto write_command_description = "Read from CAN bus " + bus;
    const auto write_command_handler     = [&logger, can, bus]() {
      std::cout << "Enter CAN ID: ";
      std::uint32_t id;
      std::cin >> std::hex >> id;
      std::cout << "Enter CAN data: ";
      std::string data;
      std::cin >> std::hex >> data;
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      if (data.length() > 16) {
        logger.log(core::LogLevel::kFatal, "Cannot send can data longer than 8 bytes");
        return;
      }
      const auto can_data = std::vector<std::uint8_t>(data.begin(), data.end());
      io::CanFrame can_frame;
      can_frame.can_id  = id;
      can_frame.can_dlc = can_data.size();
      for (int i = 0; i < can_data.size(); i++) {
        can_frame.data[i] = can_data[i];
      }
      core::Result result = can->send(can_frame);
      if (result == core::Result::kFailure) {
        logger.log(core::LogLevel::kFatal, "Failed to write to CAN bus %s", bus.c_str());
        return;
      }
      logger.log(core::LogLevel::kDebug, "Wrote to CAN bus %s", bus.c_str());
      return;
    };
    auto write_command = std::make_unique<Command>(
      write_command_name, write_command_description, write_command_handler);
    repl->addCommand(std::move(write_command));
  }
  return core::Result::kSuccess;
}

}  // namespace hyped::debug
