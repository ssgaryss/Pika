#!/usr/bin/env bash
# Generate an Xcode project for Pika with premake5 (install via `brew install premake`).
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT_DIR="$(cd "${SCRIPT_DIR}/.." && pwd)"

cd "${ROOT_DIR}"

echo "Generating Xcode project with premake5 (xcode4)..."
premake5 xcode4

echo ""
echo "Done. Open ${ROOT_DIR}/Pika.xcworkspace in Xcode, then build the 'Pikartisan' scheme."
