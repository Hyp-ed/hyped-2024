#pragma once

#include "i2c_sensors.hpp"

#include <cstdint>
#include <memory>
#include <optional>

#include <core/logger.hpp>
#include <io/i2c.hpp>  

namespace hyoed :: sensors {
constexpr std::uint8_t kDefaultVL6180XAddress = 0x29;

class VL6180X : public II2cMuxSensor<std::int16_t> {
 public:
  static std::optional<VL6180X> create(core::ILogger &logger,
                                        std::shared_ptr<io::II2c> i2c,
                                        const std::uint8_t channel,
                                        const std::uint8_t device_address = kDefaultVL6180XAddress);  

   ~VL6180X();       

   std::optional<core::Result> checkStatus();
   std::uint8_t getChannel() const;

   private:
  VL6180X(core::ILogger &logger,
          std::shared_ptr<io::II2c> i2c,
          const std::uint8_t channel,
          const std::uint8_t device_address);

 private:
  core::ILogger &logger_;
  std::shared_ptr<io::II2c> i2c_;
  const std::uint8_t channel_;
  const std::uint8_t device_address_;

private:
  // Register addresses/values taken from the VL6180X datasheet
  static constexpr std::uint8_t kCtrl = 0x00;  
  static constexpr std::uint8_t kDataHigh = 0x00;  
  static constexpr std::uint8_t kDataLow = 0x00;  
  static constexpr std::uint8_t kStatus = 0x00;  
  static constexpr std::uint8_t kBusy = 0x00;  
  static constexpr std::uint8_t kConfigurationSetting = 0x00;  
};

}  // namespace hyped::sensors
