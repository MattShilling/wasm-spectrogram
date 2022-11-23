#!/bin/bash
set -e -x
docker pull emscripten/emsdk
cd docker && docker build -t extended_emscripten .
