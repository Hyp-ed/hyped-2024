#pragma once
#include "command.hpp"

#include <memory>

#include <core/logger.hpp>
#include <core/types.hpp>
#include <debug/repl.hpp>
#include <sensors/accelerometer.hpp>

namespace hyped::debug {

class AccelerometerCommands {
 public:
  static core::Result addCommands(core::ILogger &logger,
                                  std::shared_ptr<Repl> repl,
                                  toml::v3::node_view<toml::v3::node> config);
};

}  // namespace hyped::debug