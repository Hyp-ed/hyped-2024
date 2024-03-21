#include "host_information.hpp"

#include <string.h>
#include <unistd.h>

#include <cstdint>

#include <arpa/inet.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>

namespace hyped::core {

HostInformation::HostInformation()
{
}

std::optional<std::string> HostInformation::getName() const
{
  // There is no C++ standard function to get the hostname, so we have to use the POSIX gethostname
  char host[256];
  gethostname(host, sizeof(host));
  if (host[0] == '\0') { return std::nullopt; }
  return host;
}

std::optional<std::string> HostInformation::getIp() const
{
  std::uint16_t fd       = socket(AF_INET, SOCK_DGRAM, 0);
  struct ifreq ifr       = {};
  ifr.ifr_addr.sa_family = AF_INET;
  strncpy(ifr.ifr_name, "wlan0", IFNAMSIZ - 1);
  std::string ip;
  if (ioctl(fd, SIOCGIFADDR, &ifr) < 0) { return std::nullopt; }
  close(fd);
  ip = inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr);
  if (ip == "0.0.0.0") { return std::nullopt; }
  return ip;
}

}  // namespace hyped::core