cmake_minimum_required(VERSION 2.8)
project(Centrality)
set(CMAKE_MACOSX_RPATH 1)
set(CMAKE_cpp_STANDARD 11)
set(CMAKE_BUILD_TYPE debug)
set(PROJECT_VERSION 1.0)

set(SOURCES
        src/BordersFinder.cpp
        src/BordersFinderHelper.cpp
        src/Getter.cpp
        src/BordersFinder2D.cpp
        src/Container.cpp
        glauber/Fitter.cpp
        glauber/FitterHelper.cpp
        )

string(REPLACE ".cpp" ".h" HEADERS "${SOURCES}")

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

# Telling CMake where to find the ROOT installation.
list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})
list(APPEND CMAKE_PREFIX_PATH "${CMAKE_SOURCE_DIR}/lib")

find_package(Boost REQUIRED COMPONENTS program_options)

#---Locate the ROOT package and defines a number of variables (e.g. ROOT_INCLUDE_DIRS)
find_package(ROOT REQUIRED COMPONENTS Core MathCore RIO Hist Tree)

# Define useful ROOT functions and macros (e.g. ROOT_GENERATE_DICTIONARY)
include(${ROOT_USE_FILE})
add_definitions(${ROOT_CXX_FLAGS})

include_directories(${CMAKE_SOURCE_DIR} ${ROOT_INCLUDE_DIRS})

set(DICT_FILE_NAME G__${PROJECT_NAME})
set(PCM_FILE_NAME lib${PROJECT_NAME})


ROOT_GENERATE_DICTIONARY(${DICT_FILE_NAME} ${HEADERS} LINKDEF src/CentralityLinkDef.h)
add_library(Centrality SHARED ${SOURCES} ${DICT_FILE_NAME})
target_link_libraries(Centrality ${ROOT_LIBRARIES})

# Create a main program using the library
add_executable(run_centrality_analysis run_centrality_analysis.cpp)
target_link_libraries(run_centrality_analysis Centrality ${ROOT_LIBRARIES} ${Boost_LIBRARIES})
target_include_directories(run_centrality_analysis PUBLIC src)
target_include_directories(run_centrality_analysis PRIVATE ${Boost_INCLUDE_DIRS})

add_executable(glauber glauber/main.cpp)
target_link_libraries(glauber ${ROOT_LIBRARIES} Centrality)

# install(FILES ${HEADERS} DESTINATION include)
# install(TARGETS Centrality DESTINATION lib)

# Packaging routine
# For complete explanation
# @see https://cmake.org/cmake/help/git-master/manual/cmake-packages.7.html#creating-packages
include(GenerateExportHeader)


generate_export_header(Centrality)
set_property(TARGET Centrality PROPERTY VERSION ${PROJECT_VERSION})
set_property(TARGET Centrality PROPERTY SOVERSION ${PROJECT_VERSION})
set_property(TARGET Centrality PROPERTY INTERFACE_Centrality_MAJOR_VERSION 1)
set_property(TARGET Centrality APPEND PROPERTY COMPATIBLE_INTERFACE_STRING INTERFACE_Centrality_MAJOR_VERSION)

install(TARGETS Centrality EXPORT CentralityTargets
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
        INCLUDES DESTINATION include
        )

install(
        FILES
        ${HEADERS}
        "${CMAKE_CURRENT_BINARY_DIR}/centrality_export.h"
        DESTINATION
        include/centrality
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

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
        "${CMAKE_CURRENT_BINARY_DIR}/Centrality/CentralityConfigVersion.cmake"
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY AnyNewerVersion
)

export(EXPORT CentralityTargets
        FILE "${CMAKE_CURRENT_BINARY_DIR}/Centrality/CentralityTargets.cmake")

set(ConfigPackageLocation lib/cmake/Centrality)

set(Centrality_INCLUDE_DIR "include")
set(Centrality_LIBRARY_DIR "lib")

configure_package_config_file(
        CentralityConfig.cmake.in ${CMAKE_CURRENT_BINARY_DIR}/Centrality/CentralityConfig.cmake
        INSTALL_DESTINATION ${ConfigPackageLocation}
        PATH_VARS Centrality_INCLUDE_DIR Centrality_LIBRARY_DIR
        #  [NO_SET_AND_CHECK_MACRO]
        #  [NO_CHECK_REQUIRED_COMPONENTS_MACRO]
        #  [INSTALL_PREFIX <path>]
)

install(EXPORT CentralityTargets
        FILE
        CentralityTargets.cmake
        DESTINATION
        ${ConfigPackageLocation}
        )

install(FILES
        "${CMAKE_CURRENT_BINARY_DIR}/Centrality/CentralityConfig.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/Centrality/CentralityConfigVersion.cmake"
        DESTINATION
        ${ConfigPackageLocation}
        COMPONENT
        Devel
        )

