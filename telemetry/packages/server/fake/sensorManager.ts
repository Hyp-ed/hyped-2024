import { Limits, LiveReading, Readings, measurements, ReadingsMap, SensorData, SensorInstance } from "./src";

// ************************** SHARED SENSOR PROPS/METHODS ************************** //
export abstract class Sensor {
    public static samplingTimes: Record<string, number>;
    // Time-variant objects
    // Records the actual time each sensor should be sampled next
    public static nextSamplingTimes: typeof this.samplingTimes;
    // Records whether each sensor has been sampled at the current time
    public static isSampled: Record<string, boolean>;

    readonly type: string;
    readonly key: string;
    readonly unit: string;
    readonly format: string;

    readonly limits: Limits;
    readonly rms_noise: number;
    readonly sampling_time: number;
    readonly quantity: number;
    protected _time: number; // current time in seconds
    protected readings: Readings;

    constructor(data: LiveReading) {
        Object.assign(this, data);
        this.time = 0;
        // console.log(`Instantiating ${data.type} sensor...`);
        // console.log('this:', this);
    }

    abstract update(t: number): Readings;

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

    
    // Shared instance helper methods //

    /**
     * Generates a random noise value from a Gaussian distribution
     * This function will be called for each sensor of a given type, then averaged
     * @param mean self-explanatory
     * @param rms_noise sensor's RMS noise value, used as the standard deviation
     * @returns a random number defined by the normal distribution of stdDev = RMS noise
     */
    protected static addRandomNoise(rms_noise: number, mean: number = 0): number {
        // Using the Box-Muller transform to generate random values from a normal distribution
        const u1 = Math.random();
        const u2 = Math.random();
        const z = Math.sqrt(-2 * Math.log(u1)) * Math.cos(2 * Math.PI * u2);
    
        return parseFloat((z * rms_noise + mean).toFixed(2));
    }

