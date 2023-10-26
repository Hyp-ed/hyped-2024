#include "repl.hpp"
#include "repl_logger.hpp"

#include "commands/ICommand.hpp"
#include <core/logger.hpp>
#include <core/wall_clock.hpp>

namespace hyped::debug {

Repl::Repl(core::ILogger &logger, Terminal &terminal) : logger_(logger), terminal_(terminal)
{
  addHelpCommand();
  addQuitCommand();
}

void Repl::run()
{
  int i             = 0;
  std::string input = "";

  while (true) {
    terminal_.refresh_line(input, ">> ");
    const auto &[key, letter] = terminal_.prompt();
    if (key == debug::KeyPress::kASCII) {
      input += letter;
    } else if (key == debug::KeyPress::kEnter) {
      terminal_.cr();
      // Print input
      for (auto &command : commands_) {
        if (command->getName() == input) {
          command->execute();
          break;
        }
      }
      history_.push_back(input);
      input = "";
      i     = 0;
    } else if (key == debug::KeyPress::kUp) {
      if (i < history_.size()) {
        input = history_[history_.size() - 1 - i];
        i++;
      }
    } else if (key == debug::KeyPress::kDown) {
      if (i > 0) {
        i--;
        input = history_[history_.size() - i];
      }
    } else if (key == debug::KeyPress::kBackspace) {
      if (input.size() > 0) { input.pop_back(); }
    } else if (key == debug::KeyPress::kTab) {
      std::vector<std::string> matches = autoComplete(input);
      if (matches.size() == 1) {
        input = matches[0];
      } else if (matches.size() > 1) {
        terminal_.cr();
        for (auto &match : matches) {
          terminal_.printf("%s\n", match.c_str());
        }
        terminal_.refresh_line(input, ">> ");
      }
    } else if (key == debug::KeyPress::kEscape) {
      input = "";
      terminal_.cr();
    }
  }
}

std::vector<std::string> Repl::autoComplete(const std::string &partial)
{
  std::vector<std::string> matches;
  for (auto &command : commands_) {
    if (command->getName().find(partial) == 0) { matches.push_back(command->getName()); }
  }
  return matches;
}

void Repl::addCommand(std::unique_ptr<ICommand> command)
{
  logger_.log(core::LogLevel::kDebug, "Adding command: %s", command->getName().c_str());
  commands_.push_back(std::move(command));
}

void Repl::printHelp()
{
  for (auto &command : commands_) {
    logger_.log(core::LogLevel::kDebug,
                "%s: %s",
                command->getName().c_str(),
                command->getDescription().c_str());
  }
}

void Repl::addHelpCommand()
{
  addCommand(
    std::make_unique<ICommand>("help", "Print this help message", [this]() -> core::Result {
      printHelp();
      return core::Result::kSuccess;
    }));
}

void Repl::addQuitCommand()
{
  addCommand(std::make_unique<ICommand>("quit", "Quit the program", [this]() -> core::Result {
    terminal_.quit();
    exit(0);
    return core::Result::kSuccess;
  }));
}

}  // namespace hyped::debug