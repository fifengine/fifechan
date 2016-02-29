# - Try to find OGLFT
# Once done this will define
#
#  OGLFT_FOUND - system has OGLFT
#  OGLFT_INCLUDE_DIRS - the OGLFT include directory
#  OGLFT_LIBRARIES - Link these to use OGLFT
#  OGLFT_DEFINITIONS - Compiler switches required for using OGLFT
#
#  Copyright (c) 2008 Olof Naessen <olof.naessen@gmail.com>
#
#  Redistribution and use is allowed according to the terms of the New
#  BSD license.
#  For details see the accompanying COPYING-CMAKE-SCRIPTS file.
#


if (OGLFT_LIBRARIES AND OGLFT_INCLUDE_DIRS)
  # in cache already
  set(OGLFT_FOUND TRUE)
else (OGLFT_LIBRARIES AND OGLFT_INCLUDE_DIRS)
  find_path(OGLFT_INCLUDE_DIRS
    NAMES
      OpenGL-FT.h
      OGLFT.h
    PATHS
      /usr/include
      /usr/local/include
      /opt/local/include
    PATH_SUFFIXES
      oglft
      OGLFT
  )

  find_library(OGLFT_LIBRARIES
    NAMES
      OpenGL-FT
      OGLFT
      oglft
    PATHS
      /usr/lib
      /usr/local/lib
      /opt/local/lib
  )

  add_definitions(-DOGLFT_NO_QT)

  include(${CMAKE_ROOT}/Modules/FindPackageHandleStandardArgs.cmake)

  FIND_PACKAGE_HANDLE_STANDARD_ARGS(OGLFT
                                  REQUIRED_VARS OGLFT_LIBRARIES OGLFT_INCLUDE_DIRS)
endif (OGLFT_LIBRARIES AND OGLFT_INCLUDE_DIRS)

