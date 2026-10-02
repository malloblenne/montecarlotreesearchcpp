cmake_minimum_required(VERSION 3.10)
project(MonteCarloTreeSearchCPP VERSION 0.1.0)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(src)
add_subdirectory(games)


add_executable(Main main.cpp)
target_link_libraries(Main PRIVATE GamesLibrary)

include(CTest)
#enable_testing()
#set(CPACK_PROJECT_NAME ${PROJECT_NAME})
#set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
#include(CPack)
#add_subdirectory(test)
add_subdirectory(example)