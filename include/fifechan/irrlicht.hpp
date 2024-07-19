// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#ifndef FCN_IRRLICHT_HPP
#define FCN_IRRLICHT_HPP

#include <fifechan/irrlicht/irrlichtgraphics.hpp>
#include <fifechan/irrlicht/irrlichtimage.hpp>
#include <fifechan/irrlicht/irrlichtimageloader.hpp>
#include <fifechan/irrlicht/irrlichtinput.hpp>

#include <fifechan/platform.hpp>

extern "C" {
/**
 * Exists to be able to check for Guichan Irrlicht with autotools.
 */
FCN_EXTENSION_DECLSPEC extern void fcnIrrlicht();
}

#endif // end FCN_IRRLICHT_HPP
