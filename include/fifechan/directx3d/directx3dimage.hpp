// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#ifndef INCLUDE_FIFECHAN_DIRECTX3D_DIRECTX3DIMAGE_HPP_
#define INCLUDE_FIFECHAN_DIRECTX3D_DIRECTX3DIMAGE_HPP_

#include <string>

#include "fifechan/color.hpp"
#include "fifechan/image.hpp"
#include "fifechan/platform.hpp"
#include <d3d9.h>
#include <d3dx9.h>

namespace fcn
{
    /**
     * DirectX implementation of Image.
     */
    class FIFEGUI_EXT_API DirectX3DImage : public Image
    {
    public:
        /**
         * Constructor. Load an image from a DirectX surface.
         *
         * @param surface The surface from which to load.
         * @param device The D3D device to use when converting the image to display format.
         * @param autoFree True if the surface should automatically be deleted.
         */
        DirectX3DImage(LPDIRECT3DSURFACE9 surface, LPDIRECT3DDEVICE9 device, int width, int height, bool autoFree);

        virtual ~DirectX3DImage();

        /**
         * Gets the D3D texture for the image.
         *
         * @return The D3D texture handle for the image.
         */

        virtual LPDIRECT3DTEXTURE9 getTexture() const;

        /**
         * Gets the D3D texture width of the image.
         *
         * @return The D3D texture width of the image.
         */
        virtual int getTextureWidth() const;

        /**
         * Gets the D3D texture height of the image.
         *
         * @return The D3D texture height of the image.
         */
        virtual int getTextureHeight() const;

        // Inherited from Image

        virtual void free();

        virtual int getWidth() const;

        virtual int getHeight() const;

        virtual Color getPixel(int x, int y);

        virtual void putPixel(int x, int y, Color const & color);

        virtual void convertToDisplayFormat();

    protected:
        /**
         * Holds the surface image. This surface is NULL after the image
         * has been converted to display format.
         */
        LPDIRECT3DSURFACE9 mSurface;

        /**
         * Holds the texture of the image. This texture isn't initialized until
         * the image is converted to display format.
         */
        LPDIRECT3DTEXTURE9 mTexture;

        /**
         * Holds the D3D device used when converting the image to display format.
         */
        LPDIRECT3DDEVICE9 mDevice;

        /**
         * True if the image should be auto freed.
         */
        bool mAutoFree;

        /**
         * Holds the width of the image.
         */
        int mWidth;

        /**
         * Holds the height of the image.
         */
        int mHeight;

        /**
         * Holds the texture width of the image.
         */
        int mTextureWidth;

        /**
         * Holds the texture height of the image.
         */
        int mTextureHeight;
    };
} // namespace fcn

#endif // INCLUDE_FIFECHAN_DIRECTX3D_DIRECTX3DIMAGE_HPP_
