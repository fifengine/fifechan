#!/usr/bin/env bash

# SPDX-License-Identifier: MIT
# SPDX-FileCopyrightText: 2024 Jens A. Koch.
# This file is part of fifengine/fifechan.

#
# cpplint is a python tool that checks C++ code for style issues.
# It is a part of the Google C++ Style Guide.
# It is recommended to install cpplint in a virtual environment.
# That makes the following requirements: Python 3, pip, and venv.
#
# The script will check if Python 3, pip, and venv are installed.
# If not, it will install them.
# It will then create a virtual environment and install cpplint.
# Finally, it will run cpplint on the source code.
#

# Check if Python 3, pip, and venv are already installed, if not install them
if ! command -v python3 &> /dev/null || ! command -v pip &> /dev/null || ! python3 -c 'import venv' &> /dev/null; then
    apt-get -y install python3 python3-pip python3-venv
fi

# Create a virtual environment
export VIRTUAL_ENV=/opt/venv
python3 -m venv $VIRTUAL_ENV
export PATH="$VIRTUAL_ENV/bin:$PATH"

# Check if cpplint is already installed, if not install it
if ! command -v cpplint &> /dev/null; then
    pip install --prefer-binary --no-cache-dir cpplint
fi

# Run cpplint on the source code
cpplint --recursive src include tests examples --config=../CPPLINT.cfg
