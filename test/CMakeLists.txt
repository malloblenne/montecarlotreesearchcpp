cmake_minimum_required(VERSION 3.0.0)
project(MonteCarloTreeSearchCPPTest LANGUAGES CXX VERSION 0.1.0)

find_package(Catch2 REQUIRED)

include(CTest)
include(Catch)
# These tests can use the Catch2-provided main
add_executable(tests src/MonteCarloTreeSearchTest.cpp)
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)


#catch_discover_tests(tests)
