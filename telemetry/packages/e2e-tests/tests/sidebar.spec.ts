import { test, expect } from '@playwright/test';
import { levitationHeightCommon } from '@hyped/telemetry-constants/src/pods.common';
const  { limits: { critical } } = levitationHeightCommon;

test('registers user input for levitation height input', async ({ page }) => {
  await page.goto('http://localhost:5173');

  const [lower, upper] = [critical.min, critical.max];
  const testInput = Math.floor(Math.random() * (upper - lower));en);

  await page.fill('[data-testid="height-input"]', t());

  await page.click('[data-testid="set-height-button"]')
})

/**
 * Tests response for input within allowed range of levitation height
 */
// test('correct response for allowed height input value', asymc ({ page }) => {
//   await page.goto('http://localhost:5173');

//   await page.click()
// })