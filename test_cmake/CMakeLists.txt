cmake_minimum_required( VERSION 3.5 )

project( CarnaTest )

include( FindPackageHandleStandardArgs )

find_package( Eigen3 REQUIRED )
include_directories( ${EIGEN3_INCLUDE_DIR} )

if( BUILD STREQUAL "debug" )
    set( CMAKE_BUILD_TYPE Debug )
elseif( BUILD STREQUAL "release" )
    set( CMAKE_BUILD_TYPE Release )
endif()

find_package( Carna 3.3 REQUIRED COMPONENTS ${BUILD} )
include_directories( ${Carna_INCLUDE_DIR} )
set( Carna_VERSION ${FOUND_VERSION} )

add_executable( test test.cpp )
target_link_libraries( test -lGLU ${Carna_LIBRARIES} )
