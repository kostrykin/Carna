#!/bin/bash

set -xe

mkdir -p build
cd build

cmake ..
make

ls -al
./test
