cmake_minimum_required(VERSION 3.10)
project(MonteCarloTreeSearchCPP VERSION 0.1.0)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory(src)
list(APPEND EXTRA_LIBS MonteCarloTreeSearchCPP)


add_executable(Main main.cpp)
target_link_libraries(Main PUBLIC ${EXTRA_LIBS})

include(CTest)
enable_testing()
#set(CPACK_PROJECT_NAME ${PROJECT_NAME})
#set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
#include(CPack)
add_subdirectory(test)