#!/bin/bash

set -e

docker run \
  --rm \
  -v $(pwd):/src \
  -u $(id -u):$(id -g) \
  extended_emscripten

python3 -m http.server --directory public/
