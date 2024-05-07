#pragma once 

#include <cstdint>
#include <cstring>
#include <memory>
#include <optional>

#include <core/logger.hpp>
#include <core/types.hpp>
#include <io/spi.hpp>

namespace hyped::debug {

	class OpticalFlowCommands {
		public:
		static core::Result addCommands {core::ILogger &logger,
                                  std::shared_ptr<Repl> repl,
                                  toml::v3::node_view<toml::v3::node> config};
	};
} //namespace hyped::debug