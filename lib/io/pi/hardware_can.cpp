#include "hardware_can.hpp"

#include <errno.h>
#include <stdio.h>
#include <unistd.h>

#include <cstring>

#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>

namespace hyped::io {

std::optional<std::shared_ptr<HardwareCan>> HardwareCan::create(
  core::ILogger &logger, const std::string &can_network_interface)
{
  const int socket_id = socket(PF_CAN, SOCK_RAW, CAN_RAW);
  if (socket_id < 0) {
    logger.log(core::LogLevel::kFatal, "Unable to open CAN socket");
    return std::nullopt;
  }
  const int interface_index = if_nametoindex(can_network_interface.c_str());
  if (!interface_index) {
    logger.log(core::LogLevel::kFatal,
               "Unable to find CAN network interface '%s'",
               can_network_interface.c_str());
    close(socket_id);
    return std::nullopt;
  }
  const sockaddr_can socket_address = {AF_CAN, interface_index};
  const int bind_status
    = bind(socket_id, reinterpret_cast<const sockaddr *>(&socket_address), sizeof(socket_address));
  if (bind_status < 0) {
    logger.log(core::LogLevel::kFatal, "Unable to bind CAN socket");
    close(socket_id);
    return std::nullopt;
  }
  logger.log(core::LogLevel::kInfo, "CAN socket successfully created");
  return std::make_shared<HardwareCan>(logger, socket_id);
}

HardwareCan::HardwareCan(core::ILogger &logger, const int socket)
    : logger_(logger),
      processors_(),
      socket_(socket)
{
}

HardwareCan::~HardwareCan()
{
  close(socket_);
}

core::Result HardwareCan::send(const io::CanFrame &data)
{
  if (socket_ < 0) {
    logger_.log(core::LogLevel::kFatal, "Trying to send CAN data but no CAN socket found");
    return core::Result::kFailure;
  }
  const int num_bytes_written = write(socket_, &data, sizeof(CanFrame));
  if (num_bytes_written != sizeof(CanFrame)) {
    logger_.log(
      core::LogLevel::kFatal, "Failed to send CAN data because: %s", std::strerror(errno));
    return core::Result::kFailure;
  }
  // TODOLater make logger_ more elegant
  logger_.log(core::LogLevel::kDebug,
              "CAN message sent, ID: %X, DATA: %X %X %X %X %X %X %X %X",
              static_cast<int>(data.can_id),
              static_cast<int>(data.data[0]),
              static_cast<int>(data.data[1]),
              static_cast<int>(data.data[2]),
              static_cast<int>(data.data[3]),
              static_cast<int>(data.data[4]),
              static_cast<int>(data.data[5]),
              static_cast<int>(data.data[6]),
              static_cast<int>(data.data[7]));
  return core::Result::kSuccess;
}

core::Result HardwareCan::receive()
{
  if (ioctl(socket_, FIONREAD) < sizeof(CanFrame)) { return core::Result::kFailure; }
  CanFrame message;
  if (ioctl(socket_, FIONREAD) < sizeof(CanFrame)) {
    logger_.log(core::LogLevel::kDebug, "No CAN data in rx queue");
    return core::Result::kFailure;
  }
  const int num_bytes_read = read(socket_, &message, sizeof(CanFrame));
  if (num_bytes_read < sizeof(CanFrame)) {
    logger_.log(core::LogLevel::kFatal, "Failed to receive CAN data");
    return core::Result::kFailure;
  }
  // TODOLater make logger more elegant
  logger_.log(core::LogLevel::kDebug,
              "CAN message received, ID:%X, DATA: %X %X %X %X %X %X %X %X",
              static_cast<int>(message.can_id),
              static_cast<int>(message.data[0]),
              static_cast<int>(message.data[1]),
              static_cast<int>(message.data[2]),
              static_cast<int>(message.data[3]),
              static_cast<int>(message.data[4]),
              static_cast<int>(message.data[5]),
              static_cast<int>(message.data[6]),
              static_cast<int>(message.data[7]));
  const auto subscribed_processors = processors_.find(message.can_id);
  if (subscribed_processors == processors_.end()) {
    logger_.log(core::LogLevel::kFatal, "No CanProccessor associated with id %X", message.can_id);
    return core::Result::kFailure;
  }
  for (auto &processor : subscribed_processors->second) {
    core::Result process_result = processor->processMessage(message);
    if (process_result == core::Result::kFailure) { return core::Result::kFailure; }
  }
  return core::Result::kSuccess;
}

void HardwareCan::addProcessor(const std::uint32_t id, std::shared_ptr<ICanProcessor> processor)
{
  const auto id_and_processors = processors_.find(id);
  if (id_and_processors == processors_.end()) {
    const auto processors_for_id = std::vector<std::shared_ptr<ICanProcessor>>({processor});
    processors_.emplace(id, processors_for_id);
  } else {
    id_and_processors->second.push_back(processor);
  }
  logger_.log(core::LogLevel::kDebug, "Added processor for id %X", id);
}

}  // namespace hyped::io
