const transcript = document.querySelector('#transcript');
const input = document.querySelector('#move');
const submit = document.querySelector('#submit');
const status = document.querySelector('#status');
const gameButtons = [...document.querySelectorAll('[data-game]')];
const names = {1: 'TicTacToe', 2: 'Hex', 3: 'Circle of Life'};
let worker;
let selectedGame = '1';
let selecting = true;
let waiting = false;
let restoreFocus = false;

function append(text, kind = '') {
    const nearBottom = transcript.scrollHeight - transcript.scrollTop - transcript.clientHeight < 80;
    const line = document.createElement('p');
    const boardRow = /^\s*(?:\d{1,2}\s+:|r=|col:|\\ \/|\d(?:\s+\d)+\s*$)/.test(text);
    line.className = `log-line ${kind}${boardRow ? ' board-row' : ''}`;
    line.textContent = text;
    transcript.append(line);
    while (transcript.children.length > 500) transcript.firstElementChild.remove();
    if (nearBottom) transcript.scrollTop = transcript.scrollHeight;
}

function setState(text, state = '') {
    status.textContent = text;
    status.dataset.state = state;
}

function setWaiting(value) {
    waiting = value;
    input.disabled = !value;
    submit.disabled = !value;
}

function startGame(game) {
    worker?.terminate();
    selectedGame = game;
    selecting = true;
    restoreFocus = false;
    setWaiting(false);
    transcript.replaceChildren();
    input.value = '';
    input.placeholder = game === '3' ? 'q r' : 'row col';
    document.querySelector('#input-label').textContent = game === '3' ? 'Axial coordinates (q r)' : 'Board coordinates (row col)';
    document.querySelector('#game-title').textContent = names[game];
    for (const button of gameButtons) button.setAttribute('aria-pressed', String(button.dataset.game === game));
    setState('Loading engine');
    try {
        const activeWorker = new Worker('worker.js');
        worker = activeWorker;
        activeWorker.onerror = event => {
            if (worker !== activeWorker) return;
            setWaiting(false);
            setState('Engine unavailable', 'error');
            append(event.message || 'Could not load the game engine.', 'error');
        };
        activeWorker.onmessage = ({data}) => {
            if (worker !== activeWorker) return;
            if (data.type === 'output' || data.type === 'error-output') {
                append(data.text, data.type === 'error-output' ? 'error' : '');
            } else if (data.type === 'ready') {
                setState('Starting match');
            } else if (data.type === 'input-request') {
                if (selecting) {
                    selecting = false;
                    activeWorker.postMessage({type: 'input', line: selectedGame});
                    return;
                }
                setWaiting(true);
                setState('Your turn');
                if (restoreFocus) input.focus({preventScroll: true});
                restoreFocus = false;
            } else if (data.type === 'exit') {
                setWaiting(false);
                setState(data.code === 0 ? 'Match ended' : 'Engine stopped', data.code === 0 ? '' : 'error');
            } else if (data.type === 'fatal') {
                setWaiting(false);
                setState('Engine unavailable', 'error');
                append(data.text, 'error');
            }
        };
        activeWorker.postMessage({type: 'start'});
    } catch (error) {
        setState('Engine unavailable', 'error');
        append(error.message, 'error');
    }
}

document.querySelector('#move-form').addEventListener('submit', event => {
    event.preventDefault();
    if (!waiting) return;
    const line = input.value.trim();
    restoreFocus = document.activeElement === input || document.activeElement === submit;
    append(`> ${line}`, 'command');
    input.value = '';
    setWaiting(false);
    setState(line.toLowerCase() === 'q' ? 'Ending match' : 'Engine running');
    worker.postMessage({type: 'input', line});
});
for (const button of gameButtons) button.addEventListener('click', () => startGame(button.dataset.game));
document.querySelector('#restart').addEventListener('click', () => startGame(selectedGame));
startGame(selectedGame);