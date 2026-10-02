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

if(EMSCRIPTEN)
	target_compile_options(MonteCarloTreeSearchCPP PRIVATE -fexceptions)
	target_compile_options(GamesLibrary PRIVATE -fexceptions)
	target_compile_options(Main PRIVATE -fexceptions)
	set_target_properties(Main PROPERTIES
		SUFFIX ".js"
		RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/site")
	set_property(TARGET Main APPEND_STRING PROPERTY LINK_FLAGS
		" -sASYNCIFY=1 -sASYNCIFY_STACK_SIZE=131072 -sALLOW_MEMORY_GROWTH=1 -sMODULARIZE=1 -sEXPORT_NAME=createMcts -sENVIRONMENT=worker -sEXIT_RUNTIME=1 -sDISABLE_EXCEPTION_CATCHING=0 -sEXPORTED_RUNTIME_METHODS=callMain")
	file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/site")
	foreach(asset index.html style.css app.js worker.js)
		configure_file("web/${asset}" "site/${asset}" COPYONLY)
	endforeach()
endif()

include(CTest)
if(BUILD_TESTING AND NOT EMSCRIPTEN)
	add_subdirectory(test)
endif()
if(NOT EMSCRIPTEN)
	add_subdirectory(example)
endif()