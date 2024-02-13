import { RangeMeasurement } from "/Users/admin/Documents/hypedTelemetry/telemetry/packages/types/src/pods/pods.types"

type LiveReading = RangeMeasurement & {
    quantity: number;
    readings: Readings;  // measurements it provides; e.g. accelerometer gives values for acceleration, displacement and velocity (latter two indirectly)
}

// sensor's output readings
export type Readings = { 
    [measurement: string]: number 
};


class Keyence {

  public testDataStorage: Record<string, Readings>;

  private keyenceData: LiveReading;
  private limits;

  constructor(data: LiveReading) {
    this.keyenceData = data;
    this.limits = data.limits.critical;
  }

  getData(t: number): Readings {
    const newReadings = { ...this.keyenceData.readings};

    Object.keys(this.keyenceData.readings).forEach( (key) => {
      const polarity = Math.random() >= 0.5 ? 1 : -1;
      newReadings.key += (5 * polarity)
      if (newReadings[key] >= this.limits.high ||
        newReadings[key] <= this.limits.low) {
          newReadings[key] *= -1;
        }
      });
    console.log(newReadings);
    this.testDataStorage[t] = newReadings;
    return newReadings;
  }
}




const keyenceData: LiveReading = {
    name: 'Keyence 1',
    key: 'keyence',
    format: "integer",
    type: 'keyence',
    unit: 'number of stripes',
    limits: { critical: { high: 16, low: 0 } },
    rms_noise: 0,
    sampling_time: 500,
    quantity: 2,
    readings: { keyence_1: 0, keyence_2: 0 }
}

const runTime = 20 // seconds

const testRun = new Keyence(keyenceData, runTime);


for (let t = 0; t < runTime; t++) {
  testRun.getData(t);
}

console.log("All keyence data generated");
console.log(testRun.testDataStorage);



export default Keyence;