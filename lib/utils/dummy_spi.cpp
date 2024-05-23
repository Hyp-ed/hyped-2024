#include "dummy_spi.hpp"

namespace hyped::utils {

std::optional<std::vector<std::uint8_t>> DummySpi::read(const std::uint8_t addr,
                                                        const std::uint16_t len)
{
  return std::nullopt;
}

core::Result DummySpi::write(const std::uint8_t addr, std::vector<std::uint8_t> tx)
{
  return core::Result::kFailure;
}

}  // namespace hyped::utils
