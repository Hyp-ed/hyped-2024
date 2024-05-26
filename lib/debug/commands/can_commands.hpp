#pragma once

#include <memory>

#include <core/logger.hpp>
#include <core/types.hpp>
#include <debug/repl.hpp>

namespace hyped::debug {

class CanCommands {
 public:
  /**
   * @brief Add commands to the REPL for reading from and writing to CAN buses
   *
   * @param logger
   * @param repl
   * @return core::Result kSuccess if commands were added successfully, kFailure otherwise
   */
  static core::Result addCommands(core::ILogger &logger, std::shared_ptr<Repl> &repl);
};

}  // namespace hyped::debug
