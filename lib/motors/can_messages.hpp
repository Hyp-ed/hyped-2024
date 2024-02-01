#pragma once

#include <memory>
#include <vector>

#include <core/types.hpp>
#include <io/can.hpp>

namespace hyped::motors {

class CanMessages {
  enum Operations { kRead, kWrite };
  enum Locations { temperature, accelerometer };

 public:
  core::Result CanSend(int operation, int location, int data);
  core::Result CanError(int error);

 private:
  CanMessages(std::shared_ptr<io::ICan> can);

  std::vector<uint8_t> convertToBytes(int num, int length);
  std::shared_ptr<io::ICan> can_;
};

}  // namespace hyped::motors