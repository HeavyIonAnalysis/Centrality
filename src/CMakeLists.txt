set(SOURCES
        BordersFinder.cpp
        BordersFinderHelper.cpp
        Getter.cpp
        BordersFinder2D.cpp
        Fitter.cpp
        FitterHelper.cpp
        )

string(REPLACE ".cpp" ".hpp" HEADERS "${SOURCES}")

set(DICT_FILE_NAME G__${PROJECT_NAME})
set(PCM_FILE_NAME lib${PROJECT_NAME})

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_library(Centrality SHARED ${SOURCES} ${DICT_FILE_NAME})
ROOT_GENERATE_DICTIONARY(${DICT_FILE_NAME} ${HEADERS} LINKDEF CentralityLinkDef.hpp)
target_link_libraries(Centrality ${ROOT_LIBRARIES})

install(TARGETS Centrality EXPORT CentralityTargets
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
        INCLUDES DESTINATION include
        )

install(
        FILES
        ${HEADERS}
        DESTINATION
        include/Centrality
        COMPONENT
        Devel
)

install(
        FILES
        "${CMAKE_CURRENT_BINARY_DIR}/${PCM_FILE_NAME}_rdict.pcm"
        DESTINATION
        lib
        OPTIONAL
)

install(
        FILES
        "${CMAKE_CURRENT_BINARY_DIR}/${PCM_FILE_NAME}.rootmap"
        DESTINATION
        lib
        OPTIONAL
)
