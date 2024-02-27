# Telemetry
## Fake Data Generation Branch

### Purpose
To iteratively generate appropriate and physically reasonable data series for all types of numerical-based sensors on board the pod. 'Types' refers to whether the sensor object stored in ```pods.ts``` separates sensors by some unique quality (or property).

I.e. all of the thermnistors have the exact same proerty values. In actuality, their readings will differ as they placed in different locations across the pod. However, the data structure does not hold this information and therefore they are equivalent, so only one generic 'thermistor' object will be studied.

In contrast, the pressure gauges have descriptive names which allow one to infer their likely behaviour during operation (front/back-push refers to gauging pressure upon acceleration, which would see an absolute increase in gauge pressure on both ends of the pod due to the greater stagnation pressure and the nose of the pod and pressure wake field behind). Similarly, the pressure gauges of the reservoirs can be reasonably assumed to not vary significantly, but as temperature rises the pressure will increase slightly due to Amonton's Law.

### Use
The program will record and store all values calculated by the methods created in ```pod.behaviour.ts``` throughout a specified timeframe, at a specified interval. The main file is named ```data-gen.ts```. It filters and constructs an array of relevant and unique sensors from the Pod Ness sensor object structure. A new interface is created called ```Sensor Data```. It extends ```RangeMeasurement``` - adding the properties of ```currentValue``` and ```movingAvg```, and encapsulates the measurement object with the sensor's name as a ```Record```, reflecting the object shape of the pod's sensor objects.

A Singleton class was created in its dedicated file, ```data-manager.ts```. When the program is run, a single instance of this class is created. The instance holds the current set of data, and at each iteration is updated before the current data is pushed into data storage arrays for all sensors. Once created, this instance and its data property is accessible from both the main file, and the pod behavious class file, which holds all the methods written to generate each sensor's next value.

In the main file, the program's main function, ```generateDataSeries```, runs a loop iterating through the time period defined. For each sensor type, it calls the relevant static method in the ```Behaviour``` class. Some sensors can be categorised, e.g. the navigation sensors. The values for displacement, velocity and acceleration are interdependet. As the pod's velocity is increased, its displacement and acceleration can be calculated, given the acceleration constraints provided by its critical limits nested object.

A logistic function was chosen as the appropriate function for pod velocity to follow. This allows us to minimse the time to reach maximum speed by adjusting the peak acceleration (dv/dt) to reach but not exceed 5m/s^2 at the velocity-time inflection point.

Similar functions have been or are in the process of being created for the rest of the sensors. Complexity varies, and some require a lot more guesswork.

```generateDataSeries``` takes in a boolean parameter ```random```, set to false by default. If the user sets this to true, iteration will be completely random, selecting sensors' new values as a random number between their critical limits.

### Next Steps
<ul>
    <li>Complete the behaviour class
    <li>Run different tests
    <li>Pair with GUI to visualise data generation
    <li>...
</ul>
