#include "mock_hardware_i2c.hpp"


namespace hyped::io {

    std::optional<std::shared_ptr<mock_HardwareI2c>> mock_HardwareI2c::create(core::ILogger &logger,
                                                                const std::uint8_t bus)
                                                                {
    if (bus > 2) {
    logger.log(core::LogLevel::kFatal, "Failed to create mock_HardwareI2c object: invalid bus");
    return std::nullopt;
    }
    

    return std::make_shared<mock_HardwareI2c>(logger,0);
    }
    
    mock_HardwareI2c::mock_HardwareI2c(core::ILogger &logger, const int file_descriptor)
    : logger_(logger),
      file_descriptor_(file_descriptor),
      sensor_address_(0)
    {
    }
                                                                
    
    mock_HardwareI2c::~mock_HardwareI2c(){}


    std::optional<std::uint8_t> mock_HardwareI2c::readByte(const std::uint8_t device_address,const std::uint8_t register_address){
        if (sensor_address_ != device_address) { setSensorAddress(device_address); }

        std::uint8_t read[1];
        std::uint8_t random = rand();
        read[0] = random;
        return read[0];
    }


    core::Result mock_HardwareI2c::writeByteToRegister(const std::uint8_t device_address,const std::uint8_t register_address,const std::uint8_t data){
        if (sensor_address_ != device_address) { setSensorAddress(device_address); }
        
        logger_.log(core::LogLevel::kDebug, "Successfully wrote byte to i2c device");
        return core::Result::kSuccess;
    }

    core::Result mock_HardwareI2c::writeByte(const std::uint8_t device_address, const std::uint8_t data){
        if (sensor_address_ != device_address) { setSensorAddress(device_address); }

        logger_.log(core::LogLevel::kDebug, "Successfully wrote byte to i2c device");
        return core::Result::kSuccess;
    }

    void mock_HardwareI2c::setSensorAddress(const std::uint8_t device_address){
        sensor_address_        = device_address;
    }

}