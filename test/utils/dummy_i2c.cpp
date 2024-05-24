#include <gtest/gtest.h>

#include <utils/dummy_i2c.hpp>

namespace hyped::test {

TEST(DummyI2c, write)
{
  utils::DummyI2c dummy_i2c;
  std::vector<core::Result> write_results = {core::Result::kSuccess};
  dummy_i2c.setWriteByteResults(write_results);
  const std::uint8_t tx = 0x01;
  core::Result result   = dummy_i2c.writeByte(0x01, tx);
  // Successfully sending data result in kSuccess being returned, and the sent data is saved in
  // sent_data_
  EXPECT_EQ(result, core::Result::kSuccess);
  const auto sent_data = dummy_i2c.getSentData();
  EXPECT_EQ(sent_data.size(), 1);
  EXPECT_EQ(sent_data[0].first, 0x01);
  EXPECT_EQ(sent_data[0].second, tx);
}

TEST(DummyI2c, WriteFailure)
{
  utils::DummyI2c dummy_i2c;
  std::vector<core::Result> write_results = {core::Result::kFailure};
  dummy_i2c.setWriteByteResults(write_results);
  const std::uint8_t tx = 0x01;
  core::Result result   = dummy_i2c.writeByte(0x01, tx);
  // Failing to send data result in kFailure being returned, and the sent data is not saved in
  // sent_data_
  EXPECT_EQ(result, core::Result::kFailure);
  const auto sent_data = dummy_i2c.getSentData();
  EXPECT_EQ(sent_data.size(), 0);
}

TEST(DummyI2c, read)
{
  utils::DummyI2c dummy_i2c;
  std::vector<std::optional<std::uint8_t>> read_results = {0x01};
  dummy_i2c.setReadByteResults(read_results);
  std::optional<std::uint8_t> result = dummy_i2c.readByte(0x01, 0x01);
  // Successfully reading data result in the data being returned
  EXPECT_TRUE(result.has_value());
  EXPECT_EQ(result.value(), read_results[0].value());
}

TEST(DummyI2c, readFailure)
{
  utils::DummyI2c dummy_i2c;
  std::vector<std::optional<std::uint8_t>> read_results = {std::nullopt};
  dummy_i2c.setReadByteResults(read_results);
  std::optional<std::uint8_t> result = dummy_i2c.readByte(0x01, 0x01);
  // Failing to read data result in std::nullopt being returned
  EXPECT_FALSE(result.has_value());
}

}  // namespace hyped::test
