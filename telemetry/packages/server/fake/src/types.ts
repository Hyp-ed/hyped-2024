import { Measurement, RangeMeasurement } from ".";

// for the variables representing physical sensors, not derived measurements
// for measurement objects which represent physical sensors
export type LiveReading = Omit<RangeMeasurement, 'name'> & {
    quantity: number;
    liveData: {
        [key: string]: number;
    };  // measurements it provides; e.g. accelerometer gives values for acceleration, displacement and velocity (latter two indirectly)
}

// combine all sensor data into comprehensive object type to be used throughout data generation
export type LiveData = Record<string, LiveReading>;

// for each measurements initial value, categorised by governing sensor type
export type InitialConditions = { 
    [sensorType: string]: {
        [measurement: string]: number;
    }
}




export type Relation = {
    [key: Measurement["key"]]: LiveReading;
}

// export type RelationTable = 


// Interactive sensor behaviour and interdependencies
// export type Behaviour<T extends string[]> = {
//     // live readings it uses to estimate its next value
//     // E.g. pressure depends on velocity & temperature to varying degrees
//     // dependencies?: RangeMeasurement["key"];
//     dependencies?: T; // Sensor["outputs"]
//     nextValue: getNextValue<T>;
// }


// create generic function type with constraints for getting next data value
//   from dependencies
export type getNextValue<T extends string[]> 
    = (args: Record<T[number], number>) => number;





// type SensorNextValueMethod<T extends Sensors[keyof Sensors]> = T extends {
//     measurements: { direct: infer DirectMeasurement; indirect?: (infer IndirectMeasurement)[] };
//     // Define additional parameters if needed
// }
//     ? (...args: [DirectMeasurement, ...(IndirectMeasurement extends string ? [number] : [])]) => void
//     : never;


// END OF FILE




// export type LiveReading = RangeMeasurement & {
//     value: number; // dynamic current value at given timestep
// }
// export const measurements = pods.pod_1.measurements as Record<string, Measurement>;

// const dataTypes: string[] = Object.entries(measurements)
//     .map( ( [name, props], i, arr ) => props.unit)
//     .filter( (el, i, self) => self.indexOf(el) === i );

// type SensorType = Record<string, Measurement>[keyof Record<string, Measurement>]['type']



// for categories of variables dervied from physical sensors
// export type MeasurementCategory = {
//     [key: string]: LiveReading;
// }


// make a type for dependencies in calculation

/*

Example:





*/

// const uniqueNumbers = [1, 2, 3, 4, 5, 6] as const;

// type MyType = {
//   myNumber: typeof uniqueNumbers[number];
// };

// // Example usage:
// const validObject: MyType = { myNumber: 3 }; // Valid
// const invalidObject: MyType = { myNumber: 7 }; // Error, as 7 is not in the uniqueNumbers array