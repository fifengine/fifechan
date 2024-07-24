// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#ifndef INCLUDE_FIFECHAN_BACKENDS_DIRECTX3D_DIRECTX3DGRAPHICS_HPP_
#define INCLUDE_FIFECHAN_BACKENDS_DIRECTX3D_DIRECTX3DGRAPHICS_HPP_

#include "fifechan/color.hpp"
#include "fifechan/graphics.hpp"
#include "fifechan/platform.hpp"
#include <d3d9.h>

namespace fcn
{
    /**
     * DirectX 3D implementation of the Graphics.
     */
    class FIFEGUI_EXT_API DirectX3DGraphics : public Graphics
    {
    public:
        // Needed so that drawImage(fcn::Image *, int, int) is visible.
        using Graphics::drawImage;

        /**
         * Constructor.
         *
         * @param device The D3D device to use when drawing.
         */
        explicit DirectX3DGraphics(LPDIRECT3DDEVICE9 device);

        /**
         * Constructor.
         *
         * @param device The D3D device to use when drawing.
         * @param width The width of the logical drawing surface. Should be the
         *              same as the screen resolution.
         * @param height The height ot the logical drawing surface. Should be
         *               the same as the screen resolution.
         */
        DirectX3DGraphics(LPDIRECT3DDEVICE9 device, int width, int height);

        virtual ~DirectX3DGraphics();

        /**
         * Sets the target plane on where to draw.
         *
         * @param width The width of the logical drawing surface. Should be the
         *              same as the screen resolution.
         * @param height The height ot the logical drawing surface. Should be
         *               the same as the screen resolution.
         */
        virtual void setTargetPlane(int width, int height);

        /**
         * Sets the D3D device to use when drawing.
         *
         * @param device The D3D device to use when drawing.
         */
        virtual void setDevice(LPDIRECT3DDEVICE9 device);

        /**
         * Gets the D3D device to use when drawing.
         *
         * @return The D3D device to use when drawing.
         */
        virtual LPDIRECT3DDEVICE9 getDevice() const;

        /**
         * Gets the width of the target plane.
         *
         * @return The width of the target plane.
         */
        virtual int getTargetPlaneWidth() const;

        /**
         * Gets the height of the target plane.
         *
         * @return The height of the target plane.
         */
        virtual int getTargetPlaneHeight() const;

        /**
         * A struct for plain vertex information.
         */
        struct Vertex
        {
            float x, y, z, rhw;
            DWORD color;
        };

        /**
         * A struct for vertex information that contains texture coordinates.
         */
        struct VertexWithTexture
        {
            float x, y, z, rhw;
            DWORD color;
            float u, v;
        };

        // Inherited from Graphics

        virtual void _beginDraw();

        virtual void _endDraw();

        virtual bool pushClipArea(Rectangle area);

        virtual void popClipArea();

        virtual void drawImage(Image const * image, int srcX, int srcY, int dstX, int dstY, int width, int height);

        virtual void drawPoint(int x, int y);

        virtual void drawLine(int x1, int y1, int x2, int y2);

        virtual void drawRectangle(Rectangle const & rectangle);

        virtual void fillRectangle(Rectangle const & rectangle);

        virtual void setColor(Color const & color);

        virtual Color const & getColor() const;

    protected:
        /**
         * Hold the width of the target plane.
         */
        int mWidth;

        /**
         * Holds the height of the target plane.
         */
        int mHeight;

        /**
         * True if the last color set includes alpha.
         */
        bool mAlpha;

        /**
         * Holds the last color set.
         */
        Color mColor;

        /**
         * Holds the D3D device to use when drawing.
         */
        LPDIRECT3DDEVICE9 mDevice;
    };
} // namespace fcn

#endif // INCLUDE_FIFECHAN_BACKENDS_DIRECTX3D_DIRECTX3DGRAPHICS_HPP_
