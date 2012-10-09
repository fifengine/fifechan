/***************************************************************************
 *   Copyright (C) 2012 by the fifechan team                               *
 *   https://github.com/fifechan                                           *
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
 * Copyright (c) 2004, 2005, 2006, 2007 Olof Naessén and Per Larsson
 *
 *                                                         Js_./
 * Per Larsson a.k.a finalman                          _RqZ{a<^_aa
 * Olof Naessén a.k.a jansem/yakslem                _asww7!uY`>  )\a//
 *                                                 _Qhm`] _f "'c  1!5m
 * Visit: http://guichan.darkbits.org             )Qk<P ` _: :+' .'  "{[
 *                                               .)j(] .d_/ '-(  P .   S
 * License: (BSD)                                <Td/Z <fP"5(\"??"\a.  .L
 * Redistribution and use in source and          _dV>ws?a-?'      ._/L  #'
 * binary forms, with or without                 )4d[#7r, .   '     )d`)[
 * modification, are permitted provided         _Q-5'5W..j/?'   -?!\)cam'
 * that the following conditions are met:       j<<WP+k/);.        _W=j f
 * 1. Redistributions of source code must       .$%w\/]Q  . ."'  .  mj$
 *    retain the above copyright notice,        ]E.pYY(Q]>.   a     J@\
 *    this list of conditions and the           j(]1u<sE"L,. .   ./^ ]{a
 *    following disclaimer.                     4'_uomm\.  )L);-4     (3=
 * 2. Redistributions in binary form must        )_]X{Z('a_"a7'<a"a,  ]"[
 *    reproduce the above copyright notice,       #}<]m7`Za??4,P-"'7. ).m
 *    this list of conditions and the            ]d2e)Q(<Q(  ?94   b-  LQ/
 *    following disclaimer in the                <B!</]C)d_, '(<' .f. =C+m
 *    documentation and/or other materials      .Z!=J ]e []('-4f _ ) -.)m]'
 *    provided with the distribution.          .w[5]' _[ /.)_-"+?   _/ <W"
 * 3. Neither the name of Guichan nor the      :$we` _! + _/ .        j?
 *    names of its contributors may be used     =3)= _f  (_yQmWW$#(    "
 *    to endorse or promote products derived     -   W,  sQQQQmZQ#Wwa]..
 *    from this software without specific        (js, \[QQW$QWW#?!V"".
 *    prior written permission.                    ]y:.<\..          .
 *                                                 -]n w/ '         [.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT       )/ )/           !
 * HOLDERS AND CONTRIBUTORS "AS IS" AND ANY         <  (; sac    ,    '
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING,               ]^ .-  %
 * BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF            c <   r
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR            aga<  <La
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE          5%  )P'-3L
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR        _bQf` y`..)a
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,          ,J?4P'.P"_(\?d'.,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES               _Pa,)!f/<[]/  ?"
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT      _2-..:. .r+_,.. .
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,     ?a.<%"'  " -'.a_ _,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION)                     ^
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
 * IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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
