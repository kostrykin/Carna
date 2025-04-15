#!/bin/bash

set -ex

export CMAKE_ARGS="-DBUILD_TEST=OFF -DBUILD_DEMO=OFF -DBUILD_EGL=ON"

bash linux_build.bash