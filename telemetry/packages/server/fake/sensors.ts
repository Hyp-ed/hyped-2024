import { Limits, LiveReading, Readings, measurements } from "./src";

// ************************** SHARED SENSOR PROPS/METHODS ************************** //
class SensorLogic {

    readonly type: string;
    readonly key: string;
    readonly unit: string;
    readonly format: string;

    readonly limits: Limits;
    readonly rms_noise: number;
    readonly sampling_time: number;
    readonly readings: Readings;
    readonly quantity: number;
    
    private _time: number; // current time in seconds

    constructor(data: LiveReading) {
        Object.assign(this, data);
        this.time = 0;
        // console.log(`Instantiating ${data.type} sensor...`);
        // console.log('this:', this);
    }

    /**
     * Sensor-indiscriminate menthod, dependent only on limiting range
     * Used when user selects random data generation option
     * Does not require class instantiation
     */ 
    public static getRandomValue(limits: Limits, format: 'float' | 'integer'): number {
        const { high, low } = limits.critical;
        // console.log(this._currentValue)
        const range = high - low;
        return format == 'float'
            ? parseFloat((Math.random() * range + low).toFixed(2))
            : Math.floor(Math.random() * (range + 1)) + low;
      }

    
    // Shared instance helper methods //

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

    // protected updateTime(timestep: number): void {
    //     this.time += timestep;
    // }

    protected set time(t: number) {
        this._time = t / 1000;
    }

}

// ************************************ MOTION ************************************ //
class Motion extends SensorLogic {
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
     * @param t current time in milliseconds, which is converted to seconds for calculations
     * @returns updated readings object
     */
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
        return this.readings;
    }

    /**
     * Estimate velocity as a logistic curve
     * From analytical velocity value, get accelerometer reading
     * @returns the current timestep's accelerometer reading, with noise added
     */
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
        return nextAccl + SensorLogic.addRandomNoise(this.rms_noise);
    }

    // Average the sensor readings to determine motion variables
    // this.quantity is the number of physical sensors in this instance
    private averageSensorReadings(): number {
        const { readings, quantity } = this;
        return Object.values(readings).slice(0, quantity)
            .reduce( (acc, val) => acc + val, 0) / quantity;
    }
}


// ************************************ PRESSURE ************************************ //
type gaugeType = "pull" | "push" | "reservoir" | "brakes" | "suspension";

class Pressure extends Motion {
    
    // pressure loss coefficient * vel^2, from Darcy formula (guesstimate)
    // later define pneumatic tubing parameters (L, D, k)
    protected losses: number = (1.5 * 10**(-5)) * this.velocity ** 2;
    protected gaugeFunction: gaugeType;
    
    constructor(gauge: LiveReading) {
        super(gauge);
    }

    /**
     * Function to categorise the pressure gauges into their respective types and function
    */
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
    */
    public getAirflowPressure() {

    }

    public getReservoirPressure() {

    }

}
    
    
// ************************************ TEMPERATURE ********************************* //
class Temperature extends Motion {

    constructor(thermistor: LiveReading) {
        super(thermistor);
    }
}



// ************************************ KEYENCE ************************************* //
class Keyence extends SensorLogic {
    
    // get logic from old fake-data file
    
    constructor(keyence: LiveReading) {
        super(keyence);
    }
}


// ************************************ POWER *************************************** //
class Resistance extends Temperature {
    
    // get logic from old fake-data file
    
    constructor(multimeter: LiveReading) {
        super(multimeter);
    }
}


// ************************************ HALL EFFECT ********************************* //

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
class Hall_Effect extends Motion {

    protected readonly gapHeightSetpoint: number;


    constructor(Hall_Effect: LiveReading) {
        super(Hall_Effect);
    }
}


// ************************************ LEVITATION ********************************** //
class Levitation extends Motion {

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


export default {
    SensorLogic,
    Motion,
    Pressure,
    Temperature,
    Hall_Effect,
    Keyence,
    Resistance,
    Levitation
};