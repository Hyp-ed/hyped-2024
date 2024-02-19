import { Sensor } from '../baseSensor';
import { Magnetism } from './magnetism';
import { LiveReading, Readings, utils } from '../index';

export class Levitation extends Magnetism {

  private readonly logParams = {
    t_0: 3.2, // logistic time of inflection point
    growth: 2.5, // growth rate
    t_f: 3.5, // time when logistic function switches to sinusoidal exp. decay
    l_peak: 60 // peak amplitude
  }
  private decayParams: {
    freq: -4, // rad/s
    phase: 9, // phase angle (rad)
    decay: 0.2, // decay rate
    amp: 10 // peak - steady state
  }
  private levitation: number;
  private isSteadyState: boolean = false;

  constructor(data: LiveReading) {
    super(data);
  }

  getData(t: number): Readings {
    console.log('\nlast lev:', Sensor.lastReadings.levitation);
    console.log('\nforce on?:', this.isFieldOn());
    const fieldOn = this.isFieldOn();

    if (!Sensor.isSampled['magnetism']) {
      this.displacement = super.getData(t).displacement;
      Sensor.isSampled['motion'] = true;
    } else {
      this.displacement = Sensor.lastReadings.motion.displacement;
    }

    if (!fieldOn && Object.values(Sensor.lastReadings.levitation).every(
      (val: number) => val == 0
    )) {
      return Sensor.lastReadings.levitation;
    }

    if (t <= this.logParams.t_f) {
      this.levitation = utils.logistic(
        t,
        this.logParams.l_peak,
        this.logParams.growth,
        this.logParams.t_0
      );
    }

    if (t > this.logParams.t_f) {
      this.levitation = this.levSetpoint + utils.oscillateDecay(
        t - this.logParams.t_f, // start time for oscillation
        this.decayParams.freq,
        this.decayParams.phase,
        this.decayParams.decay,
        this.decayParams.amp
      )
    }

    return Object.fromEntries(Object.keys(
      Sensor.lastReadings.levitation).map(sensor => {
        return [sensor, this.levitation + utils.gaussianRandom(this.rms_noise)]
      })
    );

    // test this works first then add logic to connect setpoints to magnetism so it can react


    // if electromagnets are powered on, levitation follows an exponential
    //   growth curve to a setpoint, overshoots it a little, then oscillates
    //   before quickly reaching steady state and maintaining a constant value
    //   with noise-caused fluctuations

    // create this new function in utils. It's an exp(kt) until it reaches the setpoint, then it oscillates
    // then (if lev >= levSetpoint) it changes to the sinusoidal decay. There should be
    // a saved graph on desmos with the parameters.

  }
}
