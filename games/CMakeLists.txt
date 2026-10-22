add_library(GamesLibrary STATIC
    tictactoe.cpp
    hex.cpp
    circleoflife.cpp)
target_include_directories(GamesLibrary PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(GamesLibrary PUBLIC MonteCarloTreeSearchCPP)