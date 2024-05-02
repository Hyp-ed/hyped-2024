#include "dummy_adc.hpp"

#include <optional>

namespace hyped::utils {

DummyAdc::DummyAdc() : values_(0.0)
{
}

std::optional<core::Float> DummyAdc::readValue()
{
  if (values_.empty()) return std::nullopt;
  const auto val = values_.front();
  values_.erase(values_.begin());
  return val;
}

void DummyAdc::setValue(std::vector<std::optional<core::Float>> values)
{
  values_ = values;
}

}  // namespace hyped::utils
