#!/usr/bin/env bash
set -euo pipefail

# Paths
RAYLIB_INCLUDE="./raylib-build-static.temp/include"
RAYLIB_LIB="./raylib-build-static.temp"
RAYGUI_INCLUDE="./"

clang -Wall -Wextra \
  raygui_example.c \
  -I"$RAYLIB_INCLUDE" \
  -I"$RAYGUI_INCLUDE" \
  "$RAYLIB_LIB/libraylib.a" \
  -lm -ldl -lpthread -lX11 \
  -o nbody.elf

./nbody.elf

