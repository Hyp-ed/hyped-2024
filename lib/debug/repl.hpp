#pragma once

#include "terminal.hpp"

#include <ncurses.h>

#include <vector>

#include "commands/ICommand.hpp"
#include <core/logger.hpp>

namespace hyped::debug {

class Repl {
 public:
  Repl(core::ILogger &logger, Terminal &terminal);
  void run();

  std::vector<std::string> autoComplete(const std::string &partial);

  void addCommand(std::unique_ptr<ICommand> command);
  void printHelp();

  void addHelpCommand();
  void addQuitCommand();

 private:
  core::ILogger &logger_;
  Terminal terminal_;
  std::vector<std::string> history_;
  std::vector<std::unique_ptr<ICommand>> commands_;
};
}  // namespace hyped::debug