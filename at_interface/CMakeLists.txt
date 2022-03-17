set(SOURCES
        CentralityFiller.cpp
        #        CentralityTask.cpp
        #        CentralityTaskTracks.cpp
        #        CentralityTaskPsd.cpp
        )

string(REPLACE ".cpp" ".hpp" HEADERS "${SOURCES}")

set(DICT_FILE_NAME G__ATCentrality)
set(PCM_FILE_NAME libATCentrality)

include_directories(${CMAKE_SOURCE_DIR}/src ${CMAKE_CURRENT_SOURCE_DIR} ${AnalysisTree_INCLUDE_DIR})

add_library(ATCentrality SHARED ${SOURCES} ${DICT_FILE_NAME})
ROOT_GENERATE_DICTIONARY(${DICT_FILE_NAME} ${HEADERS} LINKDEF ATCentralityLinkDef.hpp)
target_link_libraries(ATCentrality Centrality AnalysisTreeBase AnalysisTreeInfra ${ROOT_LIBRARIES})

#install(TARGETS Centrality EXPORT CentralityTargets
#        LIBRARY DESTINATION lib
#        ARCHIVE DESTINATION lib
#        RUNTIME DESTINATION bin
#        INCLUDES DESTINATION include
#        )
#
#install(
#        FILES
#        ${HEADERS}
#        DESTINATION
#        include/Centrality
#        COMPONENT
#        Devel
#)
#
#install(
#        FILES
#        "${CMAKE_CURRENT_BINARY_DIR}/${PCM_FILE_NAME}_rdict.pcm"
#        DESTINATION
#        lib
#        OPTIONAL
#)
#
#install(
#        FILES
#        "${CMAKE_CURRENT_BINARY_DIR}/${PCM_FILE_NAME}.rootmap"
#        DESTINATION
#        lib
#        OPTIONAL
#)
