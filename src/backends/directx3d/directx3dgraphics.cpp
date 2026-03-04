// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#include "fifechan/backends/directx3d/directx3dgraphics.hpp"

#include "fifechan/backends/directx3d/directx3dimage.hpp"
#include "fifechan/exception.hpp"
#include "fifechan/image.hpp"
#include <d3dx9.h>

namespace fcn
{
    DirectX3DGraphics::DirectX3DGraphics(LPDIRECT3DDEVICE9 device) : mAlpha(false), mDevice(device)
    {
        DirectX3DGraphics::setTargetPlane(640, 480);
    }

    DirectX3DGraphics::DirectX3DGraphics(LPDIRECT3DDEVICE9 device, int width, int height) :
        mAlpha(false), mDevice(device)
    {
        DirectX3DGraphics::setTargetPlane(width, height);
    }

    DirectX3DGraphics::~DirectX3DGraphics() { }

    void DirectX3DGraphics::_beginDraw()
    {
        D3DXMATRIX identity;
        D3DXMATRIX ortho;
        D3DXMatrixIdentity(&identity);

        D3DXMatrixOrthoLH(&ortho, static_cast<float>(mWidth), static_cast<float>(mHeight), 0.0F, 1.0F);
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

        ClipRectangle const & clip_rect = mClipStack.top();

        RECT rect;
        rect.left   = clip_rect.x;
        rect.top    = clip_rect.y;
        rect.right  = clip_rect.x + clip_rect.width;
        rect.bottom = clip_rect.y + clip_rect.height;

        mDevice->SetScissorRect(&rect);

        return result;
    }

    void DirectX3DGraphics::popClipArea()
    {
        Graphics::popClipArea();

        if (mClipStack.empty()) {
            return;
        }

        ClipRectangle const & clip_rect = mClipStack.top();

        RECT rect;
        rect.left   = clip_rect.x;
        rect.top    = clip_rect.y;
        rect.right  = clip_rect.x + clip_rect.width;
        rect.bottom = clip_rect.y + clip_rect.height;

        mDevice->SetScissorRect(&rect);
    }

    void DirectX3DGraphics::setTargetPlane(int width, int height)
    {
        mWidth  = width;
        mHeight = height;
    }

    void DirectX3DGraphics::drawImage(
        Image const * image, int srcX, int srcY, int dstX, int dstY, int width, int height)
    {
        DirectX3DImage const * srcImage = dynamic_cast<DirectX3DImage const *>(image);

        if (srcImage == nullptr) {
            throwException(
                ("Trying to draw an image of unknown format, must be a DirectXImage.");
        }

        if (mClipStack.empty()) {
            throwException(
                "The clip stack is empty, perhaps you called a draw function outside of _beginDraw() and _endDraw()?");
        }

        ClipRectangle const & top = mClipStack.top();

        dstX += top.xOffset;
        dstY += top.yOffset;

        // Find DirectX texture coordinates
        float texX1 = srcX / (float)srcImage->getTextureWidth();
        float texY1 = srcY / (float)srcImage->getTextureHeight();
        float texX2 = (srcX + width) / (float)srcImage->getTextureWidth();
        float texY2 = (srcY + height) / (float)srcImage->getTextureHeight();

        // Check if blending already is enabled
        if (!mAlpha) {
            mDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
        }

        DWORD color = 0xFFFFFFFF;

        VertexWithTexture vertices[] = {
            {(float)dstX, (float)dstY + height, 0.0F, 1.0F, color, texX1, texY2},
            {(float)dstX, (float)dstY, 0.0F, 1.0F, color, texX1, texY1},
            {(float)dstX + width, (float)dstY + height, 0.0F, 1.0F, color, texX2, texY2},
            {(float)dstX + width, (float)dstY, 0.0F, 1.0F, color, texX2, texY1},
        };

        mDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);
        mDevice->SetTexture(0, srcImage->getTexture());
        mDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vertices, sizeof(VertexWithTexture));
        mDevice->SetTexture(0, 0);

