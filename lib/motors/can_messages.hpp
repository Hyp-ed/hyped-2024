#pragma once

#include <memory>
#include <vector>

#include <core/types.hpp>
#include <io/can.hpp>

namespace hyped::motors {

class ControllerCanProcessor : public io::ICanProcessor {
  enum Operation { kRead, kWrite };
  enum Location { kTemperature, kAccelerometer };
  enum Error { kInvalidOperation };
  static constexpr std::uint8_t kErrorId = 0;  // TODOLater decide

 public:
  explicit ControllerCanProcessor(std::shared_ptr<io::ICan> can);
  core::Result sendResponse(Operation operation, Location location, std::uint64_t data);
  core::Result sendError(Error error);
  core::Result receiveMessage(io::CanFrame frame);

 private:
  static std::vector<uint8_t> convertToBytes(std::uint64_t value, std::size_t length);
  std::shared_ptr<io::ICan> can_;
};

}  // namespace hyped::motors
