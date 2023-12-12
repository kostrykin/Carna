#!/bin/bash

set -e

if command -v doxygen &> /dev/null; then
    export BUILD_DOC="ON"
else    
    export BUILD_DOC="OFF"
fi

mkdir -p build/make_debug
mkdir -p build/make_release

cd build/make_debug
cmake -DCMAKE_BUILD_TYPE=Debug -DBUILD_DOC=$BUILD_DOC -DBUILD_TEST=OFF -DBUILD_DEMO=OFF -DBUILD_EGL=ON $* ../..
make

cd ../make_release
cmake -DCMAKE_BUILD_TYPE=Release -DBUILD_DOC=$BUILD_DOC -DBUILD_TEST=OFF -DBUILD_DEMO=OFF -DBUILD_EGL=ON $* ../..
make

echo
echo "** Ready to install."

sudo make install
cd ../make_debug
sudo make install
