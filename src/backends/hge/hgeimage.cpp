// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#include "fifechan/backends/hge/hgeimage.hpp"

#include "fifechan/exception.hpp"

namespace fcn
{
    HGE* HGEImage::mHGE = NULL;

    HGEImage::HGEImage(HTEXTURE texture, bool autoFree) : mTexture(texture), mAutoFree(autoFree)
    {
        mHGE = hgeCreate(HGE_VERSION);
    }

    HGEImage::~HGEImage()
    {
        if (mAutoFree) {
            free();
        }

        mHGE->Release();
    }

    void HGEImage::free()
    {
        if (mHGESprite != NULL) {
            delete mHGESprite;
            mHGESprite = NULL;
        }

        mHGE->Texture_Free(mTexture);
    }

    hgeSprite* HGEImage::getSprite() const
    {
        return mHGESprite;
    }

    int HGEImage::getWidth() const
    {
        return mHGE->Texture_GetWidth(mTexture, true);
    }

    int HGEImage::getHeight() const
    {
        return mHGE->Texture_GetHeight(mTexture, true);
    }

    Color HGEImage::getPixel(int x, int y)
    {
        DWORD* pLockPtr = mHGE->Texture_Lock(mTexture);

        if (pLockPtr == NULL) {
            fcn::throwException(
                ("Locking of the texture failed. HGE only support locking of 32bit textures."),
                static_cast<char const *>(__FUNCTION__),
                __FILE__,
                __LINE__);
        }

        DWORD color = pLockPtr[x + y * mHGE->Texture_GetWidth(mTexture, true)];

        mHGE->Texture_Unlock(mTexture);

        return Color(GETR(color), GETG(color), GETB(color), GETA(color));
    }

    void HGEImage::putPixel(int x, int y, Color const & color)
    {
        DWORD hardwareColor = ARGB(color.a, color.r, color.g, color.b);

        DWORD* pLockPtr = mHGE->Texture_Lock(mTexture, false);

        if (pLockPtr == NULL) {
            fcn::throwException(
                ("Locking of the texture failed. HGE only support locking of 32bit textures."),
                static_cast<char const *>(__FUNCTION__),
                __FILE__,
                __LINE__);
        }

        pLockPtr[x + y * mHGE->Texture_GetWidth(mTexture, true)] = hardwareColor;

        mHGE->Texture_Unlock(mTexture);
    }

    void HGEImage::convertToDisplayFormat()
    {
        DWORD* pLockPtr = mHGE->Texture_Lock(mTexture);

        if (pLockPtr == NULL) {
            fcn::throwException(
                ("Locking of the texture failed. HGE only support locking of 32bit textures."),
                static_cast<char const *>(__FUNCTION__),
                __FILE__,
                __LINE__);
        }

        int i;
        int end = mHGE->Texture_GetWidth(mTexture, true) * mHGE->Texture_GetHeight(mTexture, true);

        for (i = 0; i < end; i++) {
            DWORD color = pLockPtr[i];
            if (GETR(color) == 0xFF && GETG(color) == 0x00 && GETB(color) == 0xFF) {
                pLockPtr[i] = ARGB(0x00, 0x00, 0x00, 0x00);
            }
        }

        mHGE->Texture_Unlock(mTexture);

        mHGESprite = new hgeSprite(
            mTexture, 0, 0, mHGE->Texture_GetWidth(mTexture, true), mHGE->Texture_GetHeight(mTexture, true));
    }
} // namespace fcn
