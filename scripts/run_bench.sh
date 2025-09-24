#!/usr/bin/env bash
set -euo pipefail

BUILD_DIR=${BUILD_DIR:-build}
CONFIG=${CONFIG:-Release}

cmake -S . -B "$BUILD_DIR" -DCMAKE_BUILD_TYPE=$CONFIG -DBUILD_BENCHMARKS=ON -DBUILD_TESTING=OFF
cmake --build "$BUILD_DIR" --config $CONFIG -j

"$BUILD_DIR"/benchmarks "$@"


