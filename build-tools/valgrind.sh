#!/usr/bin/env bash

valgrind \
  --leak-check=full \
  --show-reachable=yes \
  --track-origins=yes \
  /workspaces/fifechan_new/out/build/clang22-x64-linux-dbg-cov/tests/unit/all_tests
