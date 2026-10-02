cmake_minimum_required(VERSION 3.10)
project(MonteCarloTreeSearchCPP VERSION 0.1.0)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(ENABLE_COVERAGE "Instrument GCC builds for gcov coverage" OFF)
if(ENABLE_COVERAGE AND NOT CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
	message(FATAL_ERROR "ENABLE_COVERAGE requires GCC and gcov")
endif()
if(ENABLE_COVERAGE)
	find_program(GCOV_EXECUTABLE gcov)
	if(NOT GCOV_EXECUTABLE)
		message(FATAL_ERROR "ENABLE_COVERAGE requires gcov on PATH")
	endif()
	set(COVERAGE_COMMAND ${GCOV_EXECUTABLE} CACHE FILEPATH "Coverage tool" FORCE)
	configure_file(test/CTestCustom.cmake.in CTestCustom.cmake COPYONLY)
endif()

add_subdirectory(src)
add_subdirectory(games)


add_executable(Main main.cpp)
target_link_libraries(Main PRIVATE GamesLibrary)

include(CTest)
if(BUILD_TESTING)
	add_subdirectory(test)
endif()
add_subdirectory(example)