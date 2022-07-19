cmake_minimum_required(VERSION 3.19)
project(stlbfgs LANGUAGES C CXX)

add_library(stlbfgs)

find_package(OpenMP)
if(OPENMP_FOUND)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

if(OpenMP_CXX_FOUND)
    target_link_libraries(stlbfgs OpenMP::OpenMP_CXX)
endif()

set(source_files 
 "${CMAKE_CURRENT_SOURCE_DIR}/src/linesearch.cpp"
 "${CMAKE_CURRENT_SOURCE_DIR}/src/stlbfgs.cpp")

set(header_files
  "${CMAKE_CURRENT_SOURCE_DIR}/include/stlbfgs/linesearch.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/include/stlbfgs/stlbfgs.h")

target_sources(stlbfgs PRIVATE ${source_files})

set_target_properties(stlbfgs
  PROPERTIES
  PUBLIC_HEADER "${header_files}")

target_include_directories(stlbfgs PUBLIC 
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)
  
set_target_properties(stlbfgs
  PROPERTIES
  CXX_STANDARD 17
  CXX_STANDARD_REQUIRED ON
  CXX_EXTENSIONS OFF)