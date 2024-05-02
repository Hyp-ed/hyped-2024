#pragma once

#include <io/adc.hpp>

namespace hyped::utils {

class DummyAdc : public io::IAdc {
 public:
  DummyAdc();
  ~DummyAdc() = default;

  virtual std::optional<core::Float> readValue();
  void setValue(std::vector<std::optional<core::Float>> values);

 private:
  std::vector<std::optional<core::Float>> values_;
};

}  // namespace hyped::utils
