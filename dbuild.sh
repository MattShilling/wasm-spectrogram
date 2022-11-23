#!/bin/bash

docker run \
  --rm \
  -v $(pwd):/src \
  -u $(id -u):$(id -g) \
  extended_emscripten
