// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#include "fifechan/directx3d/directx3dimage.hpp"

#include "fifechan/exception.hpp"

namespace fcn
{
    DirectX3DImage::DirectX3DImage(
        LPDIRECT3DSURFACE9 surface, LPDIRECT3DDEVICE9 device, int width, int height, bool convertToDisplayFormat)
    {
        mTexture       = NULL;
        mSurface       = surface;
        mDevice        = device;
        mWidth         = width;
        mHeight        = height;
        mTextureWidth  = 1;
        mTextureHeight = 1;

        while (mTextureWidth < mWidth) {
            mTextureWidth *= 2;
        }

        while (mTextureHeight < mHeight) {
            mTextureHeight *= 2;
        }

        if (convertToDisplayFormat) {
            DirectX3DImage::convertToDisplayFormat();
        }
    }

    DirectX3DImage::~DirectX3DImage()
    {
        if (mAutoFree) {
            free();
        }
    }

    LPDIRECT3DTEXTURE9 DirectX3DImage::getTexture() const
    {
        return mTexture;
    }

    void DirectX3DImage::free()
    {
        if (mSurface != NULL)
            mSurface->Release();

        if (mTexture != NULL)
            mTexture->Release();
    }

    int DirectX3DImage::getWidth() const
    {
        return mWidth;
    }

    int DirectX3DImage::getHeight() const
    {
        return mHeight;
    }

    Color DirectX3DImage::getPixel(int x, int y)
    {
        if (mSurface == NULL) {
            fcn::throwException(
                ("Image has been converted to display format"),
                static_cast<char const *>(__FUNCTION__),
                __FILE__,
                __LINE__);
        }

        if (x < 0 || x >= mWidth || y < 0 || y >= mHeight) {
            fcn::throwException(
                ("Coordinates outside of the image"), static_cast<char const *>(__FUNCTION__), __FILE__, __LINE__);
        }

        D3DLOCKED_RECT lockedRect;
        mSurface->LockRect(&lockedRect, NULL, D3DLOCK_READONLY);
        DWORD* pixels   = (DWORD*)lockedRect.pBits;
        D3DXCOLOR color = pixels[lockedRect.Pitch / sizeof(DWORD) * y + x];
        mSurface->UnlockRect();

        return Color(color.r * 255, color.g * 255, color.b * 255, color.a * 255);
    }

    void DirectX3DImage::putPixel(int x, int y, Color const & color)
    {
        if (mSurface == NULL) {
            fcn::throwException(
                ("Image has been converted to display format"),
                static_cast<char const *>(__FUNCTION__),
                __FILE__,
                __LINE__);
        }

        if (x < 0 || x >= mWidth || y < 0 || y >= mHeight) {
            fcn::throwException(
                ("Coordinates outside of the image"), static_cast<char const *>(__FUNCTION__), __FILE__, __LINE__);
        }

        D3DLOCKED_RECT lockedRect;
        mSurface->LockRect(&lockedRect, NULL, 0);
        DWORD* pixels                                    = (DWORD*)lockedRect.pBits;
        pixels[lockedRect.Pitch / sizeof(DWORD) * y + x] = D3DCOLOR_RGBA(color.r, color.g, color.b, color.a);
        mSurface->UnlockRect();
    }

    void DirectX3DImage::convertToDisplayFormat()
    {
        HRESULT result;
        result = D3DXCreateTexture(
            mDevice, mTextureWidth, mTextureHeight, D3DX_DEFAULT, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &mTexture);
        if (result != D3D_OK) {
            fcn::throwException(
                ("Unable to convert image to display format!"),
                static_cast<char const *>(__FUNCTION__),
                __FILE__,
                __LINE__);
        }

        LPDIRECT3DSURFACE9 textureSurface;
        mTexture->GetSurfaceLevel(0, &textureSurface);

        RECT dest;
        dest.left   = 0;
        dest.top    = 0;
        dest.right  = mWidth;
        dest.bottom = mHeight;
        result = D3DXLoadSurfaceFromSurface(textureSurface, NULL, &dest, mSurface, NULL, NULL, D3DX_FILTER_NONE, 0);
        if (result != D3D_OK) {
            fcn::throwException(
                ("Unable to convert image to display format!"),
                static_cast<char const *>(__FUNCTION__),
                __FILE__,
                __LINE__);
        }

        mSurface->Release();
        mSurface = NULL;
    }

    int DirectX3DImage::getTextureWidth() const
    {
        return mTextureWidth;
    }

    int DirectX3DImage::getTextureHeight() const
    {
        return mTextureHeight;
    }
} // namespace fcn
