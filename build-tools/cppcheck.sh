#!/usr/bin/env bash

set -euo pipefail

cppcheck_version="2.22.0"

# Build cppcheck from source and install to /usr/local/bin/cppcheck
rebuild_cppcheck() (
    cd /tmp
    wget -q "https://github.com/danmar/cppcheck/archive/refs/tags/${cppcheck_version}.tar.gz"
    tar xzf "${cppcheck_version}.tar.gz"
    cd "cppcheck-${cppcheck_version}"
    mkdir -p build
    cd build
    cmake .. -DCMAKE_BUILD_TYPE=Release
    make -j"$(nproc)"
    make install
)

# Compare installed version against required version
version="$(cppcheck --version 2>/dev/null | awk '{print $2}' || true)"
is_newer_version_required=false
if [ "$(printf '%s\n' "$version" "$cppcheck_version" | sort -V | head -n1)" != "$cppcheck_version" ]; then
    is_newer_version_required=true
fi

# Rebuild it, if newer version
if [ "$is_newer_version_required" = true ]; then
    rebuild_cppcheck
fi

# Run it
echo "Cppcheck version: $(cppcheck --version)"

cppcheck \
    --enable=warning,style,performance,portability \
    --std=c++20 \
    --quiet \
    --inline-suppr \
    --error-exitcode=1 \
    -I include \
    src examples tests > cppcheck.log 2> cppcheck_error.log || true
