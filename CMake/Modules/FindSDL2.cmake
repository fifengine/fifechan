# - Try to find SDL2
# Once done this will define
#
#  SDL_FOUND - system has SDL2
#  SDL_INCLUDE_DIR - the SDL2 include directory
#  SDL_LIBRARY - Link these to use SDL2
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


if (SDL_LIBRARY AND SDL_INCLUDE_DIR)
  # in cache already
  set(SDL_FOUND TRUE)
else (SDL_LIBRARY AND SDL_INCLUDE_DIR)
  find_path(SDL_INCLUDE_DIR
    NAMES
      sdl.h
    PATHS
      /usr/include
      /usr/local/include
      /opt/local/include
      /usr/include/x86_64-linux-gnu
    PATH_SUFFIXES
      sdl2
  )

  find_library(SDL_LIBRARY
    NAMES
      SDL2
    PATHS
      /usr/lib
      /usr/local/lib
      /opt/local/lib
      /usr/lib/x86_64-linux-gnu
  )

  set(SDL_INCLUDE_DIR
    ${SDL_INCLUDE_DIR}
  )
  set(SDL_LIBRARY
    ${SDL_LIBRARY}
)

  if (SDL_INCLUDE_DIR AND SDL_LIBRARY)
     set(SDL_FOUND TRUE)
  endif (SDL_INCLUDE_DIR AND SDL_LIBRARY)

  if (SDL_FOUND)
    message(STATUS "Found SDL2: ${SDL_LIBRARY}")
  else (SDL_FOUND)
    if (SDL_FIND_REQUIRED)
      message(FATAL_ERROR "Could not find SDL2")
    endif (SDL_FIND_REQUIRED)
  endif (SDL_FOUND)

endif (SDL_LIBRARY AND SDL_INCLUDE_DIR)

