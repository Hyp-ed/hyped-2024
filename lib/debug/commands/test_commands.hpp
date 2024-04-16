#pragma once
#include "command.hpp"

#include <memory>

#include <core/logger.hpp>
#include <core/types.hpp>
#include <debug/repl.hpp>

namespace hyped::debug {

class TestCommands {
 public:
  static core::Result addCommands(core::ILogger &logger, std::shared_ptr<Repl> repl);
};

}  // namespace hyped::debug
