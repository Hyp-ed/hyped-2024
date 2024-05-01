#pragma once

#include <optional>
#include <string>

namespace hyped::core {
class HostInformation {
 public:
  HostInformation() = default;
  static std::optional<std::string> getName();
  static std::optional<std::string> getIp();
};

}  // namespace hyped::core
