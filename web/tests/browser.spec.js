const {test, expect} = require('@playwright/test');

async function ready(page) {
    await expect(page.locator('#move')).toBeEnabled({timeout: 90000});
    await expect(page.locator('#status')).toHaveText('Your turn');
}

async function submit(page, value) {
    await page.locator('#move').fill(value);
    await page.locator('#move').press('Enter');
}

for (const [game, move, extra] of [['1', '0 0', null], ['2', '10 10', 111], ['3', '-1 2', 51]]) {
    test(`game ${game}: real Wasm AI, validation, quit, and restart`, async ({page}) => {
        const errors = [];
        page.on('pageerror', error => errors.push(error.message));
        const wasm = page.waitForResponse(response => response.url().endsWith('/Main.wasm'));
        await page.goto('/site/');
        expect((await wasm).ok()).toBe(true);
        await page.locator(`[data-game="${game}"]`).click();
        await ready(page);
        const transcript = page.locator('#transcript');
        const preview = (await transcript.innerText()).split('\n').find(line => line.startsWith('Legal actions:'));
        expect((preview.match(/n:/g) || []).length).toBe(game === '1' ? 9 : 10);
        if (extra) await expect(transcript).toContainText(`${extra} more actions available.`);
        await submit(page, 'bad');
        await ready(page);
        await expect(transcript).toContainText('Invalid input:');
        await submit(page, '99 99');
        await ready(page);
        await expect(transcript).toContainText('Illegal move:');
        await submit(page, move);
        await ready(page);
        await expect(transcript).toContainText('AI move:');
        await expect(page.locator('#move')).toBeFocused();
        await submit(page, 'q');
        await expect(page.locator('#status')).toHaveText('Match ended');
        await expect(page.locator('#move')).toBeDisabled();
        await page.locator('#restart').click();
        await ready(page);
        await expect(transcript).not.toContainText('AI move:');
        expect(errors).toEqual([]);
    });
}

test('TicTacToe reaches a final result', async ({page}) => {
    await page.goto('/site/');
    for (let turn = 0; turn < 5; ++turn) {
        await ready(page);
        const lines = (await page.locator('#transcript').innerText()).split('\n');
        const preview = lines.filter(line => line.startsWith('Legal actions:')).at(-1);
        const coordinate = preview.match(/\((\d+), (\d+)\)/);
        expect(coordinate).not.toBeNull();
        await submit(page, `${coordinate[1]} ${coordinate[2]}`);
        await expect.poll(async () => !await page.locator('#move').isDisabled() ||
            await page.locator('#status').innerText() === 'Match ended').toBe(true);
        if (await page.locator('#status').innerText() === 'Match ended') {
            await expect(page.locator('#transcript')).toContainText(/You win!|AI wins\.|Draw\./);
            return;
        }
    }
    throw new Error('TicTacToe did not terminate after five human moves');
});

test('switching games cancels the previous AI worker', async ({page}) => {
    await page.goto('/site/');
    await page.locator('[data-game="3"]').click();
    await ready(page);
    await submit(page, '-1 2');
    await page.locator('[data-game="2"]').click();
    await ready(page);
    await expect(page.locator('#game-title')).toHaveText('Hex');
    await expect(page.locator('#transcript')).not.toContainText('captures:');
    await expect(page.locator('#transcript')).not.toContainText('AI move:');
});

test('mobile layout preserves board geometry without page overflow', async ({page}) => {
    await page.setViewportSize({width: 390, height: 844});
    await page.goto('/site/');
    await page.locator('[data-game="2"]').click();
    await ready(page);
    const dimensions = await page.evaluate(() => ({
        viewport: innerWidth,
        page: document.documentElement.scrollWidth,
        boardWhiteSpace: getComputedStyle(document.querySelector('.board-row')).whiteSpace,
    }));
    expect(dimensions.page).toBeLessThanOrEqual(dimensions.viewport);
    expect(dimensions.boardWhiteSpace).toBe('pre');
    await expect(page.locator('#submit')).toBeInViewport();
});