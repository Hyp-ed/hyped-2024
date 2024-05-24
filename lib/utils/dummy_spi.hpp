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

  void setReadResults(std::vector<std::optional<std::vector<std::uint8_t>>> results);
  void setWriteResults(std::vector<core::Result> results);
  std::vector<std::vector<std::uint8_t>> getSentData();

 private:
  std::vector<std::optional<std::vector<std::uint8_t>>> read_results_;
  std::vector<core::Result> write_results_;
  std::vector<std::vector<std::uint8_t>> sent_data_;
};

}  // namespace hyped::utils
