# - Try to find SDL2 image
# Once done this will define
#
#  SDLIMAGE_FOUND - system has SDL2 image
#  SDLIMAGE_INCLUDE_DIR - the SDL2 image include directory
#  SDLIMAGE_LIBRARY - Link these to use SDL2 image
#
#/***************************************************************************
# *   Copyright (C) 2014 by the fifechan team                               *
# *   http://fifechan.github.com/fifechan                                   *
# *   This file is part of fifechan.                                        *
# *                                                                         *
# *   fifechan is free software; you can redistribute it and/or             *
# *   modify it under the terms of the GNU Lesser General Public            *
# *   License as published by the Free Software Foundation; either          *
# *   version 2.1 of the License, or (at your option) any later version.    *
# *                                                                         *
# *   This library is distributed in the hope that it will be useful,       *
# *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
# *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU     *
# *   Lesser General Public License for more details.                       *
# *                                                                         *
# *   You should have received a copy of the GNU Lesser General Public      *
# *   License along with this library; if not, write to the                 *
# *   Free Software Foundation, Inc.,                                       *
# *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA          *
# ***************************************************************************/
#


if (SDLIMAGE_LIBRARY AND SDLIMAGE_INCLUDE_DIR)
  # in cache already
  set(SDLIMAGE_FOUND TRUE)
else (SDLIMAGE_LIBRARY AND SDLIMAGE_INCLUDE_DIR)
  find_path(SDLIMAGE_INCLUDE_DIR
    NAMES
      SDL_image.h
    PATHS
      /usr/include
      /usr/local/include
      /opt/local/include
      /sw/include
    PATH_SUFFIXES
      sdl_image2
  )

  find_library(SDLIMAGE_LIBRARY
    NAMES
      SDL2_image
    PATHS
      /usr/lib
      /usr/local/lib
      /opt/local/lib
  )

  set(SDLIMAGE_INCLUDE_DIR
    ${SDLIMAGE_INCLUDE_DIR}
  )
  set(SDLIMAGE_LIBRARY
    ${SDLIMAGE_LIBRARY}
)

  if (SDLIMAGE_INCLUDE_DIR AND SDLIMAGE_LIBRARY)
     set(SDLIMAGE_FOUND TRUE)
  endif (SDLIMAGE_INCLUDE_DIR AND SDLIMAGE_LIBRARY)

  if (SDLIMAGE_FOUND)
    if (NOT SDLIMAGE_FIND_QUIETLY)
      message(STATUS "Found SDL2 image: ${SDLIMAGE_LIBRARY}")
    endif (NOT SDLIMAGE_FIND_QUIETLY)
  else (SDLIMAGE_FOUND)
    if (SDLIMAGE_FIND_REQUIRED)
      message(FATAL_ERROR "Could not find SDL2 image")
    endif (SDLIMAGE_FIND_REQUIRED)
  endif (SDLIMAGE_FOUND)

  # show the SDLIMAGE_INCLUDE_DIRS and SDLIMAGE_LIBRARIES variables only in the advanced view
  mark_as_advanced(SDLIMAGE_LIBRARY AND SDLIMAGE_INCLUDE_DIR)

endif (SDLIMAGE_LIBRARY AND SDLIMAGE_INCLUDE_DIR)

