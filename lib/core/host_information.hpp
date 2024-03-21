#include <optional>
#include <string>

namespace hyped::core {
class HostInformation {
 public:
  HostInformation();
  std::optional<std::string> getName() const;
  std::optional<std::string> getIp() const;
};

}  // namespace hyped::core
