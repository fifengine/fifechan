// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors
#ifndef FCN_CAIRO_HPP
#define FCN_CAIRO_HPP

#include <fifechan/cairo/cairofont.hpp>
#include <fifechan/cairo/cairographics.hpp>
#include <fifechan/cairo/cairoimage.hpp>
#include <fifechan/cairo/cairoimageloader.hpp>

extern "C" {
/**
 * Exists to be able to check for Guichan Cairo with autotools.
 *
 * @bug Autotools is incapable of finding this function.
 */
extern void fcnCairo();
}

#endif // end FCN_CAIRO_HPP
