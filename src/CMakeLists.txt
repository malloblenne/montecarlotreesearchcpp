add_library(MonteCarloTreeSearchCPP 
            simplebackpropagationpolicy.cpp
            simpleexpansionpolicy.cpp
            simpleselectionpolicy.cpp
            simplesimulationpolicy.cpp
            simplestatisticsnode.cpp
            timecriteria.cpp
            treesearchnode.cpp
            montecarlotreesearch.cpp
            upperconfidenceboundbestchild.cpp)

# state that anybody linking to us needs to include the current source dir
# to find the header files, while we don't.
target_include_directories(MonteCarloTreeSearchCPP
          INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
          )

# install rules
install(TARGETS MonteCarloTreeSearchCPP DESTINATION lib)
install(FILES action.h
              backpropagationpolicy.h
              bestchild.h
              expansionpolicy.h
              gamestate.h
              montecarlotreesearch.h
              resourcecriteria.h
              selectionpolicy.h
              simplebackpropagationpolicy.h
              simpleexpansionpolicy.h
              simpleselectionpolicy.h
              simulationpolicy.h
              simplesimulationpolicy.h
              simplestatisticsnode.h
              statisticsnode.h
              timecriteria.h
              treesearch.h
              treesearchnode.h
              upperconfidenceboundbestchild.h  DESTINATION include)
