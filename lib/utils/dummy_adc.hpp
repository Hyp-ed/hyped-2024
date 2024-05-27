#pragma once

#include <io/adc.hpp>

namespace hyped::utils {

class DummyAdc : public io::IAdc {
 public:
  std::optional<core::Float> readValue() override;
  void setValues(std::vector<std::optional<core::Float>> values);

 private:
  std::vector<std::optional<core::Float>> values_;
};

}  // namespace hyped::utils
