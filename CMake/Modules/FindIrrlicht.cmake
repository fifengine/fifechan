# - Try to find Irrlicht
# Once done this will define
#
#  IRRLICHT_FOUND - system has Irrlicht
#  IRRLICHT_INCLUDE_DIRS - the Irrlicht include directory
#  IRRLICHT_LIBRARIES - Link these to use Irrlicht
#  IRRLICHT_DEFINITIONS - Compiler switches required for using Irrlicht
#
#  Copyright (c) 2008 Olof Naessen <olof.naessen@gmail.com>
#
#  Redistribution and use is allowed according to the terms of the New
#  BSD license.
#  For details see the accompanying COPYING-CMAKE-SCRIPTS file.
#


if (IRRLICHT_LIBRARIES AND IRRLICHT_INCLUDE_DIRS)
  # in cache already
  set(IRRLICHT_FOUND TRUE)
else (IRRLICHT_LIBRARIES AND IRRLICHT_INCLUDE_DIRS)
  find_path(IRRLICHT_INCLUDE_DIR
    NAMES
      irrlicht.h
    PATHS
      /usr/include
      /usr/local/include
      /opt/local/include
    PATH_SUFFIXES
      irrlicht
  )

  find_library(IRRLICHT_LIBRARY
    NAMES
      Irrlicht
    PATHS
      /usr/lib
      /usr/local/lib
      /opt/local/lib
  )

  set(IRRLICHT_INCLUDE_DIRS
    ${IRRLICHT_INCLUDE_DIR}
  )
  set(IRRLICHT_LIBRARIES
    ${IRRLICHT_LIBRARY}
)

  if (IRRLICHT_INCLUDE_DIRS AND IRRLICHT_LIBRARIES)
     set(IRRLICHT_FOUND TRUE)
  endif (IRRLICHT_INCLUDE_DIRS AND IRRLICHT_LIBRARIES)

  if (IRRLICHT_FOUND)
    if (NOT Irrlicht_FIND_QUIETLY)
      message(STATUS "Found Irrlicht: ${IRRLICHT_LIBRARIES}")
    endif (NOT Irrlicht_FIND_QUIETLY)
  else (IRRLICHT_FOUND)
    if (Irrlicht_FIND_REQUIRED)
      message(FATAL_ERROR "Could not find Irrlicht")
    endif (Irrlicht_FIND_REQUIRED)
  endif (IRRLICHT_FOUND)

  # show the IRRLICHT_INCLUDE_DIRS and IRRLICHT_LIBRARIES variables only in the advanced view
  mark_as_advanced(IRRLICHT_INCLUDE_DIRS IRRLICHT_LIBRARIES)

endif (IRRLICHT_LIBRARIES AND IRRLICHT_INCLUDE_DIRS)

