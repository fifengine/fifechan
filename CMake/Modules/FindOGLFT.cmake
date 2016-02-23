# - Try to find OpenGL-FT
# Once done this will define
#
#  OpenGL-FT_FOUND - system has OpenGL-FT
#  OpenGL-FT_INCLUDE_DIRS - the OpenGL-FT include directory
#  OpenGL-FT_LIBRARIES - Link these to use OpenGL-FT
#  OpenGL-FT_DEFINITIONS - Compiler switches required for using OpenGL-FT
#
#  Copyright (c) 2008 Olof Naessen <olof.naessen@gmail.com>
#
#  Redistribution and use is allowed according to the terms of the New
#  BSD license.
#  For details see the accompanying COPYING-CMAKE-SCRIPTS file.
#


if (OpenGL-FT_LIBRARIES AND OpenGL-FT_INCLUDE_DIRS)
  # in cache already
  set(OpenGL-FT_FOUND TRUE)
else (OpenGL-FT_LIBRARIES AND OpenGL-FT_INCLUDE_DIRS)
  find_path(OpenGL-FT_INCLUDE_DIR
    NAMES
      OpenGL-FT.h
      OGLFT.h
    PATHS
      /usr/include
      /usr/local/include
      /opt/local/include
    PATH_SUFFIXES
      oglft
  )

  find_library(OpenGL-FT_LIBRARY
    NAMES
      OpenGL-FT
      OGLFT
    PATHS
      /usr/lib
      /usr/local/lib
      /opt/local/lib
  )

  add_definitions( -DOGLFT_NO_QT)

  include(${CMAKE_ROOT}/Modules/FindPackageHandleStandardArgs.cmake)

  FIND_PACKAGE_HANDLE_STANDARD_ARGS(OpenGL-FT
                                  REQUIRED_VARS OpenGL-FT_LIBRARY OpenGL-FT_INCLUDE_DIR)


endif (OpenGL-FT_LIBRARIES AND OpenGL-FT_INCLUDE_DIRS)

