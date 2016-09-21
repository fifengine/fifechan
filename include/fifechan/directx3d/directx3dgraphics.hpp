/***************************************************************************
 *   Copyright (c) 2016 by the fifechan team                               *
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
 * Copyright (c) 2004 - 2007 Olof Naessén and Per Larsson
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
 
#ifndef FCN_DIRECTX3DGRAPHICS_HPP
#define FCN_DIRECTX3DGRAPHICS_HPP

#include "fifechan/color.hpp"
#include "fifechan/graphics.hpp"
#include "fifechan/platform.hpp"

#include <d3d9.h>

namespace fcn
{
    /**
     * DirectX 3D implementation of the Graphics.
     */
    class FCN_EXTENSION_DECLSPEC DirectX3DGraphics: public Graphics
    {
    public:

        // Needed so that drawImage(fcn::Image *, int, int) is visible.
        using Graphics::drawImage;

        /**
         * Constructor.
         *
         * @param device The D3D device to use when drawing.
         */
        DirectX3DGraphics(LPDIRECT3DDEVICE9 device);

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

		/**
		 * Destructor.
		 */
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
         * A struct for vertex information that contians texture coordinates.
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

        virtual void drawImage(const Image* image,
                               int srcX,
                               int srcY,
                               int dstX,
                               int dstY,
                               int width,
                               int height);

        virtual void drawPoint(int x, int y);

        virtual void drawLine(int x1, int y1, int x2, int y2);

        virtual void drawRectangle(const Rectangle& rectangle);

        virtual void fillRectangle(const Rectangle& rectangle);

        virtual void setColor(const Color& color);

		virtual const Color& getColor() const;

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
}

#endif // end FCN_DIRECTX3DGRAPHICS_HPP
