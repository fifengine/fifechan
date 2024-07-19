// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#ifndef FCN_ALLEGRO_HPP
#define FCN_ALLEGRO_HPP

#include <fifechan/allegro/allegrofont.hpp>
#include <fifechan/allegro/allegrographics.hpp>
#include <fifechan/allegro/allegroimage.hpp>
#include <fifechan/allegro/allegroimageloader.hpp>
#include <fifechan/allegro/allegroinput.hpp>

extern "C" {
/**
 * Exists to be able to check for Guichan Allegro with autotools.
 *
 * @bug Autotools is incapable of finding this function.
 */
extern void fcnAllegro();
}

#endif // end FCN_ALLEGRO_HPP
