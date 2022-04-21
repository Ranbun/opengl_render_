################################################################################
# CMake minimum version required
cmake_minimum_required(VERSION 3.0)

################################################################################
# RapidJSON source dir
set( RapidJSON_SOURCE_DIR "H:/OpenSources/vcpkg/buildtrees/rapidjson/src/c72ed4364c-35829b36f2.clean")

################################################################################
# RapidJSON build dir
set( RapidJSON_DIR "H:/OpenSources/vcpkg/buildtrees/rapidjson/x64-windows-rel")

################################################################################
# Compute paths
get_filename_component(RapidJSON_CMAKE_DIR "${CMAKE_CURRENT_LIST_FILE}" PATH)

set( RapidJSON_INCLUDE_DIR  "${RapidJSON_CMAKE_DIR}/../../include" )
set( RapidJSON_INCLUDE_DIRS  "${RapidJSON_CMAKE_DIR}/../../include" )
message(STATUS "RapidJSON found. Headers: ${RapidJSON_INCLUDE_DIRS}")

if(NOT TARGET rapidjson)
  add_library(rapidjson INTERFACE IMPORTED)
  set_property(TARGET rapidjson PROPERTY
    INTERFACE_INCLUDE_DIRECTORIES ${RapidJSON_INCLUDE_DIRS})
endif()

set(RAPIDJSON_INCLUDE_DIRS "${RapidJSON_INCLUDE_DIRS}")
