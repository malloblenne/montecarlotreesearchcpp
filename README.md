# montecarlotreesearchcpp
C++ Library for Monte Carlo Tree Search for (Abstract) Combinatorial Games

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
logging for future tests.
