#pragma once

#include "controller.hpp"

#include <cstdint>
#include <memory>

#include <core/types.hpp>
#include <io/hardware_can.hpp>

namespace hyped::motors {

constexpr std::uint32_t kEmgyId = 0x80;
constexpr std::uint32_t kSdoId  = 0x580;
constexpr std::uint32_t kNmtId  = 0x700;

class CanProcessor : public io::ICanProcessor {
 public:
  CanProcessor(core::Logger &logger, std::shared_ptr<Controller> controller);
  core::Result processMessage(const io::CanFrame &frame);

 private:
  core::Logger &logger_;
  std::shared_ptr<Controller> controller_;
};

}  // namespace hyped::motors
