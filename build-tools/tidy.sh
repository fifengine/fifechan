#!/usr/bin/env bash

# This runs clang-tidy on all source files.
# Please run cmake to generate compile_commands.json before running this script.
clang-tidy -p ../out/compile_commands.json ../include/*.hpp ../src/*.cpp
