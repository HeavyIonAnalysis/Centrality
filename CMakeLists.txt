cmake_minimum_required(VERSION 3.14)
project(Centrality)
set(CMAKE_MACOSX_RPATH 1)
set(PROJECT_VERSION 1.0)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

set(Centrality_BUNDLED_AT OFF CACHE BOOL "Build AnalysisTree")
set(Centrality_BUNDLED_AT_VERSION "v2.2.7" CACHE STRING "Version of AnalysisTree to build")
set(Centrality_BUNDLED_AT_GIT_SHALLOW ON CACHE BOOL "Use CMake GIT_SHALLOW option")
set(Centrality_BUILD_TESTS ON CACHE BOOL "Build tests for the Centrality")

if(NOT DEFINED CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17)
endif()

set(CMAKE_CXX_STANDARD_REQUIRED ON)
message(STATUS "Using C++${CMAKE_CXX_STANDARD}")

# by default build optimized code with debug symbols
if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Release)
endif ()

# in DEBUG mode make verbose Makefile
if (CMAKE_BUILD_TYPE MATCHES DEBUG OR CMAKE_BUILD_TYPE MATCHES Debug)
    set(CMAKE_VERBOSE_MAKEFILE ON)
endif ()

set(CMAKE_CXX_FLAGS_DEBUG "-O0 -ggdb -DDEBUG -D__DEBUG -Wall -Wextra")
set(CMAKE_CXX_FLAGS_Debug "-O0 -ggdb -DDEBUG -D__DEBUG -Wall -Wextra")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -march=native -ftree-vectorize -ffast-math -DNODEBUG")
set(CMAKE_CXX_FLAGS_Release "-O3 -march=native -ftree-vectorize -ffast-math -DNODEBUG")
message(STATUS "Using CXX flags for ${CMAKE_BUILD_TYPE}: ${CMAKE_CXX_FLAGS_${CMAKE_BUILD_TYPE}}")

# Telling CMake where to find the ROOT installation.
list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})
list(APPEND CMAKE_PREFIX_PATH ${ROOTSYS})

#---Locate the ROOT package and defines a number of variables (e.g. ROOT_INCLUDE_DIRS)
find_package(ROOT REQUIRED COMPONENTS Core MathCore RIO Hist Tree)

# Define useful ROOT functions and macros (e.g. ROOT_GENERATE_DICTIONARY)
message(STATUS "Using ROOT: ${ROOT_VERSION} <${ROOT_CONFIG}>")
include_directories(${CMAKE_SOURCE_DIR} ${ROOT_INCLUDE_DIR} ${ROOT_INCLUDE_DIRS})
include(${ROOT_USE_FILE})

include(AnalysisTree)

if (AnalysisTree_FOUND OR Centrality_BUNDLED_AT)
    add_subdirectory(at_interface)
endif ()

add_subdirectory(src)
add_subdirectory(tasks)
include(TestsSetupCentrality)

install(
        FILES
        "${CMAKE_CURRENT_BINARY_DIR}/${PCM_FILE_NAME}.rootmap"
        DESTINATION
        lib
        OPTIONAL
)

configure_file(cmake/CentralityConfig.sh.in ${CMAKE_BINARY_DIR}/CentralityConfig.sh)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
        "${CMAKE_CURRENT_BINARY_DIR}/Centrality/CentralityConfigVersion.cmake"
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY AnyNewerVersion
)

export(EXPORT CentralityTargets
        FILE "${CMAKE_CURRENT_BINARY_DIR}/Centrality/CentralityTargets.cmake")

message(STATUS "CentralityTargets: ${CentralityTargets}")

set(ConfigPackageLocation lib/cmake/Centrality)

set(Centrality_INCLUDE_DIR "include")
set(Centrality_LIBRARY_DIR "lib")

configure_package_config_file(
        cmake/CentralityConfig.cmake.in ${CMAKE_CURRENT_BINARY_DIR}/Centrality/CentralityConfig.cmake
        INSTALL_DESTINATION ${ConfigPackageLocation}
        PATH_VARS Centrality_INCLUDE_DIR Centrality_LIBRARY_DIR
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

install(FILES
        ${CMAKE_BINARY_DIR}/CentralityConfig.sh
        DESTINATION
        bin
        COMPONENT
        Devel
        )
