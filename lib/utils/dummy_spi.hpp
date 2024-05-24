#pragma once

#include <io/spi.hpp>

namespace hyped::utils {

/**
 * Provides a basic implementation of the ISpi interface.
 * This implementation does not actually communicate with any hardware.
 * It is intended to be used in unit tests.
 */
class DummySpi : public io::ISpi {
 public:
  std::optional<std::vector<std::uint8_t>> read(const std::uint8_t addr,
                                                const std::uint16_t len) override;
  core::Result write(const std::uint8_t addr, const std::vector<std::uint8_t> tx) override;
};

}  // namespace hyped::utils
