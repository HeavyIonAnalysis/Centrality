# Create a main program using the library
add_executable(main main.cpp)
target_link_libraries(main Centrality ${ROOT_LIBRARIES})
target_include_directories(main PUBLIC ${CMAKE_SOURCE_DIR}/src)

add_executable(glauber glauber.cpp)
target_link_libraries(glauber ${ROOT_LIBRARIES} Centrality)
target_include_directories(glauber PUBLIC ${CMAKE_SOURCE_DIR}/src)

if (AnalysisTree_FOUND OR Centrality_BUNDLED_AT)
    add_executable(fill_centrality fill_centrality.cpp)
    target_link_libraries(fill_centrality Centrality ATCentrality AnalysisTreeBase AnalysisTreeInfra ${ROOT_LIBRARIES})
    target_include_directories(fill_centrality PUBLIC ${CMAKE_SOURCE_DIR}/src ${CMAKE_SOURCE_DIR}/at_interface ${PROJECT_INCLUDE_DIRECTORIES})
    install(TARGETS fill_centrality RUNTIME DESTINATION bin)
endif ()

install(TARGETS glauber RUNTIME DESTINATION bin)
install(TARGETS main RUNTIME DESTINATION bin)