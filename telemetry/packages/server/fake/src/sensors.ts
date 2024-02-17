import { Readings, LiveReading, Limits, utils } from './index';

export abstract class Sensor {
  // Records the actual time each sensor should be sampled next
  public static nextSamplingTimes: Record<string, number>;
  // Records whether each sensor has been sampled at the current time
  public static isSampled: Record<string, boolean>;
  protected static globalReadings: Record<string, Readings> = {}; // stores most recent sensor readings for all sensors

  // Sensor properties
  readonly type: string; // sensor type (same as the name of the object in sensorData)
  // readonly key: string; // sensor id, but this not as a top-level property, instead the readings all have unique keys
  readonly format: 'float' | 'integer'; // for random ternary logic (keyence is integer, rest are float)
  // readonly unit: string; // if we want to display units on UI, probably not necessary as front end will already do this
  readonly limits: Limits;
  readonly rms_noise: number;
  readonly sampling_time: number;
  readonly quantity: number;
  // readonly quantity: number; // uncomment if needed somewhere like averaging

  // Variable sensor data
  protected time: number; // current time in seconds
  protected _activated: boolean = false; // flag to indicate whether the sensor has been sampled at the current time

  // Extract relevant properties from sensor data entries
  constructor({
    type,
    format,
    limits,
    rms_noise,
    sampling_time,
    readings,
    quantity,
  }: LiveReading) {
    Object.assign(this, {
      type,
      format,
      limits,
      rms_noise,
      sampling_time,
      quantity,
    });
    this.time = 0;
    console.log('readings:', readings);
    // Add initial sensor values to global readings object
    Sensor.globalReadings[this.type] = readings;
  }

  /**
   * Main data gen method shared by all sensors
   * Returns the Readings object, filtered into the values to be published with MQTT
   * For motion, only acceleration, velocitity and displacement are uploaded
   * Accelerometers are used to estimate readings, then these values are
   *   propagated to the three variables above
   * @param t
   */
  abstract getData(t: number): Readings;
  // { ... this.update(newReadings) , return; }

  /*
    
    Each instance, will run getData() generate values for their respective measurements
    Then, this.update will run after generating a value in order to publish data to MQTT

    */

  // Update time and set isSampled flag to true for reference by interested sensor classes
  protected update(newReadings: Readings): void {
    // Set flag to true to indicate that the sensor has been sampled at the current time
    this.activated = true;
    // Update readings with new values
    Sensor.globalReadings[this.type] = newReadings;

    // Only upload to server, and update time and state properties if the sensor was updated
    //   directly, i.e. not as a prerequisite for another sensor's data generatinon calculations
    // Other sensors may depend on the motion sensors, but if the motion sensors
    //   have not reached their next sampling time, this calculation should not be recorded

    // isSampled and readings are set to true by default so other dependent sensors can access
    //   the new values without having to update the parent sensor method again needlessly
    if (this.sampling_time % this.sampling_time == 0) {
      this.time += this.sampling_time;

      // update global sensor state properties
      Sensor.nextSamplingTimes[this.type] += this.sampling_time;
      Sensor.isSampled[this.type] = this.activated;

      // publish to server
      // MQTT.publish(this.readings);
    }
  }

  protected getRandomData(prevValue: number, readings: Readings): Readings {
    for (const unit in readings) {
      readings[unit] = Utilities.getRandomValue(
        prevValue,
        this.limits,
        this.rms_noise,
        this.format,
      );
    }
    return readings;
  }

  set activated(newState: boolean) {
    this._activated = newState;
  }

  protected sensorisSampled(sensor: string) {
    return Sensor.isSampled[sensor];
  }
}

class Motion extends Sensor {
  protected velocity: number;
  protected acceleration: number;

  constructor(accelerometer: LiveReading) {
    super(accelerometer);
    // record the velocity and acceleration for ease of access by dependent subclasses
    this.acceleration = accelerometer.readings.acceleration;
    this.velocity = accelerometer.readings.velocity;
  }

