#include "dummy_spi.hpp"

#include <utility>

namespace hyped::utils {

std::optional<std::vector<std::uint8_t>> DummySpi::read(const std::uint8_t addr,
                                                        const std::uint16_t len)
{
  if (read_results_.empty()) { return std::nullopt; }
  const auto result = read_results_.front();
  read_results_.erase(read_results_.begin());
  return result;
}

core::Result DummySpi::write(const std::uint8_t addr, std::vector<std::uint8_t> tx)
{
  if (write_results_.empty()) { return core::Result::kFailure; }
  const auto result = write_results_.front();
  write_results_.erase(write_results_.begin());
  if (result == core::Result::kSuccess) { sent_data_.push_back(tx); }
  return result;
}

void DummySpi::setReadResults(std::vector<std::optional<std::vector<std::uint8_t>>> results)
{
  read_results_ = std::move(results);
}

void DummySpi::setWriteResults(std::vector<core::Result> results)
{
  write_results_ = std::move(results);
}
std::vector<std::vector<std::uint8_t>> DummySpi::getSentData()
{
  return sent_data_;
}

}  // namespace hyped::utils
