// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#ifndef FCN_OPENGL_HPP
#define FCN_OPENGL_HPP

#include <fifechan/opengl/openglgraphics.hpp>
#include <fifechan/opengl/openglimage.hpp>

#include "fifechan/platform.hpp"

extern "C" {
/**
 * Exists to be able to check for Guichan OpenGL with autotools.
 */
FCN_EXTENSION_DECLSPEC extern void fcnOpenGL();
}

#endif // end FCN_OPENGL_HPP