  /**
   * Gets current reading from each accelerometer and calculates the other motion variables
   * Estimates velocity-time graph as a logistic function
   * @param t current time in seconds, used to calculate motion variables
   * @returns updated readings object containing acceleratio, velocity and displacement
   */
  getData(t: number): Readings {
    const velocityEstimate = Utilities.logistic(
      t,
      Sensor.globalReadings.motion.velocity,
      0.4, // exponential growth rate factor
      12.5, // time of inflection that ensures acceleration peaks at its limiting operating value
    );

    let accelerometerReading =
      (velocityEstimate - Sensor.globalReadings.motion.velocity) /
      this.sampling_time;
    // assert reading is not above critical limit
    accelerometerReading =
      accelerometerReading >= this.limits.critical.high
        ? this.limits.critical.high
        : accelerometerReading;
    accelerometerReading += Utilities.gaussianRandom(this.rms_noise);

    return {
      acceleration: accelerometerReading,
      velocity: (Sensor.globalReadings.motion.velocity +=
        accelerometerReading * this.sampling_time),
      displacement: (Sensor.globalReadings.motion.displacement +=
        Sensor.globalReadings.motion.velocity * this.sampling_time),
    };
  }
}

class Keyence extends Sensor {
  constructor(data: LiveReading) {
    super(data);
  }

  getData(t: number): Readings {
    const newReadings = { ...Sensor.globalReadings.keyence };

    Object.entries(Sensor.globalReadings.keyence).forEach(([key, val]) => {
      const polarity = Math.random() * 2 - 1;
      newReadings.key += 5 * polarity;
      if (
        newReadings[key] >= this.limits.critical.high ||
        newReadings[key] <= this.limits.critical.low
      ) {
        newReadings[key] *= -1;
      }
    });
    return newReadings;
  }
}

class Temperature extends Sensor {
  constructor(data: LiveReading) {
    super(data);
  }

  getData(t: number): Readings {}
}

class Resistance extends Sensor {
  constructor(data: LiveReading) {
    super(data);
  }

  getData(t: number): Readings {}
}

class Pressure extends Sensor {
  constructor(data: LiveReading) {
    super(data);
  }

  getData(t: number): Readings {}
}

class Magnetism extends Sensor {
  constructor(data: LiveReading) {
    super(data);
  }

  getData(t: number): Readings {}
}

class Levitation extends Sensor {
  constructor(data: LiveReading) {
    super(data);
  }

  getData(t: number): Readings {}
}

interface sensorObj<T extends Sensor> {
  [sensor: string]: T;
}

export default {
  motion: Motion,
  keyence: Keyence,
  temperature: Temperature,
  resistance: Resistance,
  pressure: Pressure,
  magnetism: Magnetism,
  levitation: Levitation,
} as sensorObj<any>;

// ### LOGIC IMPORTED FROM OTHER FILES TO BE INTERGRATED INTO CLASSES HERE ### //

