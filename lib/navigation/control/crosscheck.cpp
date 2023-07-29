#include "crosscheck.hpp"

#include <cmath>

namespace hyped::navigation {

Crosscheck::Crosscheck(core::ILogger &logger, const core::ITimeSource &time)
    : time_(time),
      logger_(logger)
{
}

SensorChecks Crosscheck::checkTrajectoryAgreement(const core::Float acceleration_displacement,
                                                  const core::Float encoder_displacement,
                                                  const core::Float keyence_displacement)
{
  // check encoders vs accelerometers
  SensorChecks encoder_accelerometer_check
    = checkEncoderAccelerometer(acceleration_displacement, encoder_displacement);
  if (encoder_accelerometer_check == SensorChecks::kUnacceptable) {
    logger_.log(core::LogLevel::kFatal,
                "Large disagreement between encoders and accelerometers. Trajectory cannot be "
                "accurately calculated.");
    return SensorChecks::kUnacceptable;
  }

  // check encoders vs keyence (if we're using keyence)
  if (kIsKeyenceActive) {
    SensorChecks encoder_keyence_check
      = checkEncoderKeyence(encoder_displacement, keyence_displacement);
    if (encoder_keyence_check == SensorChecks::kUnacceptable) {
      logger_.log(
        core::LogLevel::kFatal,
        "Large disagreement between encoders and keyence. Trajectory cannot be accurately "
        "calculated.");
      return SensorChecks::kUnacceptable;
    }
  }

  logger_.log(core::LogLevel::kInfo, "Trajectory values successfully verified");

  return SensorChecks::kAcceptable;
}

SensorChecks Crosscheck::checkEncoderAccelerometer(const core::Float acceleration_displacement,
                                                   const core::Float encoder_displacement)
{
  // check values are within tolerance
  const core::Float difference = acceleration_displacement - encoder_displacement;
  if (std::abs(difference) > kMaxAllowedAccelerometerEncoderDifference) {
    logger_.log(core::LogLevel::kFatal,
                "Disagreement between accelerometer and wheel encoders too large");
    return SensorChecks::kUnacceptable;
  }
  return SensorChecks::kAcceptable;
}

SensorChecks Crosscheck::checkEncoderKeyence(const core::Float encoder_displacement,
                                             const core::Float keyence_displacement)
{
  const core::Float difference = encoder_displacement - keyence_displacement;
  if (std::abs(difference) > kMaxAllowedKeyenceEncoderDifference) {
    logger_.log(core::LogLevel::kFatal,
                "Disagreement between keyence and wheel encoders too large");
    return SensorChecks::kUnacceptable;
  }
  return SensorChecks::kAcceptable;
}
}  // namespace hyped::navigation