cmake_minimum_required( VERSION 3.5 )

project( LibCarnaTest )

include( FindPackageHandleStandardArgs )

find_package( Eigen3 REQUIRED )
include_directories( ${EIGEN3_INCLUDE_DIR} )

if( BUILD STREQUAL "debug" )
    set( CMAKE_BUILD_TYPE Debug )
elseif( BUILD STREQUAL "release" )
    set( CMAKE_BUILD_TYPE Release )
endif()

find_package( LibCarna 3.4 MODULE REQUIRED COMPONENTS ${BUILD} )
include_directories( ${LibCarna_INCLUDE_DIR} )
set( LibCarna_VERSION ${FOUND_VERSION} )

add_executable( test test.cpp )
target_link_libraries( test -lGLU ${LibCarna_LIBRARIES} )
