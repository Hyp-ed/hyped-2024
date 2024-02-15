#include "can_messages.hpp"

namespace hyped::motors {

VectorControlCanMessages::VectorControlCanMessages(std::shared_ptr<io::ICan> can)
    : can_(std::move(can))
{
}

std::vector<std::uint8_t> VectorControlCanMessages::convertToBytes(std::uint64_t value,
                                                                   std::size_t length)
{
  std::vector<std::uint8_t> bytes = {};

  for (std::size_t i = 0; i < length; i++) {
    const std::uint8_t byte = (value >> (8 * (length - 1 - i))) & 0xFF;
    bytes.push_back(byte);
  }
  return bytes;
}

core::Result VectorControlCanMessages::canSend(Operation operation,  // for general update messages
                                               Location location,
                                               std::uint64_t data)
{
  io::CanFrame frame;

  const std::vector<std::uint8_t> locationVector = convertToBytes(location, 2);
  const std::vector<std::uint8_t> dataVector     = convertToBytes(data, 4);

  frame.can_id  = 0x00;
  frame.can_dlc = 8;
  frame.data[0] = static_cast<std::uint8_t>(operation);
  frame.data[1] = locationVector[0];
  frame.data[2] = locationVector[1];
  frame.data[3] = 0x00;
  frame.data[4] = dataVector[0];
  frame.data[5] = dataVector[1];
  frame.data[6] = dataVector[2];
  frame.data[7] = dataVector[3];

  const core::Result result = can_->send(frame);
  return result;
}

core::Result VectorControlCanMessages::canError(Error error)
{
  io::CanFrame frame;

  const std::vector<uint8_t> errorVector = convertToBytes(error, 4);

  frame.can_id  = kErrorId;
  frame.can_dlc = 8;
  frame.data[0] = errorVector[0];
  frame.data[1] = errorVector[1];
  frame.data[2] = errorVector[2];
  frame.data[3] = errorVector[3];
  frame.data[4] = 0x00;
  frame.data[5] = 0x00;
  frame.data[6] = 0x00;
  frame.data[7] = 0x00;

  const core::Result result = can_->send(frame);
  return result;
}

}  // namespace hyped::motors