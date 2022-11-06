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
clang-format-11 src/**/*.h src/**/*.cc -i

emcc src/math/math_tables.cc \
  src/sound/audio_frame.cc \
  src/sound/dual_fm_osc.cc \
  -Isrc/ \
  -std=c++14 \
  -O1 \
  -s WASM=1 \
  -s ALLOW_MEMORY_GROWTH=1 \
  -s WASM_ASYNC_COMPILATION=0 \
  -s SINGLE_FILE=1 \
  -s MODULARIZE=1 \
  --bind \
  -o public/synth.js

cp src/js/app.js public/app.js
mkdir -p public/worklets
cat public/synth.js src/js/synth_worklet.js > public/worklets/synth_worklet.js

# Build.
cd ${BUILD_PATH} && ninja

