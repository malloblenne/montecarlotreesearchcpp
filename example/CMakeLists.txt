cmake_minimum_required(VERSION 3.0.0)
project(ExampleUseMCTLibrary LANGUAGES CXX VERSION 0.1.0)

add_executable(main main.cpp tictactoe.cpp)
target_compile_features(main PRIVATE cxx_std_17)

target_link_libraries(main PRIVATE MonteCarloTreeSearchCPP)

