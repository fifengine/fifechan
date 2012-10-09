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

#include "fifechan/directx3d/directx3dgraphics.hpp"

#include <d3dx9.h>

#include "fifechan/exception.hpp"
#include "fifechan/image.hpp"
#include "fifechan/directx3d/directx3dimage.hpp"

namespace fcn
{
    DirectX3DGraphics::DirectX3DGraphics(LPDIRECT3DDEVICE9 device)
        : mAlpha(false), 
          mDevice(device)
    {
        DirectX3DGraphics::setTargetPlane(640, 480);
    }

    DirectX3DGraphics::DirectX3DGraphics(LPDIRECT3DDEVICE9 device, int width, int height)
        : mAlpha(false),
          mDevice(device)
    {
        DirectX3DGraphics::setTargetPlane(width, height);
    }

    DirectX3DGraphics::~DirectX3DGraphics()
    {

    }

    void DirectX3DGraphics::_beginDraw()
    {
        D3DXMATRIX identity;
        D3DXMATRIX ortho;
        D3DXMatrixIdentity(&identity);

        D3DXMatrixOrthoLH(&ortho, (float)mWidth, (float)mHeight, 0.0f, 1.0f);
        mDevice->SetTransform(D3DTS_VIEW, &identity);
        mDevice->SetTransform(D3DTS_WORLD, &identity);       
        mDevice->SetTransform(D3DTS_PROJECTION, &ortho);
    
        mDevice->SetRenderState(D3DRS_DITHERENABLE, FALSE);
        mDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
        mDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
        mDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, TRUE);
        mDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
        mDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
        
        mDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
        mDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);

        mDevice->SetTexture(0, 0);

        pushClipArea(Rectangle(0, 0, mWidth, mHeight));
    }

    void DirectX3DGraphics::_endDraw()
    {
        popClipArea();
    }

    bool DirectX3DGraphics::pushClipArea(Rectangle area)
    {
        bool result = Graphics::pushClipArea(area);
    
        const ClipRectangle& carea = mClipStack.top();

        RECT rect;
        rect.left = carea.x;
        rect.top = carea.y;
        rect.right = carea.x + carea.width;
        rect.bottom = carea.y + carea.height;
        
        mDevice->SetScissorRect(&rect);
      
        return result;
    }

    void DirectX3DGraphics::popClipArea()
    {
        Graphics::popClipArea();

        if (mClipStack.empty())
        {
            return;
        }

        const ClipRectangle& carea = mClipStack.top();

        RECT rect;
        rect.left = carea.x;
        rect.top = carea.y;
        rect.right = carea.x + carea.width;
        rect.bottom = carea.y + carea.height;
        
        mDevice->SetScissorRect(&rect);
    }

    void DirectX3DGraphics::setTargetPlane(int width, int height)
    {
        mWidth = width;
        mHeight = height;
    }

    void DirectX3DGraphics::drawImage(const Image* image,
                                      int srcX,
                                      int srcY,
                                      int dstX,
                                      int dstY,
                                      int width,
                                      int height)
    {
		const DirectX3DImage* srcImage = dynamic_cast<const DirectX3DImage*>(image);

        if (srcImage == NULL)
        {
            throw FCN_EXCEPTION("Trying to draw an image of unknown format, must be a DirectXImage.");
        }

        if (mClipStack.empty())
        {
            throw FCN_EXCEPTION("Clip stack is empty, perhaps you called a draw funtion outside of _beginDraw() and _endDraw()?");
        }

        const ClipRectangle& top = mClipStack.top();
        
       
        dstX += top.xOffset;
        dstY += top.yOffset;

         
        // Find DirectX texture coordinates
        float texX1 = srcX / (float)srcImage->getTextureWidth();
        float texY1 = srcY / (float)srcImage->getTextureHeight();
        float texX2 = (srcX+width) / (float)srcImage->getTextureWidth();
        float texY2 = (srcY+height) / (float)srcImage->getTextureHeight();
          
  
        // Check if blending already is enabled
        if (!mAlpha)
        {
            mDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
        }

        DWORD color = 0xFFFFFFFF;
     
        VertexWithTexture vertices[]=
        {
            {(float)dstX, (float)dstY + height, 0.0f, 1.0f, color, texX1, texY2},
            {(float)dstX, (float)dstY, 0.0f, 1.0f, color, texX1, texY1},
            {(float)dstX + width, (float)dstY + height, 0.0f, 1.0f, color, texX2, texY2},
            {(float)dstX + width, (float)dstY, 0.0f, 1.0f, color, texX2, texY1},
        };

        mDevice->SetFVF(D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1);
        mDevice->SetTexture(0, srcImage->getTexture());
        mDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vertices, sizeof(VertexWithTexture));
        mDevice->SetTexture(0, 0);

        if (!mAlpha)
        {
            mDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE); 
        }
    }

    void DirectX3DGraphics::drawPoint(int x, int y)
    {
        if (mClipStack.empty())
        {
            throw FCN_EXCEPTION("Clip stack is empty, perhaps you called a draw funtion outside of _beginDraw() and _endDraw()?");
        }

        const ClipRectangle& top = mClipStack.top();
       
        DWORD color = D3DCOLOR_RGBA(mColor.r, mColor.g, mColor.b, mColor.a);
        Vertex vertices[]=
        {
           {(float)x, (float)y, 0.0f, 1.0f, color},
           {(float)x + 1, (float)y + 1, 0.0f, 1.0f, color}
        };

        mDevice->SetFVF(D3DFVF_XYZRHW|D3DFVF_DIFFUSE);
        mDevice->DrawPrimitiveUP(D3DPT_LINELIST, 1, vertices, sizeof(Vertex));
    }

    void DirectX3DGraphics::drawLine(int x1, int y1, int x2, int y2)
    {
        if (mClipStack.empty())
        {
            throw FCN_EXCEPTION("Clip stack is empty, perhaps you called a draw funtion outside of _beginDraw() and _endDraw()?");
        }

        const ClipRectangle& top = mClipStack.top();
      
        x1 += top.xOffset;
        y1 += top.yOffset;
        x2 += top.xOffset;
        y2 += top.yOffset;

        // Add the last point on the line as it is excluded 
        // when drawing with DirectX 9     
        if (x1 < x2 && y1 > y2)
        {
            x2++;
            y2--;
        }        
        else if (x1 < x2 && y1 < y2)
        {
            x2++;
            y2++;
        }
        else if (x1 > x2 && y1 > y2)
        {
            x1++;
            y1++;
        }
        else if (x1 > x2 && y1 < y2)
        {
            x1++;
            y1--;
        }

        DWORD color = D3DCOLOR_RGBA(mColor.r, mColor.g, mColor.b, mColor.a);
        Vertex vertices[]=
        {
           {(float)x1, (float)y1, 0.0f, 1.0f, color},
           {(float)x2, (float)y2, 0.0f, 1.0f, color}
        };

        mDevice->SetFVF(D3DFVF_XYZRHW|D3DFVF_DIFFUSE);
        mDevice->DrawPrimitiveUP(D3DPT_LINELIST, 1, vertices, sizeof(Vertex));
    }

    void DirectX3DGraphics::drawRectangle(const Rectangle& rectangle)
    {
        if (mClipStack.empty())
        {
            throw FCN_EXCEPTION("Clip stack is empty, perhaps you called a draw funtion outside of _beginDraw() and _endDraw()?");
        }

        const ClipRectangle& top = mClipStack.top();
       
        DWORD color = D3DCOLOR_RGBA(mColor.r, mColor.g, mColor.b, mColor.a);
        Vertex vertices[]=
        {
           {(float)rectangle.x + top.xOffset, 
            (float)rectangle.y + top.yOffset, 0.0f, 1.0f, color},
           {(float)rectangle.x + rectangle.width - 1 + top.xOffset, 
            (float)rectangle.y + top.yOffset, 0.0f, 1.0f, color},

           {(float)rectangle.x + rectangle.width - 1 + top.xOffset, 
            (float)rectangle.y + top.yOffset, 0.0f, 1.0f, color},
           {(float)rectangle.x + rectangle.width - 1 + top.xOffset, 
            (float)rectangle.y + rectangle.height - 1 + top.yOffset, 0.0f, 1.0f, color},

           {(float)rectangle.x + rectangle.width - 1 + top.xOffset, 
            (float)rectangle.y + rectangle.height - 1 + top.yOffset, 0.0f, 1.0f, color},
           {(float)rectangle.x + top.xOffset,
            (float)rectangle.y + rectangle.height - 1 + top.yOffset, 0.0f, 1.0f, color},

           {(float)rectangle.x + top.xOffset, 
            (float)rectangle.y + rectangle.height - 1 + top.yOffset, 0.0f, 1.0f, color},
           {(float)rectangle.x + top.xOffset, 
            (float)rectangle.y + top.yOffset, 0.0f, 1.0f, color}
        };

        mDevice->SetFVF(D3DFVF_XYZRHW|D3DFVF_DIFFUSE);
        mDevice->DrawPrimitiveUP(D3DPT_LINELIST, 4, vertices, sizeof(Vertex));
    }

    void DirectX3DGraphics::fillRectangle(const Rectangle& rectangle)
    {
        if (mClipStack.empty())
        {
            throw FCN_EXCEPTION("Clip stack is empty, perhaps you called a draw funtion outside of _beginDraw() and _endDraw()?");
        }

        const ClipRectangle& top = mClipStack.top();
       
        DWORD color = D3DCOLOR_RGBA(mColor.r, mColor.g, mColor.b, mColor.a);
        Vertex vertices[]=
        {
           {(float)rectangle.x + top.xOffset, 
            (float)rectangle.y + rectangle.height + top.yOffset, 0.0f, 1.0f, color},
           {(float)rectangle.x + top.xOffset, 
            (float)rectangle.y + top.yOffset, 0.0f, 1.0f, color},
           {(float)rectangle.x + rectangle.width + top.xOffset, 
            (float)rectangle.y + rectangle.height + top.yOffset, 0.0f, 1.0f, color},
           {(float)rectangle.x + rectangle.width + top.xOffset, 
            (float)rectangle.y + top.yOffset, 0.0f, 1.0f, color}
        };

        mDevice->SetFVF(D3DFVF_XYZRHW|D3DFVF_DIFFUSE);
        mDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vertices, sizeof(Vertex));
    }

    void DirectX3DGraphics::setColor(const Color& color)
    {
        mColor = color;

        mAlpha = color.a != 255;

        if (mAlpha)
        {
            mDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
        }
        else
        {
            mDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
        }
    }

    const Color& DirectX3DGraphics::getColor() const
    {
        return mColor;
    }

    void DirectX3DGraphics::setDevice(LPDIRECT3DDEVICE9 device)
    {
        mDevice = device;
    }

    LPDIRECT3DDEVICE9 DirectX3DGraphics::getDevice() const
    {
        return mDevice;
    }

    int DirectX3DGraphics::getTargetPlaneWidth() const
    {
        return mWidth;
    }
   
    int DirectX3DGraphics::getTargetPlaneHeight() const
    {
        return mHeight;
    }
}
