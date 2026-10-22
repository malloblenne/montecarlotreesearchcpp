const {defineConfig} = require('@playwright/test');

module.exports = defineConfig({
    testDir: './tests',
    timeout: 120000,
    workers: 1,
    use: {
        baseURL: 'http://127.0.0.1:8766',
        viewport: {width: 1440, height: 1000},
        screenshot: 'only-on-failure',
        trace: 'retain-on-failure',
    },
    reporter: [['list'], ['junit', {outputFile: 'test-results/browser-junit.xml'}]],
    webServer: {
        command: 'python3 -m http.server 8766 --bind 127.0.0.1 --directory ../build-web',
        url: 'http://127.0.0.1:8766/site/',
        reuseExistingServer: false,
    },
});