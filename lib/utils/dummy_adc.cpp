#include "dummy_adc.hpp"

#include <optional>
#include <utility>

namespace hyped::utils {

std::optional<core::Float> DummyAdc::readValue()
{
  if (values_.empty()) { return std::nullopt; }
  const auto val = values_.front();
  values_.erase(values_.begin());
  return val;
}

void DummyAdc::setValues(std::vector<std::optional<core::Float>> values)
{
  values_ = std::move(values);
}

}  // namespace hyped::utils
