#!/usr/bin/env bash

# SPDX-License-Identifier: MIT
# SPDX-FileCopyrightText: 2024 Jens A. Koch.
# This file is part of fifengine/fifechan.

# Allow ENV.CLANG_FORMAT to define the path to the binary or default to clang-format
CLANG_FORMAT=${CLANG_FORMAT:-clang-format}

# Check clang-format version
VERSION=$("$CLANG_FORMAT" --version)

# Require clang-format 17 or 18 for consistent formatting features
if [[ ! $VERSION =~ "version 17" && ! $VERSION =~ "version 18" ]]; then
    echo "Error: Unsupported clang-format version. Must be version 17 or 18."
    echo "Found version: $VERSION"
    exit 1
fi

# Scan the top-level directory and subdirectories for .h and .cpp files

# first convert line endings to Unix format
# this step is skipped in CI environments
if [[ -z "$CI" && -z "$GITHUB_ACTION" ]]; then
    find . -type f \( -name "*.hpp" -o -name "*.cpp" \) -exec dos2unix {} \;
fi

# then apply clang-format
find . -type f \( -name "*.hpp" -o -name "*.cpp" \) -print0 | xargs -0 $CLANG_FORMAT -i -style=file

# In the CI context, we run `git diff --exit-code`.
# After clang-format finishes, we check for changes with `git diff`.
# If there are changes, we exit with a non-zero status code, causing the CI job to fail.
# This ensures that code formatting is enforced.
if [[ -n "$CI" || -n "$GITHUB_ACTION" ]]; then
    if ! git diff --exit-code; then
        echo "Error: Code formatting issues detected. Please run ./build-tools/format.sh and commit the changes."
        exit 1
    fi
fi
