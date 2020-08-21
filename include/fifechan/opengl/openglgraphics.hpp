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
 * Copyright (c) 2004 - 2008 Olof Naess�n and Per Larsson
 *
 *
 * Per Larsson a.k.a finalman
 * Olof Naess�n a.k.a jansem/yakslem
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

#ifndef FCN_OPENGLGRAPHICS_HPP
#define FCN_OPENGLGRAPHICS_HPP

#include "fifechan/color.hpp"
#include "fifechan/graphics.hpp"
#include "fifechan/platform.hpp"

namespace fcn
{
    /**
     * OpenGL implementation of the Graphics.
     */
    class FCN_EXTENSION_DECLSPEC OpenGLGraphics: public Graphics
    {
    public:

        // Needed so that drawImage(fcn::Image *, int, int) is visible.
        using Graphics::drawImage;

        /**
         * Constructor.
         */
        OpenGLGraphics();

        /**
         * Constructor.
		 *
		 * @param width the width of the logical drawing surface. Should be the
         *              same as the screen resolution.
		 *
		 * @param height the height ot the logical drawing surface. Should be
		 *               the same as the screen resolution.
		 */
        OpenGLGraphics(int width, int height);

		/**
		 * Destructor.
		 */
        virtual ~OpenGLGraphics();

        /**
         * Sets the target plane on where to draw.
		 *
		 * @param width the width of the logical drawing surface. Should be the
		 *              same as the screen resolution.
		 * @param height the height ot the logical drawing surface. Should be
		 *               the same as the screen resolution.
         */
        virtual void setTargetPlane(int width, int height);
       
        /**
         * Gets the target plane width.
         *
         * @return The target plane width.
         */
        virtual int getTargetPlaneWidth() const;

        /**
         * Gets the target plane height.
         *
         * @return The target plane height.
         */
        virtual int getTargetPlaneHeight() const;


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

        virtual void drawLine(int x1, int y1, int x2, int y2, unsigned int width);

        virtual void drawPolyLine(const PointVector& points, unsigned int width);

        virtual void drawBezier(const PointVector& points, int steps, unsigned int width);

        virtual void drawRectangle(const Rectangle& rectangle);

        virtual void fillRectangle(const Rectangle& rectangle);

        virtual void drawCircle(const Point& p, unsigned int radius);

        virtual void drawFillCircle(const Point& p, unsigned int radius);

        virtual void drawCircleSegment(const Point& p, unsigned int radius, int sangle, int eangle);

        virtual void drawFillCircleSegment(const Point& p, unsigned int radius, int sangle, int eangle);

        virtual void setColor(const Color& color);

		virtual const Color& getColor() const;

    protected:
        int mWidth, mHeight;
		bool mAlpha;
        Color mColor;
    };
}

#endif // end FCN_OPENGLGRAPHICS_HPP
