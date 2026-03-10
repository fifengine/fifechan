// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#ifndef INCLUDE_FIFECHAN_DEFAULTFONT_HPP_
#define INCLUDE_FIFECHAN_DEFAULTFONT_HPP_

#include <string>

#include "fifechan/font.hpp"
#include "fifechan/platform.hpp"

namespace fcn
{
    /**
     * A font only capable of drawing rectangles.
     * This font is used as a default font when no other font is specified.
     */
    class FIFEGUI_API DefaultFont : public Font
    {
    public:
        /**
         * Draws a glyph as a rectangle. The glyphs will always be drawn as
         * rectangles no matter the glyph.
         *
         * NOTE: You normally won't use this function to draw text since
         *       the Graphics class contains better functions for drawing
         *       text.
         *
         * @param graphics A Graphics object to be used for drawing.
         * @param glyph The glyph to draw.
         * @param x The x coordinate where to draw the glyph.
         * @param y The y coordinate where to draw the glyph.
         * @return The width of the drawn glyph in pixels.
         */
        virtual int drawGlyph(Graphics* graphics, unsigned char glyph, int x, int y);

        // Inherited from Font

        void drawString(Graphics* graphics, std::string const & text, int x, int y) override;

        int getWidth(std::string const & text) const override;

        int getHeight() const override;

        int getStringIndexAt(std::string const & text, int x) const override;
    };
} // namespace fcn

#endif // INCLUDE_FIFECHAN_DEFAULTFONT_HPP_
