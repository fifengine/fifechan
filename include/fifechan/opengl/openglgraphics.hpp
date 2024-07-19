// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

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
    class FCN_EXTENSION_DECLSPEC OpenGLGraphics : public Graphics
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

        virtual void drawImage(Image const * image, int srcX, int srcY, int dstX, int dstY, int width, int height);

        virtual void drawPoint(int x, int y);

        virtual void drawLine(int x1, int y1, int x2, int y2);

        virtual void drawLine(int x1, int y1, int x2, int y2, unsigned int width);

        virtual void drawPolyLine(PointVector const & points, unsigned int width);

        virtual void drawBezier(PointVector const & points, int steps, unsigned int width);

        virtual void drawRectangle(Rectangle const & rectangle);

        virtual void fillRectangle(Rectangle const & rectangle);

        virtual void drawCircle(Point const & p, unsigned int radius);

        virtual void drawFillCircle(Point const & p, unsigned int radius);

        virtual void drawCircleSegment(Point const & p, unsigned int radius, int sangle, int eangle);

        virtual void drawFillCircleSegment(Point const & p, unsigned int radius, int sangle, int eangle);

        virtual void setColor(Color const & color);

        virtual Color const & getColor() const;

    protected:
        int mWidth, mHeight;
        bool mAlpha;
        Color mColor;
    };
} // namespace fcn

#endif // end FCN_OPENGLGRAPHICS_HPP
