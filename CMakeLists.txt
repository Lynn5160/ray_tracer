cmake_minimum_required( VERSION 2.6 )
project( pRat )

#=====
# General
set( CMAKE_BUILD_TYPE Release )
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")

find_package(SDL2 REQUIRED)

set(PLATFORM_LIBS "-L/usr/local/lib -lSDL2 -lm -liconv -Wl,-framework,CoreAudio -Wl,-framework,AudioToolbox -Wl,-framework,ForceFeedback -lobjc -Wl,-framework,CoreVideo -Wl,-framework,Cocoa -Wl,-framework,Carbon -Wl,-framework,IOKit -Wl,-weak_framework,QuartzCore -Wl,-weak_framework,Metal")

include_directories(
  ${CMAKE_SOURCE_DIR}/src
  )

#=====
# Build
add_executable( pRat
  ${CMAKE_SOURCE_DIR}/src/main.cpp
  )

link_directories( /opt/local/lib )

set_target_properties( pRat
  PROPERTIES 
  LINKER_LANGUAGE CXX
  PREFIX ""
  OUTPUT_NAME "../bin/pRat"
  )

target_link_libraries( pRat ${SDL2_LIBRARIES} ${PLATFORM_LIBS})
