#pragma once
#include "can.hpp"

#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

#include <core/logger.hpp>
#include <core/types.hpp>

namespace hyped::io {

class HardwareCan : public ICan {
 public:
  /**
   * @brief Construct a new Hardware Can object
   *
   * @param can_network_interface The name of the CAN network interface to use
   * @return std::shared_ptr<HardwareCan> if successful, std::nullopt otherwise
   */
  static std::optional<std::shared_ptr<HardwareCan>> create(
    core::ILogger &logger, const std::string &can_network_interface);
  HardwareCan(core::ILogger &logger, const int socket);
  ~HardwareCan();

  /**
   * @brief Send a CAN message
   * @param message The message to be sent
   * @return core::Result::kSuccess if the message was sent successfully core::Result::kFailure
   * otherwise
   * @note To send an extended CAN message, set the EFF flag in the can_id field by ORing it with
   * CAN_EFF_FLAG
   */
  core::Result send(const CanFrame &message);

  /**
   * @brief Attempt to receive a CAN message
   * @return core::Result::kSuccess if a message was received and successfully processed by all
   * registered processors, core::Result::kFailure if a message is not received successfully or if a
   * message is received but not processed by all registered processors
   * @note If a message is received then the message will be processed by all processors registered
   * to that ID
   * @note If the message is an extended CAN message, the EFF flag will be set in the can_id field
   * and thus the actual ID will be frame.can_id - 0x80000000
   */
  core::Result receive();

  /**
   * @brief Registers a processor to be called when a message with the given ID is received
   * @param id The ID of the message to be processed
   * @param processor The processor to be called when a message with the given ID is received
   */
  void addProcessor(const std::uint32_t id, std::shared_ptr<ICanProcessor> processor);

 private:
  core::ILogger &logger_;
  const int socket_;
  std::unordered_map<std::uint32_t, std::vector<std::shared_ptr<ICanProcessor>>> processors_;
};

}  // namespace hyped::io
