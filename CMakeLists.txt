cmake_minimum_required(VERSION 3.10)

project(wasm-spectrogram)

set(CMAKE_CXX_STANDARD 14)
add_definitions(-std=c++14 -Os)

# Output in public/ so index.html can find the JS and WASM.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/public")

add_subdirectory(src)
