### Branch: `test-sidebar`

<blockquote>All inputs on the sidebar, including setting levitation height, are visible with correct labels and the correct MQTT state transition message is emitted (for the GO button, make sure that the enable toggle works)</blockquote>
<br>

# Setting levitation height

- ### MQTT Message
  ```
  hyped/${podId}/controls/${levitation_height}
  ```
- ### Relevant functions

  - [getLevitationHeight](../../public-app/helpers.ts)
  - [setLevitationHeight](../../server/src/modules/controls/PodControls.service.ts)

- ### Sidebar DOM elements [index](../../ui/app/components/sidebar)

- ### Tests to verify
  - SetLevitationHeight only allows valid input
    - ints within range 0-100
    - number type
    - <em>The function already implements this chcek, bt this test verifies that it works</em>.
    <hr>
  - Test that clicking the <strong>Set</strong> button sends a <stromg>POST</strong> request to the correct URL, with the correct parameters. You can use Playwright's page.waitForRequest or page.waitForResponse methods to wait for the request and check its URL and parameters.

<ol>
  <li><strong>Setting levitation height</strong></li>
  <ul>
    <li>MQTT Message:<br><code>hyped/${podId}/controls/levitation_height`,
      height.toString()</code></li>
    <li>Relevant Objects</li>
    <ul>
    
    <!-- Gets info on Levitation height -->
      <li><em>(func)</em> [view getLevitationHeight](../../public-app/helpers.ts)</li>
    </ul>
  </ul>
</ol>

<hr>

switching between pods changes the selected pod for certain elements

all view options are listed, visible and render the correct component

state machine diagram renders with the expected nodes visible

Open MCT plots measurements on graphs correctly (historical test)

Open MCT plots live measurements from MQTT messages (realtime test)

Open MCT logs a fault when a measurement limit is exceeded (check warning and critical limits)
