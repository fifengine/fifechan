// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#ifndef FCN_OPENLAYER_HPP
#define FCN_OPENLAYER_HPP

#include <fifechan/openlayer/openlayerinput.hpp>
#include <fifechan/openlayer/openlayerfont.hpp>
#include <fifechan/openlayer/openlayerimageloader.hpp>
#include <fifechan/openlayer/openlayergraphics.hpp>

#include "fifechan/platform.hpp"

extern "C" {
/**
 * Exists to be able to check for Guichan Open Layer
 * with autotools.
 */
FCN_EXTENSION_DECLSPEC extern void fcnOpenLayer();
}

#endif // end FCN_OPENLAYER_HPP
