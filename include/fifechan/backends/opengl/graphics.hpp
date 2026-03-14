// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#ifndef INCLUDE_FIFECHAN_BACKENDS_OPENGL_GRAPHICS_HPP_
#define INCLUDE_FIFECHAN_BACKENDS_OPENGL_GRAPHICS_HPP_

#include "fifechan/backends/opengl/image.hpp"
#include "fifechan/color.hpp"
#include "fifechan/graphics.hpp"
#include "fifechan/platform.hpp"

namespace fcn::opengl
{
    /**
     * OpenGL-specific implementation of the Graphics interface.
     *
     * @ingroup graphics
     */
    class FIFEGUI_EXT_API Graphics : public fcn::Graphics
    {
    public:
        using fcn::Graphics::drawImage;

        /**
         * Constructor.
         */
        Graphics();

        /**
         * Constructor.
         *
         * @param width the width of the logical drawing surface. Should be the
         *              same as the screen resolution.
         *
         * @param height the height ot the logical drawing surface. Should be
         *               the same as the screen resolution.
         */
        Graphics(int width, int height);

        ~Graphics() override;

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

        void drawImage(
            fcn::Image const * image, int srcX, int srcY, int dstX, int dstY, int width, int height) override;

        /**
         * Draws an OpenGL-specific image.
         *
         * @param image Pointer to the OpenGL image to draw.
         * @param srcX Source X coordinate.
         * @param srcY Source Y coordinate.
         * @param dstX Destination X coordinate.
         * @param dstY Destination Y coordinate.
         * @param width Width of the region to draw.
         * @param height Height of the region to draw.
         */
        void drawImage(fcn::opengl::Image const * image, int srcX, int srcY, int dstX, int dstY, int width, int height);

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
        /** Width of the logical target plane. */
        int mWidth{};

        /** Height of the logical target plane. */
        int mHeight{};

        /** Whether alpha blending is enabled. */
        bool mAlpha{};

        /** Current drawing color. */
        Color mColor;
    };
} // namespace fcn::opengl

namespace fcn
{
    using OpenGLGraphics = fcn::opengl::Graphics;
}

#endif // INCLUDE_FIFECHAN_BACKENDS_OPENGL_GRAPHICS_HPP_
