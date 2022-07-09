add_library(MonteCarloTreeSearchCPP 
            montecarlotreesearch.cpp
            timecriteria.cpp
            treesearchnode.cpp)

# state that anybody linking to us needs to include the current source dir
# to find the header files, while we don't.
target_include_directories(MonteCarloTreeSearchCPP
          INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
          )

# install rules
install(TARGETS MonteCarloTreeSearchCPP DESTINATION lib)
install(FILES action.h
              backpropagationpolicy.h
              expansionpolicy.h
              gamestate.h
              montecarlotreesearch.h
              resourcecriteria.h
              selectionpolicy.h
              simulationpolicy.h
              timecriteria.h
              treesearch.h
              treesearchnode.h  DESTINATION include)
