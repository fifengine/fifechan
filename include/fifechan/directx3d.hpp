// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#ifndef FCN_D3D_HPP
#define FCN_D3D_HPP

#include <fifechan/directx3d/directx3dgraphics.hpp>
#include <fifechan/directx3d/directx3dimage.hpp>
#include <fifechan/directx3d/directx3dimageloader.hpp>
#include <fifechan/directx3d/directx3dinput.hpp>

#include "platform.hpp"

extern "C" {
/**
 * Exists to be able to check for Guichan Directx 3D with autotools.
 */
FCN_EXTENSION_DECLSPEC extern void fcnD3D();
}

#endif // end FCN_D3D_HPP
