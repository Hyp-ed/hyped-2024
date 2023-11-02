#include "config.hpp"

namespace hyped::core {

Config::Config(core::ILogger &logger, toml::table config) : logger_(logger), config_(config)
{
}

std::optional<Config> Config::create(core::ILogger &logger, const std::string &path)
{
  toml::table table;
  try {
    table = toml::parse_file(path);
  } catch (const toml::parse_error &e) {
    logger.log(core::LogLevel::kFatal, "Error parsing TOML file: %s", e.description());
    return std::nullopt;
  }
  logger.log(core::LogLevel::kDebug, "Successfully parsed config file");
  return Config(logger, table);
}

}  // namespace hyped::core
