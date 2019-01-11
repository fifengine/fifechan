/***************************************************************************
 *   Copyright (c) 2017-2019 by the fifechan team                               *
 *   https://github.com/fifengine/fifechan                                 *
 *   This file is part of fifechan.                                        *
 *                                                                         *
 *   fifechan is free software; you can redistribute it and/or             *
 *   modify it under the terms of the GNU Lesser General Public            *
 *   License as published by the Free Software Foundation; either          *
 *   version 2.1 of the License, or (at your option) any later version.    *
 *                                                                         *
 *   This library is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU     *
 *   Lesser General Public License for more details.                       *
 *                                                                         *
 *   You should have received a copy of the GNU Lesser General Public      *
 *   License along with this library; if not, write to the                 *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA          *
 ***************************************************************************/

/*      _______   __   __   __   ______   __   __   _______   __   __
 *     / _____/\ / /\ / /\ / /\ / ____/\ / /\ / /\ / ___  /\ /  |\/ /\
 *    / /\____\// / // / // / // /\___\// /_// / // /\_/ / // , |/ / /
 *   / / /__   / / // / // / // / /    / ___  / // ___  / // /| ' / /
 *  / /_// /\ / /_// / // / // /_/_   / / // / // /\_/ / // / |  / /
 * /______/ //______/ //_/ //_____/\ /_/ //_/ //_/ //_/ //_/ /|_/ /
 * \______\/ \______\/ \_\/ \_____\/ \_\/ \_\/ \_\/ \_\/ \_\/ \_\/
 *
 * Copyright (c) 2004 - 2008 Olof Naessén and Per Larsson
 *
 *
 * Per Larsson a.k.a finalman
 * Olof Naessén a.k.a jansem/yakslem
 *
 * Visit: http://guichan.sourceforge.net
 *
 * License: (BSD)
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name of Guichan nor the names of its contributors may
 *    be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 * TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 * For comments regarding functions please see the header file.
 */

#include "fifechan/directx3d/directx3dimage.hpp"

#include "fifechan/exception.hpp"

namespace fcn
{
    DirectX3DImage::DirectX3DImage(LPDIRECT3DSURFACE9 surface,   
                                   LPDIRECT3DDEVICE9 device,
                                   int width,
                                   int height,
                                   bool convertToDisplayFormat)
    {
        mTexture = NULL;
        mSurface = surface;
        mDevice = device;
        mWidth = width;
        mHeight = height;
        mTextureWidth = 1;
        mTextureHeight = 1;

        while(mTextureWidth < mWidth)
        {
            mTextureWidth *= 2;
        }

        while(mTextureHeight < mHeight)
        {
            mTextureHeight *= 2;
        }

        if (convertToDisplayFormat)
        {
            DirectX3DImage::convertToDisplayFormat();
        }
    }

    DirectX3DImage::~DirectX3DImage()
    {
        if (mAutoFree)
        {
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
        if (mSurface == NULL)
        {
            throw FCN_EXCEPTION("Image has been converted to display format");
        }

        if (x < 0 || x >= mWidth || y < 0 || y >= mHeight)
		{
			throw FCN_EXCEPTION("Coordinates outside of the image");
		}

        D3DLOCKED_RECT lockedRect;
        mSurface->LockRect(&lockedRect, NULL, D3DLOCK_READONLY);
        DWORD* pixels = (DWORD*)lockedRect.pBits;
        D3DXCOLOR color = pixels[lockedRect.Pitch / sizeof(DWORD) * y + x];
        mSurface->UnlockRect();

        return Color(color.r*255, 
                     color.g*255, 
                     color.b*255, 
                     color.a*255);
    }

    void DirectX3DImage::putPixel(int x, int y, const Color& color)
    {
       if (mSurface == NULL)
        {
            throw FCN_EXCEPTION("Image has been converted to display format");
        }

        if (x < 0 || x >= mWidth || y < 0 || y >= mHeight)
		{
			throw FCN_EXCEPTION("Coordinates outside of the image");
		}

        D3DLOCKED_RECT lockedRect;
        mSurface->LockRect(&lockedRect, NULL, 0);
        DWORD* pixels = (DWORD*)lockedRect.pBits;
        pixels[lockedRect.Pitch / sizeof(DWORD) * y + x] = D3DCOLOR_RGBA(color.r, color.g, color.b, color.a);
        mSurface->UnlockRect();
    }

    void DirectX3DImage::convertToDisplayFormat()
    {
        HRESULT result;
        result = D3DXCreateTexture(mDevice, 
                                   mTextureWidth, 
                                   mTextureHeight, 
                                   D3DX_DEFAULT, 
                                   0,
                                   D3DFMT_A8R8G8B8,
                                   D3DPOOL_MANAGED,
                                   &mTexture);
        if (result != D3D_OK)
        {
            throw FCN_EXCEPTION("Unable to convert image to display format!");
        }

        LPDIRECT3DSURFACE9 textureSurface;
        mTexture->GetSurfaceLevel(0, &textureSurface);

        RECT dest;
        dest.left = 0;
        dest.top = 0;
        dest.right = mWidth;
        dest.bottom = mHeight;
        result = D3DXLoadSurfaceFromSurface(textureSurface, 
                                            NULL, 
                                            &dest, 
                                            mSurface, 
                                            NULL, 
                                            NULL, 
                                            D3DX_FILTER_NONE,
                                            0);
        if (result != D3D_OK)
        {
            throw FCN_EXCEPTION("Unable to convert image to display format!");
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
}
