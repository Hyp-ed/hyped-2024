import { Limits, sensorData, Readings } from '..';

export class Utilities {
  // Mathematical helper methods

  // ## For optimal timestep computation ## //

  /** Greatest common divisor */
  public static gcd(nums: number[]): number {
    // ensure numerical array is in descending order
    nums = nums.sort((a, b) => b - a);
    return nums.reduce((acc, c) => {
      return c === 0 ? acc : Utilities.gcd([c, acc % c]);
    });
  }

  /** Lowest common factor */
  public static lcm(nums: number[]): number {
    return nums.reduce((acc, c) => {
      return (acc * c) / Utilities.gcd(acc, c);
    });
  }

  /**
   * Gets the exponential average of a recent set of values
   * @param vals previous values (and chosen length of array)
   * @param alpha weighting factor
   * @returns exponentially weighted average
   */
  public expMovingAvg(vals: number[], alpha: number): number | undefined {
    if (alpha <= 0 || alpha > 1 || !vals.length) {
      console.log('Invalid parameters');
      return;
    }
    let sum = 0;
    vals.forEach((v, i) => {
      const weight = Math.pow(alpha, vals.length - 1 - i);
      sum += v * weight;
    });
    return sum / (1 - Math.pow(alpha, vals.length));
  }

  /**
   * Generates random noise value from a Gaussian distribution
   * @param mean self-explanatory
   * @param std_dev sensor's RMS noise value, used as the standard deviation
   * @returns a random number defined by the normal distribution of stdDev = RMS noise
   */
  public static gaussianRandom(std_dev: number, mean = 0): number {
    // Using the Box-Muller transform to generate random values from a normal distribution
    const u1 = Math.random();
    const u2 = Math.random();
    const z = Math.sqrt(-2 * Math.log(u1)) * Math.cos(2 * Math.PI * u2);

    return parseFloat((z * std_dev + mean).toFixed(2));
  }

  /**
   * Generates random value from random distribution defined by provided range
   * 99% probability of random value falling within critical limits
   * This results in a z-score of 2.576 for confidence level of 99%
   * Make use of gaussianRandom method to generate value, except this time the mean is the previous value,
   *   and the standard deviation is calculated from the z-score above
   * Adds noise to result */
  public static getRandomValue(
    prevValue: number,
    limits: Limits,
    rms_noise: number,
    format: 'float' | 'integer',
  ): number {
    // Assert that the measurement is from a physical sensor, not a derived measurement
    // (not important, commented out for now)
    // if (rms_noise != 0) {
    // const { high, low } = limits.critical;
    // const sttDev = (high + low) / 2; // for 99% confidence margin

    // const range = high - low;
    // Check if result should be integer (i.e. for keyence sensor)
    return format == 'float'
      ? parseFloat(this.gaussianRandom(rms_noise, prevValue).toFixed(2))
      : parseInt(this.gaussianRandom(rms_noise, prevValue).toFixed(2));
    // } else { return 0; } // don't randomise motion variables, they will be calculated from the accelerometers
  }

  /**
   * Logistic function used to estimate velocity as a function of time
   * @returns the current timestep's reading according to this analytical model
   */
  public static logistic(
    t: number,
    steadyState: number,
    growthRate: number,
    timeOfInflection: number,
  ): any {
    // Requires:
    // - setpoint for velocity (arbitrary, around 90-95% of max velocity)
    // - acceleration high limit (from sensorData)
    // - parameters of inflection time and growth height (chosen to ensure acceleration peaks at its max value)
    return steadyState / (1 + Math.exp(-growthRate * (t - timeOfInflection)));
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
  public static keyence(
    podLength: number,
    quantity: number,
    displacement: number,
    time: number,
  ): Readings {
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
      Object.keys(readings).map((key, i) => {
        // Calculate sensor offset from front of pod
        const sensorOffset = displacement - sensorRegion * i;
        return [key, Math.floor(sensorOffset) / 16];
      }),
    );

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
  public static levitate(
    t: number,
    velocity: number,
    thresholdVelocity: number,
  ): any {
    if (velocity < thresholdVelocity) {
      return 0;
    }
    // Requires:
    // - setpoint for levitation height
    // - current velocity to compare with threshold for levitation
    // - parameters of inflection time and growth height (chosen to ensure acceleration peaks at its max value)
    const setpoint =
      Object.values(sensorData.levitation_height.limits.critical).reduce(
        (acc, val) => acc + val,
      ) / 2;

    const initialMotion = (k, t_0) => this.logistic(t, setpoint + 10, k, t_0);

    const oscillation = () => {
      const amplitude = 10;
      const decayRate = 0.2;
      const frequency = 5; // Hz
    };
  }
}

// console.log(Utilities.lcm([50, 20, 100]));
// console.log(Utilities.lcm([5, 45, 500]));
// console.log(Utilities.lcm([500, 500, 600]));
// console.log('~');
// console.log(Utilities.gcd([50, 20, 100]));
// console.log(Utilities.gcd([5, 45, 500]));
// console.log(Utilities.gcd([500, 500, 600]));
// console.log(Utilities.gcd([30, 120, 45]));
