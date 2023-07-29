#include <cmath>

#include <iostream>

#include <gtest/gtest.h>

#include <core/logger.hpp>
#include <core/types.hpp>
#include <navigation/preprocessing/preprocess_encoders.hpp>
#include <utils/manual_time.hpp>

namespace hyped::test {

bool checkArrayEquality(core::EncoderData &encoder_data_a, core::EncoderData &encoder_data_b)
{
  if (encoder_data_a.size() != encoder_data_b.size()) { return false; }
  for (std::size_t i = 0; i < encoder_data_a.size(); ++i) {
    if (std::fabs(encoder_data_a.at(i) - encoder_data_b.at(i)) > core::kEpsilon) { return false; }
  }
  return true;
}

void test()
{
  utils::ManualTime manual_time;
  core::Logger logger("test", core::LogLevel::kFatal, manual_time);
  navigation::EncodersPreprocessor encoder_processer(logger);
}

TEST(Encoder, equal_data)
{
  utils::ManualTime manual_time;
  core::Logger logger("test", core::LogLevel::kFatal, manual_time);
  navigation::EncodersPreprocessor encoder_processer(logger);
  const core::EncoderData data = {1, 1, 1, 1};
  core::EncoderData answer     = {1, 1, 1, 1};
  auto final_data              = encoder_processer.processData(data);
  ASSERT_TRUE(checkArrayEquality(*final_data, answer));
}

TEST(Encoder, not_equal_data)
{
  utils::ManualTime manual_time;
  core::Logger logger("test", core::LogLevel::kFatal, manual_time);
  navigation::EncodersPreprocessor encoder_processer(logger);
  const core::EncoderData data = {6, 1, 1, 1};
  core::EncoderData answer     = {1, 1, 1, 1};
  auto final_data              = encoder_processer.processData(data);
  ASSERT_TRUE(checkArrayEquality(*final_data, answer));
}

TEST(Encoder, one_unreliable_sensor)
{
  utils::ManualTime manual_time;
  core::Logger logger("test", core::LogLevel::kFatal, manual_time);
  navigation::EncodersPreprocessor encoder_processer(logger);
  const core::EncoderData data = {6, 1, 1, 1};
  for (std::size_t i; i < 12; ++i) {
    encoder_processer.processData(data);
  }
  core::EncoderData answer = {1, 1, 1, 1};
  auto final_data          = encoder_processer.processData(data);
  ASSERT_TRUE(checkArrayEquality(*final_data, answer));
}

}  // namespace hyped::test