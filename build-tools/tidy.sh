#!/usr/bin/env bash

# -----------------------------------------------------------------------------
# clang-tidy helper
#
# Purpose
# - Run clang-tidy with the project's compile database and repo file layout.
# - Support full-repo checks or a focused single-file check.
#
# Requirements
# - A configured build directory at ./build containing compile_commands.json.
# - clang-tidy version 17 or newer available on PATH (or via CLANG_TIDY env).
#
# Usage
# - ./build-tools/tidy.sh
#     Runs clang-tidy on tracked C++ files under src/ and tests/core_tests/.
# - ./build-tools/tidy.sh path/to/file.cpp
#     Runs clang-tidy only on the provided file.
#
# Notes
# - This script discovers a compatible clang-tidy binary in this order:
#   CLANG_TIDY env var, then clang-tidy-20/19/18/17 fallbacks.
# - Exit code is propagated from clang-tidy.
# -----------------------------------------------------------------------------

set -euo pipefail

if [[ $# -gt 1 ]]; then
  echo "Usage: $0 [FILE]"
  exit 1
fi

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="${SCRIPT_DIR}/.."
# Allow overriding build directory via env var for CI or local presets.
BUILD_DIR="${BUILD_DIR:-${REPO_ROOT}/build}"
COMPILE_COMMANDS="${BUILD_DIR}/compile_commands.json"

# Ensure compile database exists.
if [[ ! -f "$COMPILE_COMMANDS" ]]; then
  echo "compile_commands.json not found in ${BUILD_DIR}; attempting to run CMake configure..."
  if ! command -v cmake >/dev/null 2>&1; then
    echo "cmake not found; cannot generate compile_commands.json. Run CMake configure first."
    exit 1
  fi

  # Try a minimal configure that exports the compile commands.
  if ! cmake -S "${REPO_ROOT}" -B "${BUILD_DIR}" -DCMAKE_EXPORT_COMPILE_COMMANDS=ON; then
    echo "CMake configure failed; please configure your build directory manually."
    exit 1
  fi

  # Ensure compile commands were generated.
  if [[ ! -f "$COMPILE_COMMANDS" ]]; then
    echo "compile_commands.json still missing after CMake configure."
    exit 1
  fi
fi

# Check binary exists and is version >=17.
check_tidy_version() {
  local binary="$1"
  if command -v "$binary" >/dev/null 2>&1; then
    local version
    local major
    version=$("$binary" --version)
    major=$(echo "$version" | sed -nE 's/.*version ([0-9]+).*/\1/p')
    if [[ -n "$major" && "$major" -ge 17 ]]; then
      echo "$binary"
      return 0
    fi
  fi
  return 1
}

CLANG_TIDY=${CLANG_TIDY:-clang-tidy}

# Pick first compatible clang-tidy.
if ! CLANG_TIDY=$(check_tidy_version "$CLANG_TIDY") \
  && ! CLANG_TIDY=$(check_tidy_version "clang-tidy-20") \
  && ! CLANG_TIDY=$(check_tidy_version "clang-tidy-19") \
  && ! CLANG_TIDY=$(check_tidy_version "clang-tidy-18") \
  && ! CLANG_TIDY=$(check_tidy_version "clang-tidy-17"); then
  echo "clang-tidy not found. Install clang-tidy version >=17."
  exit 1
fi

echo "Using $($CLANG_TIDY --version | head -n 1)"

cd "$REPO_ROOT"

# Single file mode or default repo scan. Default includes `src/`, `include/` and `tests/`.
if [[ $# -eq 1 ]]; then
  if [[ ! -f "$1" ]]; then
    echo "File not found: $1"
    exit 1
  fi
  files=("$1")
else
  mapfile -t files < <(git ls-files | grep -E '^(src/|include/|tests/).*(\.(cpp|cxx|cc|c|h|hpp))$' || true)

  if [[ ${#files[@]} -eq 0 ]]; then
    echo "No C/C++ source files found to analyze."
    exit 0
  fi
fi

"$CLANG_TIDY" -p "$BUILD_DIR" "${files[@]}"
