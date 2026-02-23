#!/usr/bin/env bash

# Exit on errors, unset vars, and pipe failures
set -euo pipefail

# Remember the original dir
START_DIR="$(pwd)"

cd /opt/vcpkg || exit 1

git pull

vcpkg integrate install

# Install additional components

apt-get update

# Package required to build libxcrypt
apt-get install -y autoconf autoconf-archive automake libtool

# Return to the original working dir
cd "$START_DIR" || exit 1
