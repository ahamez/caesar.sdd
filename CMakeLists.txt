cmake_minimum_required (VERSION 2.8.9)

#--------------------------------------------------------------------------------------------------#

project (caesar.sdd)

#--------------------------------------------------------------------------------------------------#

option (LIBSDD_PATH "Path to libsdd")
option (PACKED "Pack structs in memory (experimental)" OFF)
option (STATIC_BOOST "Force usage of static Boost libraries OFF")

#--------------------------------------------------------------------------------------------------#

if (STATIC_BOOST)
  set(Boost_USE_STATIC_LIBS ON)
endif ()
find_package(Boost 1.59.0 REQUIRED COMPONENTS program_options context coroutine system thread)

if (NOT Boost_FOUND)
  message(FATAL_ERROR "Boost not found")
endif ()

message("-- Boost Include: ${Boost_INCLUDE_DIR}")
message("-- Boost Libraries: ${Boost_LIBRARY_DIRS}")

#--------------------------------------------------------------------------------------------------#

if (PACKED)
  add_definitions("-DLIBSDD_PACKED")
endif ()

#--------------------------------------------------------------------------------------------------#

include(CheckCXXCompilerFlag)
check_cxx_compiler_flag("-std=c++14" COMPILER_SUPPORTS_CXX14)
if(NOT COMPILER_SUPPORTS_CXX14)
  message(FATAL_ERROR "The compiler ${CMAKE_CXX_COMPILER} has no C++14 support.")
endif ()

#--------------------------------------------------------------------------------------------------#

if (NOT CMAKE_BUILD_TYPE)
  message(STATUS "No build type selected, default to Release")
  set(CMAKE_BUILD_TYPE "Release")
endif()

find_path(LIBSDD_INCLUDE_DIR sdd/sdd.hh PATHS ${LIBSDD_PATH})
if (NOT LIBSDD_INCLUDE_DIR)
  message(FATAL_ERROR "libsdd not found")
endif ()

find_package (Threads)

set(CMAKE_CXX_FLAGS "-Wall -Wextra -std=c++14 ${CMAKE_CXX_FLAGS}")

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  if (CMAKE_BUILD_TYPE STREQUAL "Release")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -flto")
  endif ()
endif ()

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")
if (COVERAGE AND CMAKE_BUILD_TYPE MATCHES Debug)
  include(CodeCoverage)
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} --coverage")
endif()

#--------------------------------------------------------------------------------------------------#

include_directories(SYSTEM "${Boost_INCLUDE_DIRS}")
include_directories(SYSTEM "${PROJECT_SOURCE_DIR}/ext")
include_directories("${PROJECT_SOURCE_DIR}/src")
include_directories("${LIBSDD_INCLUDE_DIR}")

#--------------------------------------------------------------------------------------------------#

add_subdirectory(src)

#--------------------------------------------------------------------------------------------------#
