#pragma once

#include <cstdint>
#include <optional>
#include <vector>

#include <core/types.hpp>

namespace hyped::io {

class ISpi {
 public:
  /**
   * @brief Get data from sensor, starting at some address.
   * @param addr  - register from which the reading should start
   * @param len   - number of bytes to be read
   */
  virtual std::optional<std::vector<std::uint8_t>> read(const std::uint8_t addr,
                                                        const std::uint16_t len)
    = 0;

  /**
   * @brief Write data to sensor, starting at some address.
   * @param addr  - register from which writing to starts
   * @param tx    - data to be written
   */
  virtual core::Result write(const std::uint8_t addr, const std::vector<std::uint8_t> tx) = 0;
};

}  // namespace hyped::io
