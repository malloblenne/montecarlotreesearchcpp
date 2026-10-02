cmake_minimum_required(VERSION 3.10)
project(ExampleUseMCTLibrary LANGUAGES CXX VERSION 0.1.0)
# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(main main.cpp tictactoe.cpp hex.cpp circleoflife.cpp)
target_compile_features(main PRIVATE cxx_std_17)

target_link_libraries(main PRIVATE MonteCarloTreeSearchCPP)

