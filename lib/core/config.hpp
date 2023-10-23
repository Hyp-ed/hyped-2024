#include <optional>
#include <string>

#include <toml++/toml.hpp>

namespace hyped::core {

class Config {
 public:
  Config(const std::string &path);
  std::optional<Config> create(const std::string &path);

 private:
  toml::table config;
};

}  // namespace hyped::core