import { RangeMeasurement } from '../../types/src/index';
import { DataManager, SensorData } from './utils/data-manager';

    
/* Range-based sensors & measurements

ACC. & NAV:    
accelerometer   accelerometer_avg   displacement    velocity    acceleration

EXT. PRESSURE:  
back_pull               front_pull            front_push           back_push

RESERVOIR FLUID PRESSURE: 
pressure_brakes_reservoir               pressure_active_suspension_reservoir

BRAKE PRESSURE: 
pressure_front_brake                                     pressure_back_brake

MISC. SENSORS: 
thermistor   hall_effect   keyence  power_line_resistance   levitation_height



/*
Planned File Structure

Each of the main response variables obeys different physical laws, and functions can be created to 
predict and govern their change over time, after the pod starts to speed up from rest. Constraints come 
in the form of the allowable numerical ranges for each variable.

Some variables can be analytically calculated, such as acceleration from basic calculus given a set of
initial conditions and desired steady-state cruising speed. Temperature is more arbitrary, it goes up with 
an increase of kinetic energy but to what extent is up to the programmer.
*/
   
       
/**
* this class instantiates different data generation methods for a certain sensor
*/
export class Behaviour {
    // readonly sensor: RangeMeasurement
    public static timestep: number // current iteration 
    
    // Next steps could be to constuct instances with a specific data gen method
    // Such as tweaking randomness, testing response to an expected pod run, changing range limits etc.
    // constructor (sensor: RangeMeasurement, timestep: number, data: SensorData) {}
    // private constructor (dataManager) {}
    
    /**
     * Method to track and generate values for displacement, velocity, acceleration
     * - Acceleration cannot exceed 5m/s^2 or drop below 0 during a run
     * - Velocity is controlled by pod operator and has limits of 0 and 50m/s
     * The pod will follow a logarithmic curve to reach near-maximum velocity
     * Once at desired speed, acceleration will drop to 0 and pod maintains near-constant velocity
     *   with some noise
     * This noise can bring acceleration under its critical minimum, so a moving average is employed
     *   so as to ignore noise
     * @param previousValue last data value generated
     * @returns array of [displacement, velocity, acceleration]
     */
    public static motionSensors(data: SensorData): [number, number, number] {
        let disp = data.data.navigation.displacement;
        let vel = data.data.navigation.velocity;
        let accl = data.data.navigation.acceleration;
        
        
        
        // Logistic function
        // this.sensor.limits
        return [disp, vel, accl]
    }

    // private getCurrentData(): SensorData {
    //     return this.dataManager.getData()
    // }

    /**
     * Follows a rough sine wave with frequency increasing with pod velocity
     * As the timestep of 500ms is much higher than the time period of switching polarity,
     *  the resulting data will not actually resemble a sine wave as it's recording data once 
     *  every few hundred/thousand polarity switches
     * @param previousValue
     * @returns current value
     */
    hallEffect(previousValue: number) {
        // const velocity = .getData();
        // Sine wave of frequency = fn(velocity) (directly proportional)
    }

    /**
     * Begins at 0 at rest
     * Remains at 0 until certain velocity threshold is reached
     * After that, begins to levitate until setpoint is achieved
     * Then remains at that level with small noise fluctuations
     * @param previousValue
     * @returns current value
     */
    levitationHeight(previousValue: number) {
        // if 
    }

    // keyence()
}
      

/**
 * Helper function checks if the limits of a sensor's range are equal to another
 * in order to avoid duplicates in rangeReadings array
 * @param range1 a sensor's allowed range of values
 * @param range2 a sensor of the same type's allowed range
 * @returns true if they share the same limits
*/
/*
function compareLimits(range1: Limits, range2: Limits): boolean {
    return range1.critical.low === range2.critical.low && range1.critical.high === range2.critical.high;
}
*/      

/*
### Comments and cut sections of code - to be deleted later ###

   // interface Behaviour {
       //     temperature: (prevValue: number) => number;
       //     displacement: (prevValue: number) => number;
       //     velocity: (prevValue: number) => number;
       //     acceleration: (prevValue: number) => number;
       //     pressurePullPush: (prevValue: number) => number;
       //     pressurePullPush: (prevValue: number) => number;
       
       //     levitationHeight: (prevValue: number) => number;
       // }
        
        // i > 0 && console.log(sensor[0], uniqueRangeSensors[i - 1][0]);
        
        // const behaviour = {}
        // for (const sensor of uniqueRangeSensors) {
            //     // console.log(sensor[0]);
            //     behaviour[String(sensor[0])] = sensor[0];
            // }
            
            // console.log(behaviour);

// temperature: (prevValue: number) => {}


// printInput(): void {
    //     console.log(this.sensor)
    // }

*/

// console.log(uniqueRangeSensors);
// console.log(`Number of unique sensors: ${uniqueSensorAmt}`);
// console.log(`The pod uses the following set of sensors:${uniqueSensorStringArray}.`);
// console.log(uniqueRangeSensors.slice(0,5));