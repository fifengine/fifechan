// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#ifndef FCN_FIFECHAN_HPP
#define FCN_FIFECHAN_HPP

#include "fifechan/platform.hpp"

class Widget;

extern "C" {
/**
 * Gets the the version of Fifechan. As it is a C function
 * it can be used to check for Fifechan with autotools.
 *
 * @return the version of Fifechan.
 */
FCN_CORE_DECLSPEC extern char const * fcnFifechanVersion();
}

#endif // end FCN_FIFECHAN_HPP
