import { Sensor, LiveReading, Readings } from '../src';
import { Motion } from '.';

export class Keyence extends Motion {
    
    private podLength: number = 2.5; // metres
    
    constructor(keyence: LiveReading) {
        super(keyence);
    }

    /**
     * Integer value in range [0, 16], which directly corresponds to the pod displacement, which
     * has a range of [0m, 100m]. Every 16m, keyence increases by one (pole/stripe). Obviously,
     * this optical sensor has no random noise. Its graph will look like a staircase of varying 
     * width per step depending on the velocity.
     * 
     * It needs to know the displacement at this time t. No point calculating it again if it's already
     * been calculated, so we need to check and reference instances.motion.time.
    */
    update(t: number): Readings {
        this.time = t / 1000;
        // Sensors are evenly distributed along the pod
        // Displacement is measured at the nose of the pod
        //   So the keyence sensor readings each have a displacement lag of 1/numKeyences * podLength
        const sensorRegion = this.podLength / this.quantity;
        // Check if the displacement reading has been taken at this time step
        // If not, update the motion instance to get the current displacement
        if (!Sensor.isSampled['motion']) {
            super.update(t);
        }
        this.readings = Object.fromEntries(
            Object.keys(this.readings).map( (key, i) => {
                // Calculate sensor offset from front of pod
                const sensorOffset = this.displacement - (sensorRegion * i);
                return [key, Math.floor(sensorOffset) / 16]
            }
        ));

        return this.readings;
    }
}