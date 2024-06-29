import { test, expect, Page, Frame } from '@playwright/test';
import { validateMqttMessage } from '../lib/mqtt';

const getSidebar = async (page: Page): Promise<Frame> => {
  await page.goto('http://localhost:5173');
  const react = page.frame({
    url: 'http://localhost:5173/app/index.html',
  });
  expect(react).toBeTruthy();
  // wait until we get a response from the server
  await page.waitForResponse('http://localhost:3000/ping');
  return react as Frame;
};

test('has sidebar', async ({ page }) => {
  const react = await getSidebar(page);
  await expect(react.getByRole('main').getByText('Controls')).toBeVisible();
});

test('registers user input for levitation height input', async ({ page }) => {
  const react = await getSidebar(page);

  const low = 0;
  const high = 100;

  // Input random value within allowable range
  const testInput = Math.floor(Math.random() * (high - low)).toString();
  await react
    .getByTestId(`pod-controls-pod_2024`)
    .getByTestId('height-input')
    .fill(testInput);

  // Set levitation height and verify that the correct MQTT message is sent
  await validateMqttMessage(
    async () =>
      await react
        .getByTestId(`pod-controls-pod_2024`)
        .getByTestId('set-height-button')
        .click(),
    (receivedTopic, message) => {
      expect(receivedTopic).toBe(`hyped/pod_2024/controls/levitation_height`);
      expect(message.toString()).toBe(testInput);
    },
  );
});
