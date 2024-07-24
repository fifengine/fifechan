// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#ifndef INCLUDE_FIFECHAN_BACKENDS_DIRECTX3D_DIRECTX3DIMAGELOADER_HPP_
#define INCLUDE_FIFECHAN_BACKENDS_DIRECTX3D_DIRECTX3DIMAGELOADER_HPP_

#include <string>

#include "fifechan/imageloader.hpp"
#include "fifechan/platform.hpp"
#include <d3d9.h>

namespace fcn
{
    class Image;

    /**
     * DirectX ImageLoader.
     */
    class FIFEGUI_EXT_API DirectX3DImageLoader : public ImageLoader
    {
    public:
        /**
         * Constructor.
         *
         * @param device The D3D device to use when loading images.
         */
        explicit DirectX3DImageLoader(LPDIRECT3DDEVICE9 device);

        /**
         * Sets the D3D device to use when loading images.
         *
         * @param device The D3D device to use when loading images.
         */
        virtual void setDevice(LPDIRECT3DDEVICE9 device);

        // Inherited from ImageLoader

        virtual Image* load(std::string const & filename, bool convertToDisplayFormat = true);

    protected:
        /**
         * Holds the D3D device to use when loading images.
         */
        LPDIRECT3DDEVICE9 mDevice;
    };
} // namespace fcn

#endif // INCLUDE_FIFECHAN_BACKENDS_DIRECTX3D_DIRECTX3DIMAGELOADER_HPP_
