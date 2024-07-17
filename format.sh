#!/usr/bin/env bash

# Allow ENV.CLANG_FORMAT to define the path to the binary or default to clang-format
CLANG_FORMAT=${CLANG_FORMAT:-clang-format}

# Check clang-format version
VERSION=$("$CLANG_FORMAT" --version)

find src -name "*.h" -o -name "*.cpp" -exec dos2unix {} \;
find src \( -name "*.h" -o -name "*.cpp" \) -print0 | xargs -0 $CLANG_FORMAT -i -style=file

# ISO-8859-1 to UTF-8 (aka the Olof Naess�n -> Olof Naessén issue)
#find . -name "*.hpp" -type f -exec bash -c 'iconv -f ISO-8859-1 -t UTF-8 -o "{}.utf8" "{}" && mv "{}.utf8" "{}"' \;
#find . -name "*.hpp" -type f -exec bash -c 'iconv -f ISO-8859-1 -t UTF-8 -o "{}.utf8" "{}" && mv "{}.utf8" "{}"' \;