#!/usr/bin/env bash

set -e

# ------------------------------------------------------------------
# Setup Paths
# ------------------------------------------------------------------
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(dirname "$SCRIPT_DIR")"
cd "$PROJECT_ROOT"

# ------------------------------------------------------------------
# Extract Version from vcpkg.json using jq
# ------------------------------------------------------------------

if ! command -v jq &> /dev/null; then
    echo "Error: jq is required." >&2; exit 1
fi

VERSION=$(jq -r '.version' vcpkg.json)
if [[ -z "$VERSION" || "$VERSION" == "null" ]]; then
    echo "Error: Could not read version from vcpkg.json" >&2; exit 1
fi

echo "Updating files with version: $VERSION"

# ------------------------------------------------------------------
# Insert Version Number
# ------------------------------------------------------------------

# 3. Update doxygen.conf
# Match the string PROJECT_NUMBER ... = (version) and replace the version
sed -i "s/^\(PROJECT_NUMBER[[:space:]]*=[[:space:]]*\).*/\1${VERSION}/" doxygen.conf
echo "Updated doxygen.conf"

# ------------------------------------------------------------------
# ADD MORE FILE UPDATES BELOW THIS LINE
#
# Remember that the following files should not be updated manually:
#  - version.hpp: is generated during build.
#  - src/fifechan.(cpp|hpp) retrieves the version from version.hpp.
#
# ------------------------------------------------------------------

echo "Done."
