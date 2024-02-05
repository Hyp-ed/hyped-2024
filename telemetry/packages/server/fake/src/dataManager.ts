import { Limits, LiveReading, Readings, ReadingsMap, SensorData, SensorInstance, sensorData } from "..";
import { Utilities } from "./sensorUtils";

// ************************** SHARED SENSOR PROPS/METHODS ************************** //
export class Sensor {
    
    // Records the actual time each sensor should be sampled next
    public static nextSamplingTimes: Record<string, number>;
    // Records whether each sensor has been sampled at the current time
    public static isSampled: Record<string, boolean>;

    // Sensor properties
    readonly type: string; // sensor type (same as the name of the object in sensorData)
    // readonly key: string; // sensor id, but this not as a top-level property, instead the readings all have unique keys
    readonly format: string; // for random ternary logic (keyence is integer, rest are float)
    // readonly unit: string; // if we want to display units on UI, probably not necessary as front end will already do this
    readonly limits: Limits;
    readonly rms_noise: number;
    readonly sampling_time: number;
    // readonly quantity: number; // uncomment if needed somewhere like averaging
    
    // Variable sensor data
    protected readings: [number, Readings]; // live sensor readings
    protected time: number; // current time in seconds
    protected _activated: boolean = false; // flag to indicate whether the sensor has been sampled at the current time

    
    // Extract relevant properties from sensor data entries
    constructor({ type, format, limits, rms_noise, sampling_time, readings }: LiveReading) {
        Object.assign(this, { type, format, limits, rms_noise, sampling_time, readings });
        this.time = 0;
    }

    getData(t: number): Readings {
        
        // this.update(newReadings)
    }

    /*
    
    Each instance, will run getData() generate values for their respective measurements
    Then, this.update will run after generating a value in order to publish data to MQTT

    */

    // Update time and set isSampled flag to true for reference by interested sensor classes
    protected update(newReadings: Readings): void {
        // Set flag to true to indicate that the sensor has been sampled at the current time
        this.activated = true;
        // Update readings with new values
        this.readings = [this.time, newReadings];
        
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

    set activated(newState: boolean) {
        this._activated = newState;
    }

}



// import { Motion, Temperature, Pressure, Magnetism, Resistance, Levitation, Keyence } from './sensors/index';

// There is a specific order in which the data must be generated
// e.g. motion and temperature data must be generated before pressure data
// However, this issue is evaded as any required data not generated yet will be, and that reading will
//   be recorded for that sensor should it also be due to give a reading at that time
export class SensorMap {
    private sensorClasses: any[];
    private sensors: SensorInstance<typeof this.sensorClasses[number]>[] = [];
    // private sensors: any[] = [];

    private _time: number;

    private sensorData: SensorData = sensorData;

    // Time-variant objects
    // Records the actual time each sensor should be sampled next
    // nextSamplingTimes: typeof this.samplingTimes;
    // Records whether each sensor has been sampled at the current time
    // isSampled: Record<string, boolean>;

    /**
     * Single instantiation of this class for each run
     * @param runSensors user-defined array of sensor names to be run in the current simulation. The exclusion
     * of sensors from whose values others are derived does not preclude their data generation, but solely their
     * publication to the server. However, their values will only be calculated as needed, not in accordance with
     * their sampling frequencies.
     */
    constructor(private runSensors: string[]) {

        // store the sampling times for each sensor type in accessible and simple Record
        const samplingTimes: Record<string, number> = {};
        Object.entries(sensorData).forEach( ([ name, sensor ]: [string, LiveReading]) => {
            samplingTimes[name] = sensor.sampling_time;
        });

        // Dynamic object which store the next sampling times for each sensor, initialised to the first timestep
        // Every iteration, it updates by incrementing each sensor's value by its fixed sampling time
        Sensor.nextSamplingTimes = {...samplingTimes};
        Sensor.isSampled = Object.fromEntries(Object.keys(samplingTimes).map( (key) => [key, false]));

        // this.sensorClasses = [Motion, Temperature, Pressure, Magnetism, Keyence, Resistance, Levitation];
        
        // instantiated sensor classes must be stored in an array to maintain their order
        // I.e. Motion -> Temperature and Magnetism -> rest
        // this.sensors = this.sensorClasses.map( SensorCls => new SensorCls(
        //     sensorData[SensorCls.name.toLowerCase()]
        // ) );

        // console.log(this.sensors)
    }

    // Initialise sensors with their respective data and store instances in array
    initialiseSensors(): void {
        for (const sensor in sensorData) {
            this.sensors.push(new Sensor(sensorData[sensor]));
        }
        console.log(this.sensors);
    }

    // Generate data for each sensor type
    generate(): void {

        // Record previous time; if at initial step then set to 0
        const prevTime = this.time | 0;

        // Ensure the time is set to the next sampling time before the data generation loop
        // While this is an assignment operator, not an incremental, infinite loops are impossible
        //   because the while loop in main.ts runs for a finite, user-specified amount of seconds in
        //   real time.
        // Besides error handling, the logic to this code is to ensure that sensor readings are taken
        //   at every single timestep at which at least one sensor is due to generate data.
        // Depending on the variability of the sampling times, the overall frequency at which data is
        //   could vary significantly, hence this type of time increment logic.
        this.time = Math.min(...Object.values(Sensor.nextSamplingTimes));

        // Set isSampled to false for all sensors before running loop
        Sensor.isSampled = Object.fromEntries(Object.keys(Sensor.isSampled).map( (key) => [key, false]));

        const delay = this.time - prevTime;

        // Reset activated state for all sensors
        this.sensors.forEach( (sensor, i) => {
            sensor.activated = false;
        });

        setTimeout(() => {
            
            this.sensors.forEach( (sensor, i) => {
                if (this._time < Sensor.nextSamplingTimes[sensor.type]) { return; }
                
                this.sensors[i].getData(this._time);
                // this method also runs the update method

                // const sensorName = sensor.name.toLowerCase();
                // update readings for each sensor
                // MQTT.upload(this.sensorData[sensor].readings);

                // update dynamic sensor data

                // ALREADYB HAPPENING FOR INDIVIDUAL SENSORS IN SENSOR CLASS
                // Sensor.nextSamplingTimes[sensor.type] += Sensor.samplingTimes[sensor.type];
                // Sensor.isSampled[sensor.type] = true;
            });

            // Object.keys(this.sensorData).

        }, delay);

        // upload all data for the current timestep
    }

    generateRandom(): void {
        
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
                    this.sensorData[sensor].readings[unit] = Utilities.getRandomValue(
                        limits, rms_noise, format
                    );
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



// const run_1 = new SensorMap(sensorData);