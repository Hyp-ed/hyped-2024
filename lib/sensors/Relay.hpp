#pragma once

#include <cstdint>
#include <memory>

#include <optional>
#include <core/logger.hpp>
#include <io/gpio.hpp>

namespace hyped::sensors {

class Relay {
 public:
        static std::optional <Relay> create(core::ILogger &logger,
                                             std::shared_ptr <io::IGpio> gpio,
                                             const std::uint8_t new_pin);

        ~Relay(); 

    private:
       Relay(core::ILogger &logger, std::shared_ptr <io::IGpioWriter> gpio_writer);
};

class RelayWriter :
 public:
       static std::optional <RelayWriter>create(core::ILogger &logger,
                                                const int write_file_descriptor);

       ~RelayWriter();

   //  bool open();
     bool Write();  

   private:
     RelayWriter(core::ILogger &logger_, const int write_file_descriptor_);
};

}  // namespace hyped::sensors