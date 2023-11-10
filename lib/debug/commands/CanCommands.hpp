#pragma once
#include "Command.hpp"

#include <memory>
#include <vector>

#include <core/logger.hpp>
#include <core/types.hpp>
#include <debug/repl.hpp>
#include <io/can.hpp>

namespace hyped::debug {
class CanCommands {
 public:
  static core::Result addCommands(core::ILogger &logger,
                                  std::shared_ptr<Repl> repl,
                                  toml::v3::node_view<toml::v3::node> config);
};
}  // namespace hyped::debug
