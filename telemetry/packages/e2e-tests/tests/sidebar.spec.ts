import { test, expect } from '@playwright/test';
import { levitationHeightCommon } from '@hyped/telemetry-constants/src/pods/common';
const  { limits: { critical } } = levitationHeightCommon;

test('registers user input for levitation height input', async ({ page }) => {
  await page.goto('http://localhost:5173');

  // Manouvre to debug logging view
  await page.click('text="Logs"');

  // Ensure connection is established
  await expect(page.locator.)

  const [lower, upper] = [critical.low, critical.high];
  // Input random value within allowable range
  const testInput = Math.floor(Math.random() * (upper - lower)).toString();
  await page.fill('[data-testid="height-input"]', testInput);
  // Set height and send levitation message to control system
  await page.click('text="Set"');
  await page.click('text=" LEVITATE"')

  await expect(page).
})

/**
 * Tests response for input within allowed range of levitation height
 */
// test('correct response for allowed height input value', asymc ({ page }) => {
//   await page.goto('http://localhost:5173');

//   await page.click()
// })