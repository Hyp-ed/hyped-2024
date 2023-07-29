#pragma once

#include <memory>
#include <optional>
#include <string>

#include <core/types.hpp>

#ifdef __linux__
#include <linux/can.h>
#endif
namespace hyped::io {

#ifdef __linux__
using CanFrame = can_frame;
#else
// structs and values as defined in <linux/can.h>
struct CanFrame {
  std::uint32_t can_id;              // 32 bit CAN_ID + EFF/RTR/ERR flags
  std::uint8_t can_dlc;              // frame payload length in bytes (0 .. CAN_MAX_DLEN)
  std::uint8_t __pad;                // padding - do not asign value
  std::uint8_t __res0;               // reserved - do not asign value
  std::uint8_t __res1;               // reserved - do not asign value
  std::array<std::uint8_t, 8> data;  // data that is sent over CAN, split into bytes
};
struct sockaddr_can {
  std::uint16_t can_family;
  int can_ifindex;
  union {
    // transport protocol class address info (e.g. ISOTP)
    struct {
      std::uint32_t rx_id, tx_id;
    } tp;
  } can_addr;
};
#define PF_CAN 29
#define AF_CAN 29
#define CAN_RAW 1
#define CAN_MAX_DLEN 8
#define CAN_EFF_FLAG 0x80000000U  // EFF/SFF is set in the MSB
#endif

class ICanProcessor {
 public:
  /**
   * @brief Called by ICan::receive() when a message with the given ID is received. What it then
   * does with the frame is up to the implementation
   * @param frame The CAN message to be processed
   * @return core::Result::kSuccess if the message was processed successfully,
   * core::Result::kFailure otherwise
   */
  virtual core::Result processMessage(const io::CanFrame &frame) = 0;
};

class ICan {
 public:
  /**
   * @brief Send a CAN message
   * @param message The message to be sent
   * @return core::Result::kSuccess if the message was sent successfully core::Result::kFailure
   * otherwise
   * @note To send an extended CAN message, set the EFF flag in the can_id field by ORing it with
   * CAN_EFF_FLAG
   */
  virtual core::Result send(const CanFrame &message) = 0;

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
  virtual core::Result receive() = 0;

  /**
   * @brief Registers a processor to be called when a message with the given ID is received
   * @param id The ID of the message to be processed
   * @param processor The processor to be called when a message with the given ID is received
   */
  virtual void addProcessor(const std::uint32_t id, std::shared_ptr<ICanProcessor> processor) = 0;
};
}  // namespace hyped::io