/*
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


export class Levitation extends Magnetism {

    // Measured by Time-of-Flight, and indirectly by hall effect sensors
    // Pod lifts off at some threshold speed then will follow a logistic curve until a defined setpoint
    // As well as noise, add some random spikes or down/up trends from the setpoint (seperate to 
    //   random noise, these will be actual gap height fluctuations)
    // When this occurs, the magnetic field strength changes in unison
    //   (i.e. there's no lag time, unlike the other way around)
    // The hall effect has a dynamic setpoint which is linked to the desired gap height
    // The control system adjusts the field strength to get back to the gap height setpoint

    constructor(time_of_flight: LiveReading) {
        super(time_of_flight);
    }
}



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

/*


import { LiveReading, Readings, measurements } from '../src';
import { Sensor } from '../sensorManager';

export class Motion extends Sensor {
    protected displacement: number;
    protected velocity: number;
    protected acceleration: number;
    
    constructor(accelerometer: LiveReading) {
        super(accelerometer);
        // record the velocity and acceleration for ease of access by dependent subclasses
        // only if the the class is being instantiated with a motion sensor, not indirectly by a subclass
        if (accelerometer.type == 'motion') {
            this.displacement = accelerometer.readings.displacement;
            this.velocity = accelerometer.readings.velocity;
            this.acceleration = accelerometer.readings.acceleration;
        }
    }

    /**
     * Gets current reading from each accelerometer and calculates the other motion variables
     * @param t current time in milliseconds, which is converted to seconds for calculations
     * @returns updated readings object

    update(t: number): Readings {
        this.time = t / 1000;
        for (const unit in this.readings) {
            switch (unit.replace(/_[^_]*\d$/, '')) {
                // switch argument coalesces the accelerometers to one string
                // however each reading will be different due to inidividual random noise
                case 'accelerometer':
                    this.readings[unit] = this.getSensorReading();
                    break;
                // acceleration reading takes the average of all accelerometer readings
                case 'acceleration':
                    this.readings.acceleration = this.averageSensorReadings();
                    break;
                // velocity and displacement are calculated from the acceleration reading using basic kinematics
                case 'velocity':
                    this.readings.velocity += this.readings.acceleration * this.sampling_time;
                    break;
                case 'displacement':
                    this.readings.displacement += this.readings.velocity * this.sampling_time;
                    break;
                default:
                    break;
            }
        }

        const { accelerometer_1, accelerometer_2, accelerometer_3, accelerometer_4, ...data } = this.readings;

        this.displacement = data.displacement;
        this.velocity = data.velocity;
        this.acceleration = data.acceleration;
        // only need to return the derived motion variables, not the individual accelerometer readings
        return data;
    }

    /**
     * Estimate velocity as a logistic curve
     * From analytical velocity value, get accelerometer reading
     * @returns the current timestep's accelerometer reading, with noise added

    private getSensorReading(): number {

        // Logistic function describing velocity as a function of time
        const { velocity: vel, sampling_time: dt } = this.readings;
        const maxVel = measurements.velocity.limits.critical.high;
        const maxAccl = measurements.acceleration.limits.critical.high;
        
        // Logistic curve parameters, set to ensure acceleration doesn't exceed 5
        const growthRate = 0.4;
        const timeOfInflection = 12.5;

        // Calculate expected accelerometer reading from velocity function
        const nextVel = maxVel / (1 + Math.exp(-growthRate * (this.time - timeOfInflection)))
        const nextAccl = (nextVel - vel) / (dt / 1000) >= maxAccl ?
            maxAccl : (nextVel - vel) / (dt / 1000);
        
        // Add noise to the reading
        return nextAccl + Sensor.addRandomNoise(this.rms_noise);
    }

    // Average the sensor readings to determine motion variables
    // this.quantity is the number of physical sensors in this instance
    private averageSensorReadings(): number {
        const { readings, quantity } = this;
        return Object.values(readings).slice(0, quantity)
            .reduce( (acc, val) => acc + val, 0) / quantity;
    }
}



type gaugeType = "pull" | "push" | "reservoir" | "brakes" | "suspension";

export class Pressure extends Temperature {
    
    // pressure loss coefficient * vel^2, from Darcy formula (guesstimate)
    // later define pneumatic tubing parameters (L, D, k)
    protected losses: number = (1.5 * 10**(-5)) * this.velocity ** 2;
    protected gaugeFunction: gaugeType;
    
    constructor(gauge: LiveReading) {
        super(gauge);
    }

    /**
     * Function to categorise the pressure gauges into their respective types and function

    public seperateGauges() {
        console.log(Object.keys(this.readings).forEach( (out: string): void => {
            out.replace(/.*_(.+)/, '$1') as gaugeType;
        }));
    }
    
    /**
     * There are two intake tubes and two exhaust tubes for air, one of each stationed at
     *  the back and front of the pod
     * Assuming there is no other point of infiltration, the air pressure in must be roughly
     *  equal to the air pressure out (assuming minimal losses)
     * Therefor

    public getAirflowPressure() {

    }

    public getReservoirPressure() {

    }

}




export class Resistance extends Temperature {
    readonly initialResistance: number;
    
    constructor(multimeter: LiveReading) {
        super(multimeter);
        this.initialResistance = multimeter.readings.power_line_resistance;
    }

    update(t: number): Readings {
        this.time = t / 1000;
        // Resistance is directly proportional to temperature
        // Use iron resistivity of ~5 * 10^(-3) ohm/K
        this.readings.resistance += (this.readings.temperature - this.initialResistance) * 5 * 10**(-3);
        return this.readings;
    }
}




// MOTION CONTROL UNUSED METHODS //

private getSensorReading(): number {
    // Logistic function describing velocity as a function of time
    const { velocity: vel, sampling_time: dt } = this.readings;
    const maxVel = measurements.velocity.limits.critical.high;
    const maxAccl = measurements.acceleration.limits.critical.high;

    // Logistic curve parameters, set to ensure acceleration doesn't exceed 5
    const growthRate = 0.4;
    const timeOfInflection = 12.5;

    // Calculate expected accelerometer reading from velocity function
    const nextVel =
      maxVel / (1 + Math.exp(-growthRate * (this.time - timeOfInflection)));
    const nextAccl =
      (nextVel - vel) / (dt / 1000) >= maxAccl
        ? maxAccl
        : (nextVel - vel) / (dt / 1000);

    // Add noise to the reading
    return nextAccl + SensorLogic.addRandomNoise(this.rms_noise);
  }

  // Average the sensor readings to determine motion variables
  // this.quantity is the number of physical sensors in this instance
  private averageSensorReadings(): number {
    const { readings, quantity } = this;
    return (
      Object.values(readings)
        .slice(0, quantity)
        .reduce((acc, val) => acc + val, 0) / quantity
    );
  }




*/
