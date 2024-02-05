import { Limits, LiveReading, SensorData, Readings } from "..";

export class Utilities {

    

    // ## MOTION ## //
    /**
     * Logistic function used to estimate velocity as a function of time
     * @returns the current timestep's reading according to this analytical model
     */
    public static logistic(t: number, setpoint: number, growthRate: number, timeOfInflection: number): any {
        // Requires: 
        // - setpoint for velocity (arbitrary, around 90-95% of max velocity)
        // - acceleration high limit (from sensorData)
        // - parameters of inflection time and growth height (chosen to ensure acceleration peaks at its max value)
    }


    // ## TEMPERATURE ## //



    // ## RESISTANCE ## //


    
    
    /** ## KEYENCE ## **
     * Integer value in range [0, 16], which directly corresponds to the pod displacement, which
     * has a range of [0m, 100m]. Every 16m, keyence increases by one (pole/stripe). Obviously,
     * this optical sensor has no random noise. Its graph will look like a staircase of varying 
     * width per step depending on the velocity.
     * 
     * It needs to know the displacement at this time t. No point calculating it again if it's already
     * been calculated, so we need to check and reference instances.motion.time.
    */
    public static keyence(podLength: number, quantity: number, displacement: number, time: number): Readings {
   
        time /= 1000; // convert to seconds
        // Sensors are evenly distributed along the pod
        // Displacement is measured at the nose of the pod
        //   So the keyence sensor readings each have a displacement lag of 1/numKeyences * podLength
        const sensorRegion = podLength / quantity;
        // Check if the displacement reading has been taken at this time step
        // If not, update the motion instance to get the current displacement
        if (!Sensor.isSampled['motion']) {
            super.update(t);
        }
        readings = Object.fromEntries(
            Object.keys(readings).map( (key, i) => {
                // Calculate sensor offset from front of pod
                const sensorOffset = displacement - (sensorRegion * i);
                return [key, Math.floor(sensorOffset) / 16]
            }
        ));

        return readings;
    }



    // ## PRESSURE ## //

    // ## LEVITATION & MAGNETISM ## //
    /**
     * When the electromagnets are powered on, the pod will follow an exponential curve to its setpoint,
     * then oscillate with decaying amplitude around its setpoint. This occurs every time the magnetic field
     * is altered. The magnetic field is controlled by the operator, so the input will remain fixed, however
     * changes in the magnetic field from the propulsion system will cause the hall effect sensors to register
     * fluctuations, not to mention sensor noise. However, this should not affect the levitation reading as the
     * current powering the magnetic field controlling the gap height remains constant (or user-controlled, at
     * least).
     * Magnetic force follows the inverse square law, and it is a repulsive force between the pod and track.
     * @param t 
     * @param sensor 
     */
    public static levitate(t: number, velocity: number, thresholdVelocity: number): any {
        if (velocity < thresholdVelocity) { return 0; }
        // Requires:
        // - setpoint for levitation height
        // - current velocity to compare with threshold for levitation
        // - parameters of inflection time and growth height (chosen to ensure acceleration peaks at its max value)
        const setpoint = Object.values(sensorData.levitation_height.limits.critical)
            .reduce((acc, val) => acc + val) / 2;

        const initialMotion = (k, t_0) => this.logistic(t, setpoint + 10, k, t_0);

        const oscillation = () => {
            const amplitude = 10;
            const decayRate = 0.2;
            const frequency = 5; // Hz
        }
    }



    // ## HELPER FUNCTIONS ## //

    /**
     * Sensor-indiscriminate menthod, dependent only on limiting range
     * Used when user selects random data generation option
     * Does not require class instantiation
     */ 
    public static getRandomValue(limits: Limits, rms_noise: number, format: 'float' | 'integer'): number {
        if (rms_noise != 0) {
            const { high, low } = limits.critical;
            // console.log(this._currentValue)
            const range = high - low;
            return (format == 'float'
            ? parseFloat((Math.random() * range + low).toFixed(2))
            : Math.floor(Math.random() * (range + 1)) + low
            ) + this.addRandomNoise(rms_noise);
        } else { return 0; } // don't randomise motion variables, they will be calculated from the accelerometers
      }

    /**
     * Generates a random noise value from a Gaussian distribution
     * This function will be called for each sensor of a given type, then averaged
     * @param mean self-explanatory
     * @param rms_noise sensor's RMS noise value, used as the standard deviation
     * @returns a random number defined by the normal distribution of stdDev = RMS noise
     */
    public static addRandomNoise(rms_noise: number, mean: number = 0): number {
        // Using the Box-Muller transform to generate random values from a normal distribution
        const u1 = Math.random();
        const u2 = Math.random();
        const z = Math.sqrt(-2 * Math.log(u1)) * Math.cos(2 * Math.PI * u2);
    
        return parseFloat((z * rms_noise + mean).toFixed(2));
    }

    /**
     * Gets the exponential average of a recent set of values
     * @param vals previous values (and chosen length of array)
     * @param alpha weighting factor
     * @returns exponentially weighted average
     */
    public expMovingAvg(vals: number[], alpha: number): number | undefined {
        if (alpha <= 0 || alpha > 1 || !vals.length) {
                console.log("Invalid parameters");
                return;
            }
        let sum = 0;
        vals.forEach( (v, i) => {
            const weight = Math.pow(alpha, vals.length - 1 - i)
            sum += v * weight;
        });
        return sum / (1 - Math.pow(alpha, vals.length));
    }

}