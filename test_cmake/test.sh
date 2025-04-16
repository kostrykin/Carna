#!/bin/bash

set -xe

mkdir -p build
cd build

cmake -DBUILD="$1" ..
make

ls -al
./test
