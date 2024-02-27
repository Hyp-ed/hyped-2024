
## To-do list

- Complete definitions file [X]
- Organise file structure [X]
- Create logical functions for next data points for all sensors [ ]
- Complete all classes in sensors.ts [ ]
<em><ul>
    <li>Minimise code and amount of classes as much as possible</li>
    <li>Categorise sensors with similar data functionality (by that I mean the nextValue method) into the same class, e.g. class TempDependent, VelocityDependent, Pressure etc.</li>
    <li>Remove comments</li>
</ul></em>

- Complete main.ts [ ] 
<em><ul>
    <li>Change main function to interact with Sensor classes not Behaviour</li>
    <li>Think about user-specified parameters like end time (or else they just hit ^C or wait for hardcoded end time)</li>
    <li>Create class instances. Minimse amount of classes - one for all the pressure sensors with different function parameters or conditions for instance</li>
    <li>Think of how to work with different timesteps for different sensors which depend on each other</li>
    <li>Think of more features to be added</li>
    <li>Remove comments</li>
</ul></em>

- Add functionality to DataManager to upload live values [ ]
- Change stored data into an object of 2D arrays which store the timestamp and the value at said time for each object
- Add flexibility with user input via read/write to csv file with data parameters [ ]



## Next Steps

<ol>
    <li>Code in the noise. Shouldn't be difficult, just use a function based on Math.random and weight the amplitude of the noise to some extent. Decide whether how 'noisy' data is might be dependent upon certain vasriables such as speed. I'm not sure so ask GPT or research this.</li>
    <li>Create moving average function with ```window``` parameter set to 5 as default. Look into exponential moving average too. The average value will be used to determine whether a reading is out of bounds or it's just the noise.</li>
    <li>Find out how the noise levels compare from different sensors e.g. thermistors, pressure gauges, digital sensors, navigation etc.</li>
    <li>Create a simple function for <strong>reservoir pressure</strong>, it will not vary by much, but increase with temperature slightly. Reading will have some noise.</li>
    <li>Write functions for the other pressures. Push = acceleration, so front pressure goes up and back goes down (both go further away from atmospheric, their absolute gauge pressure increases). And pull = decelleration, so the opposite. Also double check with David that you're interpreting the pressure variable terms correctly.</li>
    <li>I am assuming that the accelerometer(s) are all supposed to measure the absolute pod acceleration with respect to a stationay observer, and that's how the navigation parameters are found. Except this assumption seems wrong, as the accelerometer has a range of -150 to 150 m/s^2 while acceleration can only go up to 5 m/s^2. Is this perhaps referring to the sensors' physical limits of its capability to read acceleration, while the pod itself is not built to exceed 5m/s^2? In other words, the accelerometer will be limited to the 0-5 range, it's just not "critical" for the sensor in terms of safety, it's critical for the pod's safety. Perhaps. But another uncertainty is that the pod's acceleration can be easily determined by it's speed, which - <em>I assume</em> - we are controlling. <strong><em>Update: wrong, we are not controlling speed. We just switch on the power and track it using the accelerometer. The sensor's operational range is +-150. Above 150 it won't read the acceleration accurately. The pod cannot exceed 5.</em></strong> We have one sensor to measure navigation quantities, and that's the accelerometer. It has an operating range of -150 to 150. This specific pod prototype, Pod Ness, has an operating acceleration range of 0 to 5 (presumably this means -5 to 5). We generate fake data for the accelerometer sensor, and analyse it with the view of keeping pod acceleration below 5, and we also calculate other navigation quantities starting from acceleration.</li>
</ol>
<hr>

<ol>
    <strong>Thursday notes from discussion with David + discussion with Damen about the public app, React and Three.js</strong><hr>
    <li><strong>Seperate all sensor data into its own file. This sensor data structure will be like the one in pods, but reduced to the data-gen relevant sensors, and with added/removed properties. Outside of this file, no other functional part of the program will be able to modify or access the file. The data file will be imported. Its properties are constant (so no 'currentVal' or 'movingAvg'). They are properties inherent to the sensor, like with the pod object. A new property David would like is a sampling rate property. This will define the delta T for that sensor's data generation, giving us a lot more freedom and making the code run a lot quicker as right now, there's only one global delta T variable so if we needed say 0.05s, all variables would be measured twenty times per second which would be unnecessary and slow. Of course, the value of this property will be changed and modified by our team, but not during runtime. This is a fixed object which is exporting its data for the data generation function to run. Another object or array will store the transient values, which we will also send live to the server as they are calculated and a graph can be animated in real time.</strong></li>
    <li>One issue I foresee is that if different sensors have different delta Ts, it will make the main loop more complicated. Say the thermistor takes a reading every 0.2s. The accelerometer every 0.5s. So we'd run the loop ever 0.2, while checking if the time is also a multiple of 0.5 (and all the other sensors' times). To mitigate this slightly, I will add functionality to the ```specific``` parameter, so we can view a select few variables in one run, or all of them if we want to.</li>
    <li>Different sensors will have a new property which defines its time interval (dt) and perhaps its degree of noise (as higher quality sensors would have less noise due to better electronic circuits)</li>
    Separate the sensor file into a new file with the sensor object and its properties relevant to the data generation
    <li>The fake data generation program will be placed into its own directory</li>
    <li>It's only for internal use, but there are restrictions/rules we need to follow from EHW committee</li>
    <li>Add a method to the data manager class to upload the data at each step of the iteration to the server/mqtt so we can view it live (graph animation)</li>
    <li>We don't have complete or current data on all the sensors we're using   and we'll need to ask electronics team to fill in a spreadsheet with the data for range limits, (critical and warning and expected/nominal levels)</li>
    <li>Run the logical data gen methods past the electronics/other team to see if they agree it makes sense</li>
</ol>