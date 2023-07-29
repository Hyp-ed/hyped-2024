#pragma once

#include "core/logger.hpp"
#include "core/types.hpp"
#include <navigation/control/consts.hpp>

namespace hyped::navigation {

enum class KeyenceDataStatus { kAgreed = 0, kDisagreed };

class KeyencePreprocessor {
 public:
  /**
   * @brief Construct a new Keyence Preprocessor object
   *
   * @param log_: Navigation logger
   */
  KeyencePreprocessor(core::ILogger &log_);

  /**
   * @brief Checks that keyences have not disagreed twice in a row.
   *
   * @param keyence_data: inputs from Keyence Sensors
   * @return SensorChecks: enum indicating if Keyence Sensors have failed
   */
  SensorChecks checkKeyenceAgrees(const core::KeyenceData &keyence_data);
  /**
   *
   */
 private:
  core::ILogger &log_;
  KeyenceDataStatus previous_data_status_;
};

}  // namespace hyped::navigation