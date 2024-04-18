End-to-end tests to write:

- <span class="doing"> all inputs on the sidebar, including setting levitation height, are visible with correct labels and the correct MQTT state transition message is emitted (for the GO button, make sure that the enable toggle works)
  </span>
  <ul>
  <li>Setting Levitation Height</li>
  <li>Correct Labels Visible</li>
  <li>MQTT State Transition Message Emitted (<em>Enable Toggle should work for 'GO' button</em>)</li>
  </ul>
  <hr>
  - <span class="todo"> switching between pods changes the selected pod for certain elements
  </span><br>- <span class="todo"> all view options are listed, visible and render the correct component
  </span><br>- <span class="todo"> state machine diagram renders with the expected nodes visible
  </span><br>- <span class="todo"> Open MCT plots measurements on graphs correctly (historical test)
  </span><br>- <span class="todo"> Open MCT plots live measurements from MQTT messages (realtime test)
  </span><br>- <span class="todo"> Open MCT logs a fault when a measurement limit is exceeded (check warning and critical limits)
  </span><br>- <span class="todo"> More to come...

<style>
  .todo {
    color: red;
  }
  .doing {
    color: orange;
    font-weight: bold;
  }
  .done {
    color: lime;
    text-decoration: underline;
  }
</style>
