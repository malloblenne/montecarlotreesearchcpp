let started = false;

self.onmessage = async ({data}) => {
    if (data.type === 'input' && self.pendingInput) {
        const resolve = self.pendingInput;
        self.pendingInput = null;
        resolve(data.line);
        return;
    }
    if (data.type !== 'start' || started) return;
    started = true;
    try {
        importScripts('Main.js');
        const module = await createMcts({
            noInitialRun: true,
            print: text => self.postMessage({type: 'output', text}),
            printErr: text => self.postMessage({type: 'error-output', text}),
            onAbort: reason => self.postMessage({type: 'fatal', text: String(reason)}),
            onExit: code => self.postMessage({type: 'exit', code}),
        });
        self.postMessage({type: 'ready'});
        module.callMain([]);
    } catch (error) {
        if (error?.name === 'ExitStatus') return;
        self.postMessage({type: 'fatal', text: error.message || String(error)});
    }
};