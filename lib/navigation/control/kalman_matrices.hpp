#pragma once

#include <array>
#include <cstdint>
#include <optional>

#include <Eigen/Dense>
#include <core/types.hpp>


namespace hyped::navigation {

constexpr std::size_t state_dimension       = 2;
constexpr std::size_t control_dimension     = 1;
constexpr std::size_t measurement_dimension = 2;
const core::Float kDeltaT = 0.01;


//Constant matrices
using StateTransitionMatrix = Eigen::Matrix<core::Float, state_dimension, state_dimension>;
inline const StateTransitionMatrix kStateTransitionMatrix = [] {
    StateTransitionMatrix m;
    m << 1, kDeltaT,
          0, 1;
    return m;
}();

using ControlMatrix = Eigen::Matrix<core::Float, state_dimension, control_dimension>;
inline const ControlMatrix kControlMatrix = [] {
    ControlMatrix m;
    m << 0.5 * kDeltaT * kDeltaT,
         kDeltaT;
    return m;
}();                                                    
                               
using ErrorCovarianceMatrix = Eigen::Matrix<core::Float, state_dimension, state_dimension>;
inline const ErrorCovarianceMatrix kErrorCovarianceMatrix = [] {
    ErrorCovarianceMatrix m;
    m << 1, 0, //TODO: tune these  
         0, 1;
    return m;
}();

using MeasurementMatrix
  = Eigen::Matrix<core::Float, measurement_dimension, state_dimension>;
//TODO: could be optimised by changing 1,1 element only
inline const MeasurementMatrix kMeasurementMatrix_optical_only = [] {
    MeasurementMatrix m;
    m << 0, 0,
         0, kDeltaT;
    return m;
}();
inline const MeasurementMatrix kMeasurementMatrix_both = [] {
    MeasurementMatrix m;
    m << 1, 0,
         0, kDeltaT;
    return m;
}();

using MeasurementNoiseCovarianceMatrix
  = Eigen::Matrix<core::Float, measurement_dimension, measurement_dimension>;
inline const MeasurementMatrix kMeasurementMatrix_optical_only_2 = [] {
    MeasurementMatrix m;
    m << 0, 0,
         0, 1;
    return m;
}();

//Changing values
using StateVector = Eigen::Matrix<core::Float, state_dimension, 1>;
using StateTransitionCovarianceMatrix
  = Eigen::Matrix<core::Float, state_dimension, state_dimension>;
using ControlInput = Eigen::Matrix<core::Float, control_dimension, 1>;
using MeasurementVector = Eigen::Matrix<core::Float, measurement_dimension, 1>;

}  // namespace hyped::navigation