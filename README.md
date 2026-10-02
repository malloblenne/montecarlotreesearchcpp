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
