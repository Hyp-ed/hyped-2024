#include "dummy_spi.hpp"

namespace hyped::utils {

core::Result DummySpi::read(const std::uint8_t addr,
                            const std::uint8_t *rx,
                            const std::uint16_t len)
{
  return core::Result::kFailure;
}

core::Result DummySpi::write(const std::uint8_t addr,
                             const std::uint8_t *tx,
                             const std::uint16_t len)
{
  return core::Result::kFailure;
}

}  // namespace hyped::utils