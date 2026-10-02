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

The example currently plays Circle of Life. The root program is updated in the
following CLI task. Generated binaries and CMake files stay in `build/`.
