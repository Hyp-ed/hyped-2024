#pragma once

#include <io/adc.hpp>

namespace hyped::utils {

class DummyAdc : public io::IAdc {
 public:
  DummyAdc()  = default;
  ~DummyAdc() = default;

  virtual std::optional<core::Float> readValue();
};

}  // namespace hyped::utils