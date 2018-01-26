cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

project(glucose)

#--------------------------------------------------------------------------------------------------
# Configurable options:

option(STATIC_BINARIES "Link binaries statically." ON)
option(USE_SORELEASE   "Use SORELEASE in shared library filename." ON)

#--------------------------------------------------------------------------------------------------
# Library version:

set(GLUCOSE_SOMAJOR   4)
set(GLUCOSE_SOMINOR   1)
set(GLUCOSE_SORELEASE 0)

# Compute VERSION and SOVERSION:
if (USE_SORELEASE)
  set(GLUCOSE_VERSION ${GLUCOSE_SOMAJOR}.${GLUCOSE_SOMINOR}.${GLUCOSE_SORELEASE})
else()
  set(GLUCOSE_VERSION ${GLUCOSE_SOMAJOR}.${GLUCOSE_SOMINOR})
endif()
set(GLUCOSE_SOVERSION ${GLUCOSE_SOMAJOR})

#--------------------------------------------------------------------------------------------------
# Dependencies:

find_package(ZLIB REQUIRED)

include_directories(${ZLIB_INCLUDE_DIRS})
include_directories(${glucose_SOURCE_DIR}/glucose/)

#--------------------------------------------------------------------------------------------------
# Compile flags:

add_definitions(-D__STDC_FORMAT_MACROS -D__STDC_LIMIT_MACROS -O3 -DNDEBUG -std=c++11)

#--------------------------------------------------------------------------------------------------
# Build Targets:

set(GLUCOSE_LIB_SOURCES
    glucose/utils/Options.cc
    glucose/utils/System.cc
    glucose/core/Solver.cc
    glucose/simp/SimpSolver.cc
    glucose/parallel/MultiSolvers.cc
    glucose/parallel/SolverConfiguration.cc
    glucose/parallel/ClausesBuffer.cc
    glucose/parallel/SolverCompanion.cc
    glucose/parallel/SharedCompanion.cc
    glucose/parallel/ParallelSolver.cc)

add_library(glucose-lib-static STATIC ${GLUCOSE_LIB_SOURCES})
add_library(glucose-lib-shared SHARED ${GLUCOSE_LIB_SOURCES})

target_include_directories(glucose-lib-static PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(glucose-lib-shared PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(glucose-lib-static ${ZLIB_LIBRARY_RELEASE})
target_link_libraries(glucose-lib-shared ${ZLIB_LIBRARY_RELEASE})

add_executable(glucose glucose/simp/Main.cc)
add_executable(glucose-syrup glucose/parallel/Main.cc)

if(STATIC_BINARIES)
  target_link_libraries(glucose glucose-lib-static)
  target_link_libraries(glucose-syrup glucose-lib-static)
else()
  target_link_libraries(glucose glucose-lib-shared)
  target_link_libraries(glucose-syrup glucose-lib-shared)
endif()

set_target_properties(glucose-lib-static PROPERTIES OUTPUT_NAME "glucose")
set_target_properties(glucose-lib-shared
  PROPERTIES
    OUTPUT_NAME "glucose" 
    VERSION ${GLUCOSE_VERSION}
    SOVERSION ${GLUCOSE_SOVERSION})

set_target_properties(glucose       PROPERTIES OUTPUT_NAME "glucose")
set_target_properties(glucose-syrup PROPERTIES OUTPUT_NAME "glucose-syrup")

#--------------------------------------------------------------------------------------------------
# Installation targets:

install(TARGETS glucose-lib-static glucose-lib-shared glucose glucose-syrup 
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)

install(DIRECTORY glucose/mtl glucose/utils glucose/core glucose/simp
        DESTINATION include/glucose
        FILES_MATCHING PATTERN "*.h")
