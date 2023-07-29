#pragma once

#include <cstdint>

#include <core/types.hpp>

namespace hyped::io {

class ISpi {
 public:
  /**
   * @brief Get data from sensor, starting at some address.
   * @param addr  - register from which the reading should start
   * @param rx    - pointer to head of read buffer
   * @param len   - number of bytes to be read, i.e. size of the read buffer
   */
  virtual core::Result read(const std::uint8_t addr,
                            const std::uint8_t *rx,
                            const std::uint16_t len)
    = 0;

  /**
   * @brief Write data to sensor, starting at some address.
   * @param addr  - register from which writing to starts
   * @param tx    - pointer to head of write buffer
   * @param len   - number of bytes to be written, i.e. size of the write buffer
   */
  virtual core::Result write(const std::uint8_t addr,
                             const std::uint8_t *tx,
                             const std::uint16_t len)
    = 0;
};

}  // namespace hyped::io
