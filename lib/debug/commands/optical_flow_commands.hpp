#pragma once

#include <memory>

#include <core/logger.hpp>
#include <core/types.hpp>
#include <debug/repl.hpp>
#include <sensors/optical_flow.hpp>

namespace hyped::debug {

class OpticalFlowCommands {
 public:
  static core::Result addCommands(core::ILogger &logger,
                                  const std::shared_ptr<Repl> &repl,
                                  toml::v3::node_view<toml::v3::node> config);
};
}  // namespace hyped::debug
