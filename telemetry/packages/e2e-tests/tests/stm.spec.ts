import { test, expect } from '@playwright/test';

test('mode-appropriate stm nodes rendered', async ({ page }) => {
  await page.goto('http://localhost:5173');
  await page.click('button:text("State")')

  await expect(page).toHaveTitle('HYPED24 | Telemetry');
});
