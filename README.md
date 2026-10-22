# montecarlotreesearchcpp
C++ Library for Monte Carlo Tree Search for (Abstract) Combinatorial Games

## Browser Games

Play at [MCTS Game Room](https://malloblenne.github.io/montecarlotreesearchcpp/).
The browser runs the root `main.cpp` and the same games/search libraries compiled
to WebAssembly, not a JavaScript reimplementation. Select a game, type coordinates,
and press Enter or **Play move**. **Restart game** starts a fresh match; switching
games cancels the previous match. Input formats and legal actions come from the
same C++ CLI. The AI runs in a Web Worker so the page remains responsive.

### Build the Website

Install and activate [Emscripten SDK](https://emscripten.org/docs/getting_started/downloads.html)
3.1.74 (the version pinned in CI), then build through CMake:

```bash
emcmake cmake -S . -B build-web -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTING=OFF
cmake --build build-web --parallel 4
python3 -m http.server 8765 --directory build-web/site
```

Open `http://localhost:8765`. CMake emits `Main.js`, `Main.wasm`, and all frontend
assets into `build-web/site/`. Serve the files over HTTP; opening `index.html`
directly with `file://` will not load the worker and Wasm correctly. All assets
use relative paths, including on a repository-specific Pages URL. No external
fonts, scripts, server game logic, or cross-origin isolation headers are required.
Native CMake builds keep their existing CLI, examples, tests, and coverage.

Asyncify suspends C++ only while awaiting input, and C++ exception handling remains
enabled. The worker is terminated on restart/switch, so even an in-progress AI
search can be canceled. Initial download and larger-game AI turns can take longer
on low-powered devices.

## Build

On Linux, install Bash, CMake 3.10 or newer, GNU Make, and a C++17 compiler
(for example GCC). From the repository root, run:

```bash
./build.sh
```

The script creates `build/`, configures with `cmake ..`, and builds the full
project using all available CPU cores. It also works from another working
directory. Optional arguments are forwarded to CMake:

```bash
./build.sh -DCMAKE_BUILD_TYPE=Release
```

Run the root program or the AI-versus-AI example from the repository root:

```bash
./build/Main
./build/example/main
```

The example currently plays Circle of Life with two AI players. Generated
binaries and CMake files stay in `build/`.

## Play Against the AI

Run `./build/Main` and choose `1` (TicTacToe), `2` (Hex), or `3` (Circle of Life).
You play first as X (`1`); the AI plays O (`2`); empty cells are `0`.

- TicTacToe: enter `row col`, both zero-based from `0` through `2`.
- Hex: enter `row col`, both zero-based from `0` through `10`. X connects the
	top and bottom edges; O connects the left and right edges.
- Circle of Life: enter axial `q r`, both from `-4` through `4`, with `q+r`
	also in that range. Rows are labeled `r`, and each row shows its `q` range.
	Occupied cells and placements that create groups larger than four are illegal.

For example, enter `1 2` in TicTacToe or `-1 2` in Circle of Life. The program
prints the expected format before play and shows at most ten legal actions per
human turn. Additional legal coordinates remain selectable even when not shown.
The `n:` values in previews are internal legal-action indices, not keyboard input.
Invalid input is rejected without consuming a turn. Enter `q` or send EOF to
exit from the menu or a human turn. AI budgets are 1,000 iterations for TicTacToe
and Hex, and 100 for Circle of Life.

## Layout

- `src/`: the Monte Carlo tree search library (`MonteCarloTreeSearchCPP`).
- `games/`: TicTacToe, Hex, and Circle of Life, compiled once into the
	`GamesLibrary` static library (`build/games/libGamesLibrary.a`).
- `main.cpp`: root executable, linked with the games library.
- `example/`: AI-versus-AI example, also linked with the same games library.
- `test/`: library tests.

## Tests

Tests build by default using the bundled Catch2 header; no package download is
needed. Run them with CTest, which prints failure output:

```bash
./build.sh -DCMAKE_BUILD_TYPE=Debug
cd build
ctest --output-on-failure
```

The suite checks search-library statistics, iteration reset, tree expansion,
backpropagation, simulation, UCB selection, and repeated factory searches.
It also checks all three games' coordinate mappings and the CLI. Search tests
use a deterministic toy game rather than asserting random AI choices.
To build without tests, pass `-DBUILD_TESTING=OFF` to `build.sh`.

## Coverage

Coverage is opt-in and requires GCC and its matching `gcov`. Start with a fresh
`build/` directory to avoid mixing existing coverage counters or build options:
preserve or remove previous build output before running this sequence.

```bash
./build.sh -DCMAKE_BUILD_TYPE=Debug -DBUILD_TESTING=ON -DENABLE_COVERAGE=ON
cd build
ctest --output-on-failure
ctest -T Coverage
```

The last command invokes **gcov**, displays line-coverage totals, and writes
CTest coverage XML and annotated output under `Testing/`. Coverage excludes
games and test code to focus on the `src` library. Normal builds have no
instrumentation (`-DENABLE_COVERAGE=OFF`).

For HTML and standard Cobertura XML reports, install `gcovr` and run from the
repository root after the instrumented tests:

```bash
gcovr --root . --filter 'src/' --object-directory build --txt
gcovr --root . --filter 'src/' --object-directory build --xml-pretty -o build/coverage.xml
gcovr --root . --filter 'src/' --object-directory build --html-details -o build/coverage.html
```

gcovr formats gcov data, including branch coverage. No minimum coverage threshold
is enforced; the report exposes gaps such as time-based criteria and diagnostic
logging for future tests. gcovr also includes instrumented files with no execution
data, whereas CTest's native coverage may omit them. Its totals can therefore
be lower even though both reports are limited to `src/`.

## GitHub Actions

The **Build, Test, and Coverage** workflow runs on pushes, pull requests, and
manual dispatch, with two independent Linux jobs:

- **Linux Release** builds and tests the full project, then uploads the
	`mcts-linux-x86_64` binary artifact and `release-test-results` (JUnit XML and
	CTest logs).
- **Linux GCC Coverage** runs instrumented tests, prints native gcov output,
	and uploads `test-and-coverage-reports`: JUnit XML, Cobertura XML, coverage
	text, HTML details, and CTest logs. The job summary shows passed/failed checks
	and `src/` line/branch coverage. Available reports are retained on test failure.

Download artifacts from the workflow run's **Artifacts** section. Extract the
archive inside the binary artifact and run:

```bash
tar -xzf mcts-linux-x86_64.tar.gz
./Main
./example/main
```

Binaries target x86-64 Ubuntu 24.04 and depend on compatible system C/C++
runtime libraries; they are not fully static or cross-platform binaries.
Static libraries are in the archive's `src/` and `games/` directories; use the
repository headers when linking them. No GitHub release publication or external
coverage service is configured. Open `coverage.html` alongside its companion
HTML files to inspect per-file coverage. JUnit counts in the GitHub summary
represent the three CTest checks, not individual Catch2 assertions.

On recent CMake versions (3.21 or newer), local JUnit output is available with
`ctest --output-on-failure --output-junit test-results.xml` inside `build/`.
Older CMake versions can produce Catch2 JUnit output directly with
`./build/test/tests --reporter junit --out build/catch2-results.xml` from the root;
that report does not include the separate CLI smoke check.

## Browser Tests

Emscripten builds omit the native example and Catch2 targets; native CTest
behavior is unchanged. Browser regressions use Node.js 22 and Playwright:

```bash
cd web
npm ci
npx playwright install --with-deps chromium
npm test
```

Build the Wasm site first. Tests start their own HTTP server on port 8766 and
load the site under `/site/` to verify subpath asset loading. They exercise every
game's actual Wasm AI, invalid input, the ten-action preview limit, quitting,
restart, terminal results, worker cancellation, and mobile layout. JUnit XML
and failure screenshots/traces are written under `web/test-results/`.
Current Playwright browsers require a supported OS, such as Ubuntu 24.04;
the project's Debian 11 container can still build Wasm and serve the site.

## GitHub Pages

The **WebAssembly and GitHub Pages** workflow builds and tests on pushes and
pull requests. Successful builds publish from the default branch, an explicitly
selected `PAGES_PUBLISH_BRANCH` repository variable, or manual workflow dispatch.
Pull requests never deploy. Each run uploads browser test reports and the
self-contained Pages site artifact; deployment uses GitHub's Pages Actions.

In repository **Settings > Pages**, set **Source** to **GitHub Actions**.
The `github-pages` environment must permit the publishing branch. Push the
changes to the default branch, or set **Settings > Secrets and variables >
Actions > Variables > PAGES_PUBLISH_BRANCH** to a branch intended for publication.
Leave the variable unset for default-branch-only publishing. The deployment job
provides the live URL after it succeeds:

<https://malloblenne.github.io/montecarlotreesearchcpp/>

The native release and gcov workflow remains separate. Generated Wasm files are
uploaded as deployment artifacts, not committed into the source tree.
