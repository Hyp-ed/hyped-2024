#pragma once

#include "logger.hpp"

#include <optional>
#include <string>

#include <toml++/toml.hpp>

namespace hyped::core {

class Config {
 public:
  Config(core::ILogger &logger, toml::table config);
  static std::optional<Config> create(core::ILogger &logger, const std::string &path);

 private:
  core::ILogger &logger_;
  toml::table config_;
};

}  // namespace hyped::core