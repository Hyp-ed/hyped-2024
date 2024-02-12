#pragma once

#include "core/types.hpp"

namespace hyped::motors {
class VectorController {
 private:
  core::Float Ia;
  core::Float Ib;
  core::Float Ic;
  core::Float rotorVelocity;
  core::Float Id;
  core::Float Iq;
  core::Float Vd;
  core::Float Vq;
  core::Float IdRef;
  core::Float IqRef;
  core::Float Kp;
  core::Float Ki;
  core::Float theta;

 public:
  VectorController();

  ~VectorController();

  void measureCurrentsAndVelocity(core::Float ia, core::Float ib, core::Float ic, core::Float rotorVelocity);
  void convertTo2Axis();
  void inverse2Axis();
  void piControl();
  void inverseParkTransform();
    };
}