#
# Copyright 2026 Jens A. Koch.
# SPDX-License-Identifier: MIT
# This file is part of fifengine/fifechan.
#

# Copy a resource folder into the destination, serialized with a lock so that
# multiple targets sharing one output directory can copy in parallel safely.
file(MAKE_DIRECTORY "${DST}")
file(LOCK "${DST}/.copy_resources.lock" GUARD PROCESS TIMEOUT 120)
file(COPY "${SRC}/" DESTINATION "${DST}")
