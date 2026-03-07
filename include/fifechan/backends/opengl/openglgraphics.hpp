// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#ifndef INCLUDE_FIFECHAN_BACKENDS_OPENGL_OPENGLGRAPHICS_HPP_
#define INCLUDE_FIFECHAN_BACKENDS_OPENGL_OPENGLGRAPHICS_HPP_

#include "fifechan/color.hpp"
#include "fifechan/graphics.hpp"
#include "fifechan/platform.hpp"

namespace fcn
{
    /**
     * OpenGL implementation of the Graphics.
     */
    class FIFEGUI_EXT_API OpenGLGraphics : public Graphics
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

        ~OpenGLGraphics() override;

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

        void _beginDraw() override;

        void _endDraw() override;

        bool pushClipArea(Rectangle area) override;

        void popClipArea() override;

        void drawImage(Image const * image, int srcX, int srcY, int dstX, int dstY, int width, int height) override;

        void drawPoint(int x, int y) override;

        void drawLine(int x1, int y1, int x2, int y2) override;

        void drawLine(int x1, int y1, int x2, int y2, unsigned int width) override;

        void drawPolyLine(PointVector const & points, unsigned int width) override;

        void drawBezier(PointVector const & points, int steps, unsigned int width) override;

        void drawRectangle(Rectangle const & rectangle) override;

        void fillRectangle(Rectangle const & rectangle) override;

        void drawCircle(Point const & p, unsigned int radius) override;

        void drawFillCircle(Point const & p, unsigned int radius) override;

        void drawCircleSegment(Point const & p, unsigned int radius, int sangle, int eangle) override;

        void drawFillCircleSegment(Point const & p, unsigned int radius, int sangle, int eangle) override;

        void setColor(Color const & color) override;

        Color const & getColor() const override;

    protected:
        int mWidth{};
        int mHeight{};
        bool mAlpha{};
        Color mColor;
    };
} // namespace fcn

#endif // INCLUDE_FIFECHAN_BACKENDS_OPENGL_OPENGLGRAPHICS_HPP_
