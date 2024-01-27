#include <core/types.hpp>
#include <io/can.hpp>
#include <memory>

namespace hyped::motors {

class CanMessages {

    enum Operations {kRead, kWrite};
    enum Locations {temperature, accelerometer};
        
    public:
        core::Result CanSend(int operation, int location, int data);

    private:
        CanMessages(std::shared_ptr<io::ICan> can);

        std::vector<uint8_t> convertToBytes(int num, int length);
        std::shared_ptr<io::ICan> can_;
};

}