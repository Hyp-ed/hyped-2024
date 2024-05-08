#pragma once

#include <memory>

#include <core/logger.hpp>
#include <core/types.hpp>
#include <debug/repl.hpp>

namespace hyped::debug {

class I2cCommands {
 public:
  /**
   * @brief Add commands to the REPL for reading from and writing to I2C devices
   *
   * @param logger
   * @param repl
   * @return core::Result
   */
  static core::Result addCommands(core::ILogger &logger, std::shared_ptr<Repl> &repl);
};

}  // namespace hyped::debug
