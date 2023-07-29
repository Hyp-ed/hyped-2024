#include "time_frequency_calculator.hpp"

#include <fstream>
#include <iostream>

#include <rapidjson/document.h>
#include <rapidjson/error/en.h>
#include <rapidjson/istreamwrapper.h>

namespace hyped::motors {

std::optional<std::shared_ptr<TimeFrequencyCalculator>> TimeFrequencyCalculator::create(
  core::ILogger &logger, const std::string &path)
{
  std::ifstream input_stream(path);
  if (!input_stream.is_open()) {
    logger.log(core::LogLevel::kFatal, "Failed to open file %s", path.c_str());
    return std::nullopt;
  }
  rapidjson::IStreamWrapper input_stream_wrapper(input_stream);
  rapidjson::Document document;
  rapidjson::ParseResult result = document.ParseStream(input_stream_wrapper);
  if (!result) {
    logger.log(core::LogLevel::kFatal,
               "Error parsing JSON: %s",
               rapidjson::GetParseError_En(document.GetParseError()));
    return std::nullopt;
  }
  std::vector<std::pair<std::uint64_t, std::uint32_t>> frequency_table;
  if (!document.HasMember("times")) {
    logger.log(core::LogLevel::kFatal,
               "Missing required field 'times' in configuration file at %s",
               path.c_str());
    return std::nullopt;
  }
  if (!document.HasMember("frequencies")) {
    logger.log(core::LogLevel::kFatal,
               "Missing required field 'frequencies' in configuration file at %s",
               path.c_str());
    return std::nullopt;
  }
  const auto times       = document["times"].GetArray();
  const auto frequencies = document["frequencies"].GetArray();
  for (rapidjson::SizeType i = 0; i < times.Size(); i++) {
    frequency_table.emplace_back(times[i].GetUint64(), frequencies[i].GetUint());
  }
  return std::make_shared<TimeFrequencyCalculator>(logger, frequency_table);
}

TimeFrequencyCalculator::TimeFrequencyCalculator(
  core::ILogger &logger, std::vector<std::pair<std::uint64_t, std::uint32_t>> &frequency_table)
    : logger_(logger),
      frequency_table_(frequency_table)
{
  start_time_ = std::chrono::system_clock::now();
}

std::uint32_t TimeFrequencyCalculator::calculateFrequency(core::Float velocity)
{
  const auto current_time = std::chrono::system_clock::now();
  const std::uint32_t nanoseconds_elapsed
    = std::chrono::duration_cast<std::chrono::nanoseconds>(current_time - start_time_).count();
  const auto frequency
    = std::find_if(frequency_table_.rbegin(),
                   frequency_table_.rend(),
                   [nanoseconds_elapsed](const std::pair<std::uint64_t, std::uint32_t> &pair) {
                     return nanoseconds_elapsed > pair.first;
                   })
        ->second;
  return frequency;
}

void TimeFrequencyCalculator::reset()
{
  start_time_ = std::chrono::system_clock::now();
}
}  // namespace hyped::motors
