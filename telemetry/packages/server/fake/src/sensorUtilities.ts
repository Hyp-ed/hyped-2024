// No imports, this file contains pure, general functions

export class Utilities {
  /** Greatest common divisor */
  public static gcd(nums: number[]): number {
    // ensure numerical array is in descending order
    nums = nums.sort((a, b) => b - a);
    return nums.reduce((acc, c) => {
      return c === 0 ? acc : Utilities.gcd([c, acc % c]);
    });
  }

  /**
   * Simple floating point rounding method
   * @param num
   * @returns
   */
  public static round2DP(num: number): number {
    return parseFloat(num.toFixed(2));
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
    rms_noise: number,
    format: 'float' | 'integer',
  ): number {
    // Check numerical format (i.e. keyence sensor is an integer measurement)
    return format == 'float'
      ? parseFloat(this.gaussianRandom(rms_noise, prevValue).toFixed(2))
      : parseInt(this.gaussianRandom(rms_noise, prevValue).toFixed(2));
  }

  /**
   * Simple averages, used to average readings from a set of similar sensors
   * @param values numbers to average
   * @returns arithmetic average
   */
  public static average(values: number[]): number {
    return values.reduce((acc, c) => acc + c) / values.length;
  }

  /**
   * Logistic function used as an analytical basis for certain sensor variables' behaviour over time
   * @returns the current timestep's reading according to this analytical model
   */
  // Requires:
  // - setpoint for velocity (arbitrary, around 90-95% of max velocity)
  // - acceleration high limit (from sensorData)
  // - parameters of inflection time and growth height (chosen to ensure acceleration peaks at its max value)
  public static logistic(
    t: number,
    steadyState: number, // around 95% of max velocity giving a 5% margin for noise fluctuations
    growthRate: number, // exponential growth rate factor
    timeOfInflection: number, // time at which second derivative reaches a stationary point
  ): number {
    // round result for legibility
    return parseFloat(
      (
        steadyState /
        (1 + Math.exp(-growthRate * (t - timeOfInflection)))
      ).toFixed(2),
    );
  }

  public static oscillateDecay(
    t: number,
    freq: number,
    phase: number,
    decay: number,
    amp: number,
  ): number {
    return amp * Math.exp(-decay * t) * Math.cos(freq * t + phase);
  }
}
