#!/bin/bash

set -xe

mkdir -p build
cd build

cmake -DBUILD="$1" -DCMAKE_MODULE_PATH="$CMAKE_MODULE_PATH" ..
make

ls -al
./test
