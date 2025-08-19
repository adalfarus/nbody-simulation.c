#!/usr/bin/env bash

set -euo pipefail

# Constants
RAYLIB_VERSION=master   # or use a fixed version/tag like '5.5.0'
RAYLIB_DIR=raylib
RAYLIB_BUILD_DIR=raylib-build-static.temp

# Clean old clone if exists
rm -rf "$RAYLIB_DIR" "$RAYLIB_BUILD_DIR"

# Clone raylib
git clone --depth 1 --branch "$RAYLIB_VERSION" https://github.com/raysan5/raylib.git "$RAYLIB_DIR"
cd "$RAYLIB_DIR"

# Create build directory
mkdir build && cd build

# Build statically with large near/far clipping distances
cmake .. \
  -DCMAKE_BUILD_TYPE=Release \
  -DBUILD_SHARED_LIBS=OFF # \
  # -DCMAKE_C_FLAGS="-DDEFAULT_NEAR_CULL_DISTANCE=1000.0 -DDEFAULT_FAR_CULL_DISTANCE=1.0e+12"

make -j

# Move static build directory to root
cd ../..
mv "$RAYLIB_DIR/build/raylib" "$RAYLIB_BUILD_DIR"

# Optional cleanup
rm -rf "$RAYLIB_DIR"

