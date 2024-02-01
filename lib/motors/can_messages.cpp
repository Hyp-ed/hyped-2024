#include "can_messages.hpp"

namespace hyped::motors {

std::vector<uint8_t> convertToBytes(int num, int length)
{
  std::vector<uint8_t> bytes = {0};

  for (int i = 0; i < length; i++) {
    bytes[i] = (num >> 8 * (length - (i + 1))) & 0xFF;
  }
  return bytes;
}

core::Result CanMessages::CanSend(int operation, int location, int data)
{
  io::CanFrame frame;

  frame.can_id  = 0x00;
  frame.can_dlc = 8;
  frame.data[0] = convertToBytes(operation, 1)[0];
  frame.data[1] = convertToBytes(location, 2)[0];
  frame.data[2] = convertToBytes(location, 2)[1];
  frame.data[3] = 0x00;
  frame.data[4] = convertToBytes(data, 5)[1];
  frame.data[5] = convertToBytes(data, 5)[2];
  frame.data[6] = convertToBytes(data, 5)[3];
  frame.data[7] = convertToBytes(data, 5)[4];

  core::Result end = can_->send(frame);
  return end;
}

core::Result CanMessages::CanError(int error)
{
  io::CanFrame frame;

  frame.can_id  = convertToBytes(error, 1)[0];
  frame.can_dlc = 8;
  frame.data[0] = 0x00;
  frame.data[1] = convertToBytes(/*location*/ 0, 2)[0];  // wherever the location is
  frame.data[2] = convertToBytes(0, 2)[0];
  frame.data[3] = 0x00;
  frame.data[4] = 0x00;
  frame.data[5] = 0x00;
  frame.data[6] = 0x00;
  frame.data[7] = 0x00;

  core::Result end = can_->send(frame);
  return end;
}

}  // namespace hyped::motors