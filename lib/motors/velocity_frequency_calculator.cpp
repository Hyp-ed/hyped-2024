#include "velocity_frequency_calculator.hpp"

#include <fstream>
#include <sstream>

#include <rapidjson/document.h>
#include <rapidjson/error/en.h>
#include <rapidjson/istreamwrapper.h>

namespace hyped::motors {
std::optional<std::shared_ptr<VelocityFrequencyCalculator>> VelocityFrequencyCalculator::create(
  core::ILogger &logger, const std::string &coefficient_file_path)
{
  std::ifstream input_stream(coefficient_file_path);
  if (!input_stream.is_open()) {
    logger.log(core::LogLevel::kFatal, "Failed to open file %s", coefficient_file_path.c_str());
    return std::nullopt;
  }
  rapidjson::IStreamWrapper input_stream_wrapper(input_stream);
  rapidjson::Document document;
  const rapidjson::ParseResult result = document.ParseStream(input_stream_wrapper);
  if (!result) {
    logger.log(core::LogLevel::kFatal,
               "Error parsing JSON: %s",
               rapidjson::GetParseError_En(document.GetParseError()));
    return std::nullopt;
  }
  if (!document.HasMember("coefficients")) {
    logger.log(core::LogLevel::kFatal, "Error parsing JSON: missing coefficients");
    return std::nullopt;
  }
  const auto coefficients = document["coefficients"].GetArray();
  if (coefficients.Size() != 5) {
    logger.log(core::LogLevel::kFatal,
               "Error parsing JSON: expected 5 coefficients, got %d",
               coefficients.Size());
    return std::nullopt;
  }
  std::array<core::Float, 5> coefficients_array;
  for (std::size_t i = 0; i < coefficients.Size(); i++) {
    coefficients_array[i] = coefficients[i].GetFloat();
  }
  return std::optional<std::shared_ptr<VelocityFrequencyCalculator>>();
}

VelocityFrequencyCalculator::VelocityFrequencyCalculator(core::ILogger &logger,
                                                         std::array<core::Float, 5> &coefficients)
    : logger_(logger),
      coefficients_(coefficients)
{
}

std::uint32_t VelocityFrequencyCalculator::calculateFrequency(core::Float velocity)
{
  const core::Float frequency = velocity * velocity * velocity * velocity * coefficients_[0]
                                + velocity * velocity * velocity * coefficients_[1]
                                + velocity * velocity * coefficients_[2]
                                + velocity * coefficients_[3] + coefficients_[4];
  return frequency;
}
}  // namespace hyped::motors
