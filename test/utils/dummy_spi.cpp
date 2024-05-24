#include <gtest/gtest.h>

#include "core/types.hpp"
#include <utils/dummy_spi.hpp>

namespace hyped::test {

TEST(DummySpi, write)
{
  utils::DummySpi dummy_spi;
  std::vector<core::Result> write_results = {core::Result::kSuccess};
  dummy_spi.setWriteResults(write_results);
  std::vector<std::uint8_t> tx = {0x01, 0x02, 0x03};
  core::Result result          = dummy_spi.write(0x01, tx);
  // Successfully sending data result in kSuccess being returned, and the sent data is saved in
  // sent_data_
  EXPECT_EQ(result, core::Result::kSuccess);
  const auto sent_data = dummy_spi.getSentData();
  EXPECT_EQ(sent_data.size(), 1);
  for (std::size_t i = 0; i < tx.size(); i++) {
    EXPECT_EQ(sent_data[0][i], tx[i]);
  }
}

TEST(DummySpi, WriteFailure)
{
  utils::DummySpi dummy_spi;
  std::vector<core::Result> write_results = {core::Result::kFailure};
  dummy_spi.setWriteResults(write_results);
  std::vector<std::uint8_t> tx = {0x01, 0x02, 0x03};
  core::Result result          = dummy_spi.write(0x01, tx);
  // Failing to send data result in kFailure being returned, and the sent data is not saved in
  // sent_data_
  EXPECT_EQ(result, core::Result::kFailure);
  const auto sent_data = dummy_spi.getSentData();
  EXPECT_EQ(sent_data.size(), 0);
}

TEST(DummySpi, read)
{
  utils::DummySpi dummy_spi;
  std::vector<std::optional<std::vector<std::uint8_t>>> read_results
    = {std::vector<std::uint8_t>{0x01, 0x02, 0x03}};
  dummy_spi.setReadResults(read_results);
  std::optional<std::vector<std::uint8_t>> result = dummy_spi.read(0x01, 3);
  // Successfully reading data result in the data being returned
  EXPECT_TRUE(result.has_value());
  const auto read_data = result.value();
  EXPECT_EQ(read_data.size(), 3);
  for (std::size_t i = 0; i < read_data.size(); i++) {
    EXPECT_EQ(read_data[i], read_results[0].value()[i]);
  }
}

TEST(DummySpi, readFailure)
{
  utils::DummySpi dummy_spi;
  std::vector<std::optional<std::vector<std::uint8_t>>> read_results = {std::nullopt};
  dummy_spi.setReadResults(read_results);
  std::optional<std::vector<std::uint8_t>> result = dummy_spi.read(0x01, 3);
  // Failing to read data result in std::nullopt being returned
  EXPECT_FALSE(result);
}

}  // namespace hyped::test
