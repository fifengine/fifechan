# - Try to find Allegro
# Once done this will define
#
#  ALLEGRO_FOUND - system has Allegro
#  ALLEGRO_INCLUDE_DIRS - the Allegro include directory
#  ALLEGRO_LIBRARIES - Link these to use Allegro
#  ALLEGRO_DEFINITIONS - Compiler switches required for using Allegro
#
#  Copyright (c) 2008 Olof Naessen <olof.naessen@gmail.com>
#
#  Redistribution and use is allowed according to the terms of the New
#  BSD license.
#  For details see the accompanying COPYING-CMAKE-SCRIPTS file.
#


if (ALLEGRO_LIBRARIES AND ALLEGRO_INCLUDE_DIRS)
  # in cache already
  set(ALLEGRO_FOUND TRUE)
else (ALLEGRO_LIBRARIES AND ALLEGRO_INCLUDE_DIRS)
  find_path(ALLEGRO_INCLUDE_DIR
    NAMES
      allegro.h
    PATHS
      /usr/include
      /usr/local/include
      /opt/local/include
      /sw/include
    PATH_SUFFIXES
      allegro
  )

  find_library(ALLEG_LIBRARY
    NAMES
      alleg
    PATHS
      /usr/lib
      /usr/local/lib
      /opt/local/lib
      /sw/lib
  )

  set(ALLEGRO_INCLUDE_DIRS
    ${ALLEGRO_INCLUDE_DIR}
  )
  set(ALLEGRO_LIBRARIES
    ${ALLEG_LIBRARY}
)

  if (ALLEGRO_INCLUDE_DIRS AND ALLEGRO_LIBRARIES)
     set(ALLEGRO_FOUND TRUE)
  endif (ALLEGRO_INCLUDE_DIRS AND ALLEGRO_LIBRARIES)

  if (ALLEGRO_FOUND)
    if (NOT Allegro_FIND_QUIETLY)
      message(STATUS "Found Allegro: ${ALLEGRO_LIBRARIES}")
    endif (NOT Allegro_FIND_QUIETLY)
  else (ALLEGRO_FOUND)
    if (Allegro_FIND_REQUIRED)
      message(FATAL_ERROR "Could not find Allegro")
    endif (Allegro_FIND_REQUIRED)
  endif (ALLEGRO_FOUND)

  # show the ALLEGRO_INCLUDE_DIRS and ALLEGRO_LIBRARIES variables only in the advanced view
  mark_as_advanced(ALLEGRO_INCLUDE_DIRS ALLEGRO_LIBRARIES)

endif (ALLEGRO_LIBRARIES AND ALLEGRO_INCLUDE_DIRS)

