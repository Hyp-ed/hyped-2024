#pragma once

#include <memory>
#include <vector>

#include <core/types.hpp>
#include <io/can.hpp>

namespace hyped::motors {

class CanMessages {
  enum Operation { kRead, kWrite };
  enum Location { kTemperature, kAccelerometer };
  enum Error { Error1, Error2 };
  static constexpr std::uint8_t kErrorId = 0;  // TBD

 public:
  CanMessages(std::shared_ptr<io::ICan> can);
  core::Result canSend(Operation operation, Location location, std::uint64_t data);
  core::Result canError(Error error);

 private:
  std::vector<uint8_t> convertToBytes(std::uint64_t value, std::size_t length);
  std::shared_ptr<io::ICan> can_;
};

}  // namespace hyped::motors