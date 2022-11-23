#!/bin/bash

# Stop on errors.
set -e

# C++ build output here. Note: JS and WASM output will go to 'public/'.
# See CMakeLists.txt.
BUILD_PATH="build/"

BUILD_WITH_DOCKER=${BUILD_WITH_DOCKER:-0}

# Set toolchain file.
EMSCRIPTEN_TOOLCHAIN_DOCKER=/emsdk/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake
EMSCRIPTEN_TOOLCHAIN_LOCAL=/home/matts/code/emsdk/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake
EMSCRIPTEN_TOOLCHAIN=${EMSCRIPTEN_TOOLCHAIN_LOCAL}

if [ "${BUILD_WITH_DOCKER}" -eq "1" ]; then
  EMSCRIPTEN_TOOLCHAIN=${EMSCRIPTEN_TOOLCHAIN_DOCKER}
fi

# Format.
clang-format-11 src/**/*.h src/**/*.cc -i

# Initialize build.
cmake \
    -DCMAKE_TOOLCHAIN_FILE=${EMSCRIPTEN_TOOLCHAIN} \
    -B ${BUILD_PATH} \
    -G Ninja

# Build.
cd ${BUILD_PATH} && cmake --build .

cd ../

# Move things around.
cp src/js/app.js public/app.js
mkdir -p public/worklets
cp src/js/synth_worklet.js public/worklets/synth_worklet.js
