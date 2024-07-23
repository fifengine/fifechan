// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#ifndef INCLUDE_FIFECHAN_OPENLAYER_OPENLAYERGRAPHICS_HPP_
#define INCLUDE_FIFECHAN_OPENLAYER_OPENLAYERGRAPHICS_HPP_

#include <OpenLayer.hpp>
#if defined(DELETE)
#    undef DELETE
#endif
#include <string>

#include "fifechan/color.hpp"
#include "fifechan/exception.hpp"
#include "fifechan/graphics.hpp"

namespace fcn
{
    /**
     * OpenLayer implementation of Graphics.
     */
    class FIFEGUI_EXT_API OpenLayerGraphics : public Graphics
    {
    public:
        // Needed so that drawImage(fcn::Image *, int, int) is visible.
        using Graphics::drawImage;

        OpenLayerGraphics();

        /**
         * Constructor.
         *
         * @param width the width of the logical drawing surface. Should be the
         *              same as the screen resolution.
         *
         * @param height the height ot the logical drawing surface. Should be
         *               the same as the screen resolution.
         */
        OpenLayerGraphics(int width, int height);

        virtual ~OpenLayerGraphics();

        /**
         * Sets the target plane on where to draw.
         *
         * @param width the width of the logical drawing surface. Should be the
         *              same as the screen resolution.
         * @param height the height ot the logical drawing surface. Should be
         *               the same as the screen resolution.
         */
        virtual void setTargetPlane(int width, int height);

        // Inherited from Graphics.

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

        ol::Rgba const & getOpenLayerColor() const;

    protected:
        int mWidth, mHeight;
        ol::Rgba mRgba;
        Color mColor;
    };
} // namespace fcn

#endif // INCLUDE_FIFECHAN_OPENLAYER_OPENLAYERGRAPHICS_HPP_
