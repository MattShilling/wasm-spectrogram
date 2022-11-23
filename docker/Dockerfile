FROM emscripten/emsdk
RUN apt update && apt install -y ninja-build cmake clang-format-11
CMD BUILD_WITH_DOCKER=1 ./build.sh
