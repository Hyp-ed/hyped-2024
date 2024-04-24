#include "can_commands.hpp"

namespace hyped::debug {

core::Result CanCommands::addCommands(core::ILogger &logger, std::shared_ptr<Repl> repl)
{
  const auto write_command_name        = "can write";
  const auto write_command_description = "Write to a CAN bus";
  const auto write_command_usage       = "can write <bus> <CAN ID> <CAN data>";
  const auto write_command_handler     = [&logger, repl](std::vector<std::string> args) {
    if (args.size() != 3) {
      logger.log(core::LogLevel::kFatal, "Invalid number of arguments");
      return;
    }
    const auto bus          = args[0];
    const auto optional_can = repl->getCan(bus);
    if (!optional_can) {
      logger.log(core::LogLevel::kFatal, "Error getting CAN bus");
      return;
    };
    const auto can   = std::move(*optional_can);
    std::uint32_t id = std::stoul(args[1], nullptr, 16);
    std::string data = args[2];
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
    write_command_name, write_command_description, write_command_usage, write_command_handler);
  repl->addCommand(std::move(write_command));
  return core::Result::kSuccess;
}

}  // namespace hyped::debug
