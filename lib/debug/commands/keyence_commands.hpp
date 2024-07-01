#pragma once

#include <memory>

#include <core/logger.hpp>
#include <core/types.hpp>
#include <debug/repl.hpp>

namespace hyped::debug {

class KeyenceCommands {
 public:
  /**
   * @brief Add commands to the REPL for reading from keyence stripe counters
   *
   * @param logger
   * @param repl
   * @param config
   * @return core::Result kSuccess if commands were added successfully, kFailure otherwise
   */
  static core::Result addCommands(core::ILogger &logger,
                                  std::shared_ptr<Repl> &repl,
                                  core::ITimeSource &time,
                                  toml::v3::node_view<toml::v3::node> config);
};

}  // namespace hyped::debug
