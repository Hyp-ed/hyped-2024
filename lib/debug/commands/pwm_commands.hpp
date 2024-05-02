#pragma once
#include "command.hpp"

#include <memory>

#include <core/logger.hpp>
#include <core/types.hpp>
#include <debug/repl.hpp>

namespace hyped::debug {

class PwmCommands {
 public:
  /**
   * @brief Add commands to the REPL for starting and stopping PWM signals
   *
   * @param logger
   * @param repl
   * @return core::Result kSuccess if commands were added successfully, kFailure otherwise
   */
  static core::Result addCommands(core::ILogger &logger, std::shared_ptr<Repl> repl);
};

}  // namespace hyped::debug
