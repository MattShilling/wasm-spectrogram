#!/bin/bash

# Stop on errors.
set -e

# C++ build output here. Note: JS and WASM output will go to 'public/'.
# See CMakeLists.txt.
BUILD_PATH="build/"

# Set toolchain file.
EMSCRIPTEN_MODULE=/home/matts/code/emsdk/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake

# Initialize build.
cmake \
    -DCMAKE_TOOLCHAIN_FILE=${EMSCRIPTEN_MODULE} \
    -B ${BUILD_PATH} \
    -G Ninja

# Format.
clang-format-11 src/*.h src/*.cc -i

# Build.
cd ${BUILD_PATH} && ninja
