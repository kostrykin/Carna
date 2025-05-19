#!/bin/bash

set -ex

export CMAKE_ARGS="-DBUILD_TEST=ON -DBUILD_DEMO=ON $CMAKE_ARGS"

bash linux_build.bash
