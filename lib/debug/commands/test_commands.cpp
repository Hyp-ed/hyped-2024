#include "test_commands.hpp"

namespace hyped::debug {

core::Result TestCommands::addCommands(core::ILogger &logger, std::shared_ptr<Repl> repl)
{
  const auto test_command_name        = "echo";
  const auto test_command_description = "echoes the arguments back to the user";
  const auto test_command_usage       = "echo <args-to-print...>";
  const auto test_command_handler     = [&logger](std::vector<std::string> args) {
    for (std::size_t i = 0; i < args.size(); i++) {
      logger.log(core::LogLevel::kInfo, "arg %i = %s", i, args[i].c_str());
    }
  };
  auto test_command = std::make_unique<Command>(
    test_command_name, test_command_description, test_command_usage, test_command_handler);
  repl->addCommand(std::move(test_command));
  return core::Result::kSuccess;
}

}  // namespace hyped::debug
