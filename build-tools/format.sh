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
# then apply clang-format
find . -type f \( -name "*.hpp" -o -name "*.cpp" \) -exec dos2unix {} \;

find . -type f \( -name "*.hpp" -o -name "*.cpp" \) -print0 | xargs -0 -I{} $CLANG_FORMAT -i -style=file {}
