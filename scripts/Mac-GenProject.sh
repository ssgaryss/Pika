#!/usr/bin/env bash
# Generate macOS project files with premake5 (install it via `brew install premake`).
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT_DIR="$(cd "${SCRIPT_DIR}/.." && pwd)"

cd "${ROOT_DIR}"

echo "Generating Makefiles with premake5 (gmake2)..."
premake5 gmake2

echo ""
echo "Done. Build with one of:"
echo "  make config=debug      # Debug"
echo "  make config=release    # Release"
echo "  make config=dist       # Dist"
