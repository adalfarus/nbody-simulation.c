#!/usr/bin/env bash
set -euo pipefail

# Constants
RAYGUI_DIR=raygui.temp
RAYGUI_URL="https://raw.githubusercontent.com/raysan5/raygui/master/src/raygui.h"

# Create folder if it doesn't exist
mkdir -p "$RAYGUI_DIR"

# Download raygui.h
echo "Downloading raygui.h..."
curl -L --fail -o "$RAYGUI_DIR/raygui.h" "$RAYGUI_URL"

echo "raygui.h downloaded to $RAYGUI_DIR/"
