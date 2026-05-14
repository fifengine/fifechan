#!/usr/bin/env bash

set -euo pipefail

echo "Cppcheck version: $(cppcheck --version)"

cppcheck \
    --enable=warning,style,performance,portability \
    --std=c++20 \
    --quiet \
    --inline-suppr \
    --error-exitcode=1 \
    -I include \
    src examples tests > cppcheck.log 2> cppcheck_error.log || true