        if (!mAlpha) {
            mDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
        }
    }

    void DirectX3DGraphics::drawPoint(int x, int y)
    {
        if (mClipStack.empty()) {
            throwException(
                "The clip stack is empty, perhaps you called a draw function
                 outside of _beginDraw() and _endDraw()?");
        }

        ClipRectangle const & top = mClipStack.top();

        DWORD color       = D3DCOLOR_RGBA(mColor.r, mColor.g, mColor.b, mColor.a);
        Vertex vertices[] = {{(float)x, (float)y, 0.0F, 1.0F, color}, {(float)x + 1, (float)y + 1, 0.0F, 1.0F, color}};

        mDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
        mDevice->DrawPrimitiveUP(D3DPT_LINELIST, 1, vertices, sizeof(Vertex));
    }

    void DirectX3DGraphics::drawLine(int x1, int y1, int x2, int y2)
    {
        if (mClipStack.empty()) {
            throwException(
                "The clip stack is empty, perhaps you called a draw function outside of _beginDraw() and _endDraw()?");
        }

        ClipRectangle const & top = mClipStack.top();

        x1 += top.xOffset;
        y1 += top.yOffset;
        x2 += top.xOffset;
        y2 += top.yOffset;

        // Add the last point on the line as it is excluded
        // when drawing with DirectX 9
        if (x1 < x2 && y1 > y2) {
            x2++;
            y2--;
        } else if (x1 < x2 && y1 < y2) {
            x2++;
            y2++;
        } else if (x1 > x2 && y1 > y2) {
            x1++;
            y1++;
        } else if (x1 > x2 && y1 < y2) {
            x1++;
            y1--;
        }

        DWORD color       = D3DCOLOR_RGBA(mColor.r, mColor.g, mColor.b, mColor.a);
        Vertex vertices[] = {{(float)x1, (float)y1, 0.0F, 1.0F, color}, {(float)x2, (float)y2, 0.0F, 1.0F, color}};

        mDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
        mDevice->DrawPrimitiveUP(D3DPT_LINELIST, 1, vertices, sizeof(Vertex));
    }

    void DirectX3DGraphics::drawRectangle(Rectangle const & rectangle)
    {
        if (mClipStack.empty()) {
            throwException(
                "The clip stack is empty, perhaps you called a draw function outside of _beginDraw() and _endDraw()?");
        }

        ClipRectangle const & top = mClipStack.top();

        DWORD color       = D3DCOLOR_RGBA(mColor.r, mColor.g, mColor.b, mColor.a);
        Vertex vertices[] = {
            {(float)rectangle.x + top.xOffset, (float)rectangle.y + top.yOffset, 0.0F, 1.0F, color},
            {(float)rectangle.x + rectangle.width - 1 + top.xOffset,
             (float)rectangle.y + top.yOffset,
             0.0F,
             1.0F,
             color},

            {(float)rectangle.x + rectangle.width - 1 + top.xOffset,
             (float)rectangle.y + top.yOffset,
             0.0F,
             1.0F,
             color},
            {(float)rectangle.x + rectangle.width - 1 + top.xOffset,
             (float)rectangle.y + rectangle.height - 1 + top.yOffset,
             0.0F,
             1.0F,
             color},

            {(float)rectangle.x + rectangle.width - 1 + top.xOffset,
             (float)rectangle.y + rectangle.height - 1 + top.yOffset,
             0.0F,
             1.0F,
             color},
            {(float)rectangle.x + top.xOffset,
             (float)rectangle.y + rectangle.height - 1 + top.yOffset,
             0.0F,
             1.0F,
             color},

            {(float)rectangle.x + top.xOffset,
             (float)rectangle.y + rectangle.height - 1 + top.yOffset,
             0.0F,
             1.0F,
             color},
            {(float)rectangle.x + top.xOffset, (float)rectangle.y + top.yOffset, 0.0F, 1.0F, color}};

        mDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
        mDevice->DrawPrimitiveUP(D3DPT_LINELIST, 4, vertices, sizeof(Vertex));
    }

    void DirectX3DGraphics::fillRectangle(Rectangle const & rectangle)
    {
        if (mClipStack.empty()) {
            throwException(
                "The clip stack is empty, perhaps you called a draw function outside of _beginDraw() and _endDraw()?");
        }

        ClipRectangle const & top = mClipStack.top();

        DWORD color       = D3DCOLOR_RGBA(mColor.r, mColor.g, mColor.b, mColor.a);
        Vertex vertices[] = {
            {(float)rectangle.x + top.xOffset, (float)rectangle.y + rectangle.height + top.yOffset, 0.0F, 1.0F, color},
            {(float)rectangle.x + top.xOffset, (float)rectangle.y + top.yOffset, 0.0F, 1.0F, color},
            {(float)rectangle.x + rectangle.width + top.xOffset,
             (float)rectangle.y + rectangle.height + top.yOffset,
             0.0F,
             1.0F,
             color},
            {(float)rectangle.x + rectangle.width + top.xOffset, (float)rectangle.y + top.yOffset, 0.0F, 1.0F, color}};

        mDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
        mDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vertices, sizeof(Vertex));
    }

    void DirectX3DGraphics::setColor(Color const & color)
    {
        mColor = color;

        mAlpha = color.a != 255;

        if (mAlpha) {
            mDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
        } else {
            mDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
        }
    }

    Color const & DirectX3DGraphics::getColor() const
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
} // namespace fcn
