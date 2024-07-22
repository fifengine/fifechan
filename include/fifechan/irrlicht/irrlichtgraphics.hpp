// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#ifndef FCN_IRRLICHTGRAPHICS_HPP
#define FCN_IRRLICHTGRAPHICS_HPP

#include "fifechan/color.hpp"
#include "fifechan/graphics.hpp"
#include "fifechan/platform.hpp"
#include "irrlicht.h"

namespace fcn
{
    class Image;
    class Rectangle;

    /**
     * Irrlicht implementation of the Graphics.
     */
    class FIFEGUI_EXT_API IrrlichtGraphics : public Graphics
    {
    public:
        // Needed so that drawImage(fcn::Image *, int, int) is visible.
        using Graphics::drawImage;

        /**
         * Constructor.
         *
         * @param driver the Irrlicht video driver to draw to.
         */
        IrrlichtGraphics(irr::video::IVideoDriver* driver);

        /**
         * Destructor.
         */
        ~IrrlichtGraphics();

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
        Color mColor;
        irr::video::IVideoDriver* mDriver;
    };
} // namespace fcn

#endif // end FCN_IRRLICHTGRAPHICS_HPP
