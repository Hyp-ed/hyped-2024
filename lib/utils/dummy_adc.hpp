#pragma once

#include <io/adc.hpp>

namespace hyped::utils {

class DummyAdc : public io::IAdc {
 public:
  std::optional<core::Float> readValue() override;
};

}  // namespace hyped::utils
