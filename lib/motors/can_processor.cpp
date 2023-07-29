#include "can_processor.hpp"
#include "controller.hpp"

namespace hyped::motors {

CanProcessor::CanProcessor(core::Logger &logger, std::shared_ptr<Controller> controller)
    : logger_(logger),
      controller_(controller)
{
}

core::Result CanProcessor::processMessage(const io::CanFrame &frame)
{
  const std::uint32_t id = frame.can_id;
  const std::uint16_t index
    = (static_cast<std::uint16_t>(frame.data[1]) << 8) | static_cast<std::uint16_t>(frame.data[0]);
  const std::uint8_t subindex = frame.data[2];
  const std::uint16_t data    = (static_cast<std::uint16_t>(frame.data[7]) << 24)
                             | (static_cast<std::uint16_t>(frame.data[6]) << 16)
                             | (static_cast<std::uint16_t>(frame.data[5]) << 8)
                             | static_cast<std::uint16_t>(frame.data[4]);
  // Handle error messages
  if (id == motors::kEmgyId) {
    controller_->processErrorMessage(data);
    return core::Result::kFailure;
  }
  // Handle NMT messages
  if (id == motors::kNmtId) { return controller_->processNmtMessage(frame.data[3]); }
  // Handle SDO messages
  if (id == motors::kSdoId) { return controller_->processSdoMessage(index, subindex, data); }
  logger_.log(core::LogLevel::kFatal,
              "Unknown CAN message received. ID: %d, Index: %d, Subindex: %d, Data: %d",
              id,
              index,
              subindex,
              data);
  return core::Result::kFailure;
}

}  // namespace hyped::motors
