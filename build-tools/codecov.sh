#!/usr/bin/env bash
#
# build-tools/codecov.sh
#
# SPDX-License-Identifier: MIT
# SPDX-FileCopyrightText: 2026 Jens A. Koch.
# This file is part of fifengine/fifechan.
#
# Local coverage script for fifechan.
# Builds with clang22-x64-linux-dbg-cov preset, runs tests,
# and generates coverage reports for a specific source file or directory.
#
# Usage:
#   ./build-tools/codecov.sh                    # Full project coverage
#   ./build-tools/codecov.sh src/widget.cpp    # Coverage for specific file
#   ./build-tools/codecov.sh src/widgets/     # Coverage for directory
#

set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"
PRESET="clang22-x64-linux-dbg-cov"
BUILD_DIR="${PROJECT_ROOT}/out/build/${PRESET}"
COVERAGE_DIR="${BUILD_DIR}/coverage"
COV_EXCLUDE_REGEX='(vcpkg_installed|build|tests)'

# Parse arguments
TARGET_FILE=""
if [ $# -gt 0 ]; then
    TARGET_FILE="$1"
    # Convert to absolute path if relative
    if [[ ! "$TARGET_FILE" = /* ]]; then
        TARGET_FILE="${PROJECT_ROOT}/${TARGET_FILE}"
    fi
fi

echo "=========================================="
echo "fifechan code coverage script"
echo "=========================================="
echo "Project root: ${PROJECT_ROOT}"
echo "Preset: ${PRESET}"
echo "Build dir: ${BUILD_DIR}"
if [ -n "$TARGET_FILE" ]; then
    echo "Target: ${TARGET_FILE}"
fi
echo ""

# Step 1: Configure if needed
if [ ! -d "${BUILD_DIR}" ]; then
    echo "→ Configuring build with preset: ${PRESET}"
    cmake --preset "${PRESET}"
else
    echo "→ Build directory already exists: ${BUILD_DIR}"
fi

# Step 2: Build
echo "→ Building with preset: ${PRESET}"
cmake --build "${BUILD_DIR}"

# Step 3: Prepare coverage environment
echo "→ Preparing coverage environment"
mkdir -p "${COVERAGE_DIR}"
export LLVM_PROFILE_FILE="${COVERAGE_DIR}/%p.profraw"

# Clean up any old coverage data
rm -f "${COVERAGE_DIR}"/*.profraw
rm -f "${COVERAGE_DIR}"/*.profdata
rm -f "${COVERAGE_DIR}"/*.lcov

# Step 4: Run tests with coverage instrumentation
echo "→ Running tests with coverage instrumentation"
# Use xvfb for headless SDL tests
if command -v xvfb-run &> /dev/null; then
    xvfb-run -a ctest --test-dir "${BUILD_DIR}" --output-on-failure || true
else
    echo "Warning: xvfb-run not found, tests requiring video may fail"
    ctest --test-dir "${BUILD_DIR}" --output-on-failure || true
fi

# Step 5: Merge coverage data
echo "→ Merging coverage data"
shopt -s nullglob
profraw_files=("${COVERAGE_DIR}"/*.profraw)
if [ ${#profraw_files[@]} -eq 0 ]; then
    echo "Error: No .profraw files found. Did tests run with LLVM_PROFILE_FILE set?"
    exit 1
fi
# shellcheck disable=SC2068
llvm-profdata merge -sparse ${profraw_files[@]} -o "${COVERAGE_DIR}/coverage.profdata"

# Step 6: Find instrumented binaries
echo "→ Finding instrumented test binaries"
mapfile -t BINARIES < <(find "${BUILD_DIR}" -maxdepth 3 -type f -executable \( -name "*_test" -o -name "*_tests" \) -print || true)
if [ ${#BINARIES[@]} -eq 0 ]; then
    echo "Error: No instrumented binaries found."
    exit 1
fi
echo "Found ${#BINARIES[@]} instrumented binaries"

# Step 7: Generate coverage report
echo "→ Generating coverage report"

if [ -n "$TARGET_FILE" ]; then
    # For specific file/directory, show detailed coverage
    echo ""
    echo "Coverage report for: ${TARGET_FILE}"
    echo "=========================================="
    
    # Use llvm-cov show for specific file with source highlighting
    llvm-cov show -instr-profile="${COVERAGE_DIR}/coverage.profdata" \
        "${BINARIES[@]}" \
        --ignore-filename-regex="${COV_EXCLUDE_REGEX}" \
        --format=text \
        --output-dir="${COVERAGE_DIR}/report" \
        --sources="$TARGET_FILE" \
        2>&1 | head -200
    
    # Also show summary for the target
    echo ""
    echo "Summary for: ${TARGET_FILE}"
    echo "=========================================="
    llvm-cov report -instr-profile="${COVERAGE_DIR}/coverage.profdata" \
        "${BINARIES[@]}" \
        --ignore-filename-regex="${COV_EXCLUDE_REGEX}" \
        --sources="$TARGET_FILE"
    
    # Export JSON for the target
    echo ""
    echo "JSON export for: ${TARGET_FILE}"
    echo "=========================================="
    llvm-cov export -format=text -instr-profile="${COVERAGE_DIR}/coverage.profdata" \
        "${BINARIES[@]}" \
        --ignore-filename-regex="${COV_EXCLUDE_REGEX}" \
        --sources="$TARGET_FILE" \
        > "${COVERAGE_DIR}/coverage.json" 2>&1
    echo "JSON exported to: ${COVERAGE_DIR}/coverage.json"
else
    # Full project report
    
    # Export LCOV report
    echo "→ Exporting LCOV report"
    llvm-cov export -format=lcov -instr-profile="${COVERAGE_DIR}/coverage.profdata" \
        "${BINARIES[@]}" --ignore-filename-regex="${COV_EXCLUDE_REGEX}" \
        > "${COVERAGE_DIR}/coverage.lcov"
    
    # Export JSON report
    echo "→ Exporting JSON report"
    llvm-cov export -format=text -instr-profile="${COVERAGE_DIR}/coverage.profdata" \
        "${BINARIES[@]}" --ignore-filename-regex="${COV_EXCLUDE_REGEX}" \
        > "${COVERAGE_DIR}/coverage.json"
    
    # Print summary
    echo ""
    echo "Coverage Summary"
    echo "=========================================="
    llvm-cov report -instr-profile="${COVERAGE_DIR}/coverage.profdata" \
        "${BINARIES[@]}" \
        --ignore-filename-regex="${COV_EXCLUDE_REGEX}"
    
    # Generate HTML report
    echo ""
    echo "→ Generating HTML report"
    llvm-cov show -instr-profile="${COVERAGE_DIR}/coverage.profdata" \
        "${BINARIES[@]}" \
        --ignore-filename-regex="${COV_EXCLUDE_REGEX}" \
        --format=html \
        --output-dir="${COVERAGE_DIR}/html"
    
    echo ""
    echo "=========================================="
    echo "Coverage reports generated:"
    echo "  - LCOV: ${COVERAGE_DIR}/coverage.lcov"
    echo "  - JSON: ${COVERAGE_DIR}/coverage.json"
    echo "  - HTML: ${COVERAGE_DIR}/html/index.html"
    echo "=========================================="
fi

echo ""
echo "Done!"
