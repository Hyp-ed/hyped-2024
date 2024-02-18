import { Sensor } from '../baseSensor';
import { Motion } from './motion';
import { Readings, LiveReading, trackLength } from '../index';

/**
 * Integer value in range [0, 16], which directly corresponds to the pod displacement, which
 * has a range of [0m, 100m]. Every 16m, keyence increases by one (pole/stripe). Obviously,
 * this optical sensor has no random noise. Its graph will look like a staircase of varying
 * width per step depending on the velocity.
 *
 * It needs to know the displacement at this time t. No point calculating it again if it's already
 * been calculated, so we need to check and reference instances.motion.time.
 */
export class Keyence extends Motion {
  private podLength = 2.5;

  constructor(data: LiveReading) {
    super(data);
  }

  getData(t: number): Readings {
    // Sensors are evenly distributed along the pod
    // Displacement is measured at the nose of the pod
    // So the keyence sensor readings each have a displacement lag of 1/numKeyences * podLength
    // Subtract 1 as there are n-1 gaps between n sensors placed end to end with rest equally spaced between
    const sensorRegion = this.podLength / (this.quantity - 1);
    const noPoles = this.limits.critical.high;

    // Check if the displacement reading has been taken at this time step
    // If not, update the motion instance to get the current displacement
    //   and set the isSampled flag to true so other sensors in this iteration can
    //   immediately extract the calculated displacement value
    if (!Sensor.isSampled['motion']) {
      this.displacement = super.getData(t).displacement;
      Sensor.isSampled['motion'] = true;
    } else {
      this.displacement = Sensor.lastReadings['motion'].displacement;
    }

    const readings = Object.keys(Sensor.lastReadings.keyence).map((key, i) => {
      // Calculate sensor displacement
      // Return value cannot be negative as sensor counts physical poles/stripes
      const relDisp =
        this.displacement - sensorRegion * i >= 0
          ? this.displacement - sensorRegion * i
          : 0;
      return [key, Math.floor(relDisp * (noPoles / trackLength))];
    });
    // return readings in expected format
    return Object.fromEntries(readings);
  }
}
