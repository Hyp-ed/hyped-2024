import { Sensor, LiveReading, Readings } from '../src';
import { Motion } from '.';

export class Magnetism extends Motion {

   protected readonly gapHeightSetpoint: number;


   constructor(hall_effect: LiveReading) {
      console.log('Magnetism constructor');
      console.log(hall_effect);
      super(hall_effect);
   }
}

/**
* Sensor used to monitor gap height. Magnetic field strength follows the inverse 
  square law to distance.
* The controllable magnetic field is the input and the gap height is the output.
* Magnetic field caused by LIM can cause interference! This makes it more interesting. As the
  pod accelerates, the propulsive field strength increases and increases the reading on the
  HE sensor, interfering with its function. Therefore the HR setpoint relating to the gap height
  must be dynamically controlled as speed changes.
* Say at speed v1, HE setpoint for lev setpoint is 5A. Then speed doubles to 2v1. This will cause
  an increase in the hall effect reading if there's no magnetic shield or protective orientation.
* The lower the change in speed (the smaller the acceleration), the more time the HE has to self-correct.
* As the function of the levitation control is to maintain the link between the HE and TOF sensor
  setpoints, the pod will begin to levitate following a roughly exponential curve, starting at a 
  slow rate of change.
* The control system will adjust for this as quickly as possible and increase the relative HE setpoint
  to keep the gap height as it was.
* HE units are amperes, and let's assume that current is roughly linear to mag field strength, so it also
  follows the inverse square law.
* HE needs to know the variables of velocity and acceleration, and the desired lev setpoint (const),
  hence it inherits from Motion, and Levitation inherits from HE as it is the output of the HE's input
* For the variations with speed, the HE reading variance should be relative small compared to its range,
  as it will be situated so as to limit interference. Use a small coefficient * acceleration.
*/