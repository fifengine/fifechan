// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#ifndef FCN_ALLEGROGRAPHICS_HPP
#define FCN_ALLEGROGRAPHICS_HPP

#include <allegro.h>

#include "fifechan/color.hpp"
#include "fifechan/graphics.hpp"
#include "fifechan/platform.hpp"

namespace fcn
{
    class Font;

    /**
     * Allegro implementation of Graphics.
     */
    class FIFEGUI_EXT_API AllegroGraphics : public Graphics
    {
    public:
        // Needed so that drawImage(fcn::Image *, int, int) is visible.
        using Graphics::drawImage;

        /**
         * Constructor.
         */
        AllegroGraphics();

        /**
         * Contsructor. Sets the drawing target.
         *
         * @param target the target to draw to.
         */
        AllegroGraphics(BITMAP* target);

        /**
         * Destructor.
         */
        virtual ~AllegroGraphics();

        /**
         * Sets the target bitmap to draw to. It can be any bitmap with the same
         * bit-depth as the screen. However, if you pass the screen bitmap you
         * will probably get flicker. Use a double buffer!
         *
         * @param target the bitmap to draw to.
         */
        virtual void setTarget(BITMAP* target);

        /**
         * Gets the target bitmap.
         *
         * @return the target bitmap.
         */
        virtual BITMAP* getTarget();

        /**
         * Gets the color in Allegro format.
         *
         * @return the color in Allegro format.
         */
        int getAllegroColor() const;

        /**
         * Draws an Allegro bitmap.
         *
         * @param bitmap the bitmap to draw.
         * @param dstX the x-coordinate where to draw the bitmap.
         * @param dstY the y-coordinate where to draw the bitmap.
         */
        virtual void drawBitmap(BITMAP* bitmap, int dstX, int dstY);

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
        BITMAP* mTarget;
        bool mClipNull;
        int mAllegroColor;
        Color mColor;
    };
} // namespace fcn

#endif // end FCN_ALLEGROGRAPHICS_HPP
