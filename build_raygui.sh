#!/usr/bin/env bash
set -euo pipefail

# Constants
RAYGUI_VERSION="4.0"
RAYGUI_REPO="https://github.com/raysan5/raygui.git"
SRC_DIR="raygui"

# Clean up
rm -f "raygui.h"
rm -rf "$SRC_DIR"

# Clone specific tag
git clone --branch "$RAYGUI_VERSION" --depth 1 "$RAYGUI_REPO" "$SRC_DIR"

# Copy the header
cp "$SRC_DIR"/src/raygui.h ./

# Clean up source
rm -rf "$SRC_DIR"

echo "raygui v$RAYGUI_VERSION download complete!"

