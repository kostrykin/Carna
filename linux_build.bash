#!/bin/bash

set -ex

if [ -v CARNA_BUILD_DOCS ] && [ command -v doxygen >/dev/null ]; then
    export BUILD_DOC="ON"
else
    export BUILD_DOC="OFF"
fi

mkdir -p build/make_debug
mkdir -p build/make_release
cd build/make_debug

if [ "$BUILD" != "only_release" ]; then
    cd ../make_debug
    cmake -DCMAKE_BUILD_TYPE=Debug -DBUILD_DOC=$BUILD_DOC $CMAKE_ARGS $* ../..
    make
fi

if [ "$BUILD" != "only_debug" ]; then
    cd ../make_release
    cmake -DCMAKE_BUILD_TYPE=Release -DBUILD_DOC=$BUILD_DOC $CMAKE_ARGS $* ../..
    make
fi

echo
echo "** Ready to install."

if [ -z "$CARNA_NO_INSTALL" ]; then
    if [ "$BUILD" != "only_release" ]; then
        cd ../make_debug
        sudo make install
    fi
    
    if [ "$BUILD" != "only_debug" ]; then
        cd ../make_release
        sudo make install
    fi
fi
