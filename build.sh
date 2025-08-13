#!/usr/bin/env bash
set -euo pipefail

# Paths
RAYLIB_INCLUDE="./raylib-build-static.temp/include"
RAYLIB_LIB="./raylib-build-static.temp"
RAYGUI_INCLUDE="./raylib-build-static.temp/include"
RAYGUI_LIB="./raylib-build-static.temp"

clang -Wall -Wextra -Werror \
  main.c nbody.c nbody_config.c solar_system_conf.c \
  -I"$RAYLIB_INCLUDE" \
  "$RAYLIB_LIB/libraylib.a" \
  -lm -ldl -lpthread -lX11 \
  -o nbody.elf

./nbody.elf

