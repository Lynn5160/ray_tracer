cmake_minimum_required( VERSION 2.6 )
project( pRat )

#=====
# General
set( CMAKE_BUILD_TYPE Release )
set( CMAKE_CXX_COMPILER g++ )

include_directories(${CMAKE_SOURCE_DIR}/src)

#=====
# Build
add_executable( pRat
  ${CMAKE_SOURCE_DIR}/src/main.cpp
  )

set_target_properties( pRat
  PROPERTIES
  LINKER_LANGUAGE CXX
  PREFIX ""
  OUTPUT_NAME "pRat"
  )
