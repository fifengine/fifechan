#!/usr/bin/env bash

# Allow ENV.CLANG_FORMAT to define the path to the binary or default to clang-format
CLANG_FORMAT=${CLANG_FORMAT:-clang-format}

# Check clang-format version
VERSION=$("$CLANG_FORMAT" --version)

# Scan the top-level directory and subdirectories for .h and .cpp files
# first convert line endings to Unix format
# then apply clang-format
#find . -type f \( -name "*.hpp" -o -name "*.cpp" \) -exec dos2unix {} \;
find . -type f \( -name "*.hpp" -o -name "*.cpp" \) -print0 | xargs -0 -I{} $CLANG_FORMAT -i -style=file {}
