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