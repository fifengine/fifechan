#!/usr/bin/env bash

# Exit on errors, unset vars, and pipe failures
set -euo pipefail

# Remember the original dir
START_DIR="$(pwd)"

# ------------------------------------------------------------------
# Install additional components
# ------------------------------------------------------------------

apt-get update

# Package required to build libxcrypt
apt-get install -y autoconf autoconf-archive automake libtool

# Packages required to build SDL2

apt-get install -y libx11-dev libxft-dev libxext-dev

# Install jq if not already installed (required to update vcpkg baseline)
if ! command -v jq > /dev/null 2>&1; then
	apt-get install -y jq
fi

# ------------------------------------------------------------------
# SETUP VCPKG
# -------------------------------------------------------------------

cd /opt/vcpkg || exit 1

git pull

./bootstrap-vcpkg.sh

vcpkg integrate install

# Update VCPKG baseline in vcpkg.json

if [ -f "$START_DIR/vcpkg.json" ]; then
	BASELINE="$(git -C /opt/vcpkg rev-parse HEAD)"
	if ! jq -e 'has("builtin-baseline")' "$START_DIR/vcpkg.json" > /dev/null; then
		tmpfile="$(mktemp)"
		jq --arg baseline "$BASELINE" '. + {"builtin-baseline": $baseline}' "$START_DIR/vcpkg.json" > "$tmpfile"
		mv "$tmpfile" "$START_DIR/vcpkg.json"
	fi
fi

# Return to the original working dir
cd "$START_DIR" || exit 1