    protected static expMovingAvg(vals: number[], alpha: number): number | undefined {
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

import { Motion, Temperature, Pressure, Magnetism, Resistance, Levitation, Keyence } from './sensors/index';

// this class generates all data required current timestep
// there is a specific order in which the data must be generated
// e.g. motion and temperature data must be generated before pressure data
// and levitation data must be generated last, after the hall effect reading is recorded
export class SensorMap {
    private sensorClasses: any[];
    private sensors: SensorInstance<typeof this.sensorClasses[number]>[] = [];
    // private sensors: any[] = [];

    private _time: number;
    samplingTimes: Record<string, number>;
    sensorData: SensorData;

    // Time-variant objects
    // Records the actual time each sensor should be sampled next
    // nextSamplingTimes: typeof this.samplingTimes;
    // Records whether each sensor has been sampled at the current time
    // isSampled: Record<string, boolean>;

    constructor(sensorData: SensorData, samplingTimes: Record<string, number>) {
        Sensor.samplingTimes = samplingTimes;
        // create object to store the next sampling time for each sensor, initialised to the first timestep
        Sensor.nextSamplingTimes = {...samplingTimes};
        Sensor.isSampled = Object.fromEntries(Object.keys(samplingTimes).map( (key) => [key, false]));

        this.sensorClasses = [Motion, Temperature, Pressure, Magnetism, Keyence, Resistance, Levitation];
        
        // instantiated sensor classes must be stored in an array to maintain their order
        // I.e. Motion -> Temperature and Magnetism -> rest
        this.sensors = this.sensorClasses.map( SensorCls => new SensorCls(
            sensorData[SensorCls.name.toLowerCase()]
        ) );

        // console.log(this.sensors)
    }

    getData(): void {
        // generate data for each sensor type
        const prevTime = this.time | 0;

        // Ensure the time is set to the next sampling time before the data generation loop
        this.time = Math.min(...Object.values(Sensor.nextSamplingTimes));

        const delay = this.time - prevTime;

        setTimeout(() => {
            
            this.sensors.forEach( (sensor, i) => {
                if (this._time < Sensor.nextSamplingTimes[sensor.type]) { return; }
                
                const sensorName = sensor.name.toLowerCase();
                // update readings for each sensor
                this.sensorData[sensorName].readings = this.sensors[i].update(this._time);
                // MQTT.upload(this.sensorData[sensor].readings);

                // update dynamic sensor data
                Sensor.nextSamplingTimes[sensor.type] += Sensor.samplingTimes[sensor.type];
                Sensor.isSampled[sensor.type] = true;
            });

            // Object.keys(this.sensorData).

        }, delay);

        // upload all data for the current timestep
    }

    getRandomData(): void {
        
        const prevTime = this.time | 0;

        // Ensure the time is set to the next sampling time before the data generation loop
        this.time = Math.min(...Object.values(Sensor.nextSamplingTimes));

        const delay = this.time - prevTime;

        setTimeout( () => {
            // Loop through sensor types and update their readings data
            for (const sensor in this.sensorData) {
                // Skip sensors that haven't reached their next sampling time
                if (this._time < Sensor.nextSamplingTimes[sensor]) { continue; }
                
                // Get new randomised values for each sensor
                for (const unit in this.sensorData[sensor]) {
                    const { limits, rms_noise, format } = this.sensorData[sensor];
                    this.sensorData[sensor].readings[unit] = Sensor
                        .getRandomValue(limits, rms_noise, format);
                }

                Sensor.isSampled[sensor] = true;

                // The motion sensors are a special case, as they consist of not just sensors but also derived measurements
                // Only the derived measurements need to be uploaded, and while the accelerometers will be randomised, the
                //  motion variables will be calculated from them
                if (sensor == 'motion') {
                    const { accelerometer_1, accelerometer_2, accelerometer_3, accelerometer_4, ...data } 
                        = this.sensorData['motion'].readings;
                    // MQTT.upload(data);
                } else {
                    // MQTT.upload(this.sensorData[sensor].readings);
                }

                // Set the next sampling time for the sensor
                Sensor.nextSamplingTimes[sensor] += Sensor.samplingTimes[sensor];
            }
        }, delay);

        // const dataToUpload: ReadingsMap = {};
        
        // for (const [sensor, data] of Object.entries(this.sensorData)) {
        //     dataToUpload[sensor] = data.readings;
        // }
    }

    private set time(nextTime) {
        // this method should only be called once the nextSamplingTimes object has been updated
        if (nextTime == this._time) { return; }
        this._time = nextTime;
    }

    get time() {
        return this._time;
    }

}

const samplingTimes = {
    motion: 1000,
    pressure: 500,
    temperature: 500,
    magnetism: 500,
    keyence: 500,
    resistance: 500,
    levitation: 500
}

const sensorData: SensorData = {
        motion: {
          name: 'Accelerometer 1',
          key: 'accelerometer',
          format: 'float',
          type: 'motion',
          unit: 'm/s²',
          limits: { critical: { low: -150, high: 150 } },
          rms_noise: 0.01625,
          sampling_time: 1000,
          quantity: 4,
          readings: {
            accelerometer_1: 0,
            accelerometer_2: 0,
            accelerometer_3: 0,
            accelerometer_4: 0,
            displacement: 0,
            velocity: 5,
            acceleration: 0
          }
        },
        pressure: {
          name: 'Pressure – Back Pull',
          key: 'pressure_back_pull',
          format: 'float',
          type: 'pressure',
          unit: 'bar',
          rms_noise: 0.001,
          sampling_time: 500,
          limits: { critical: { low: -0.2, high: 5.5 }, warning: { low: -0.2, high: 5.5 } },
          quantity: 8,
          readings: {
            pressure_back_pull: 1,
            pressure_front_pull: 1,
            pressure_front_push: 1,
            pressure_back_push: 1,
            pressure_brakes_reservoir: 5,
            pressure_active_suspension_reservoir: 5,
            pressure_front_brake: 1,
            pressure_back_brake: 1
          }
        },
        temperature: {
          name: 'Thermistor 1',
          key: 'thermistor',
          format: 'float',
          type: 'temperature',
          unit: '°C',
          limits: { critical: { low: -40, high: 125 }, warning: { low: -40, high: 125 } },
          rms_noise: 0.05,
          sampling_time: 500,
          quantity: 12,
          readings: {
            thermistor_1: 25,
            thermistor_2: 25,
            thermistor_3: 25,
            thermistor_4: 25,
            thermistor_5: 25,
            thermistor_6: 25,
            thermistor_7: 25,
            thermistor_8: 25,
            thermistor_9: 25,
            thermistor_10: 25,
            thermistor_11: 25,
            thermistor_12: 25
          }
        },
        magnetism: {
          name: 'Hall Effect 1',
          key: 'hall_effect',
          format: 'float',
          type: 'magnetism',
          unit: 'A',
          limits: { critical: { low: 0, high: 500 } },
          rms_noise: 0.5,
          sampling_time: 500,
          quantity: 2,
          readings: { hall_effect_1: 0, hall_effect_2: 0 }
        },
        keyence: {
          name: 'Keyence 1',
          key: 'keyence',
          format: 'integer',
          type: 'keyence',
          unit: 'number of stripes',
          limits: { critical: { low: 0, high: 16 } },
          rms_noise: 0,
          sampling_time: 500,
          quantity: 2,
          readings: { keyence_1: 0, keyence_2: 0 }
        },
        resistance: {
          name: 'Power Line Resistance',
          key: 'power_line_resistance',
          format: 'integer',
          type: 'resistance',
          unit: 'kΩ',
          limits: { critical: { low: 0, high: 100 } },
          rms_noise: 1,
          sampling_time: 500,
          quantity: 1,
          readings: { power_line_resistance: 10 }
        },
        levitation: {
          name: 'Levitation Height',
          key: 'levitation_height',
          format: 'float',
          type: 'levitation',
          unit: 'mm',
          limits: { critical: { low: 0, high: 100 } },
          rms_noise: 2,
          sampling_time: 500,
          quantity: 1,
          readings: { levitation_height: 0 }
        }
}

const sensor = new SensorMap(sensorData, samplingTimes);
