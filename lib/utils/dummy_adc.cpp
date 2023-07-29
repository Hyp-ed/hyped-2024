#include "dummy_adc.hpp"

namespace hyped::utils {

std::optional<core::Float> DummyAdc::readValue()
{
  return std::nullopt;
}

}  // namespace hyped::utils