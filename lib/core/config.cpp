#include "config.hpp"

namespace hyped::core {

Config::Config(const std::string &path)
{
}

std::optional<Config> Config::create(const std::string &path)
{
  return std::nullopt;
}

}  // namespace hyped::core
