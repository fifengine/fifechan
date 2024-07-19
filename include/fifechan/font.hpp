// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#ifndef FCN_FONT_HPP
#define FCN_FONT_HPP

#include <string>
#include "fifechan/platform.hpp"

namespace fcn
{
    class Graphics;

    /**
     * Interface for a font.
     *
     * @see ImageFont
     */
    class FIFEGUI_API Font
    {
    public:
        /**
         * Destructor.
         */
        virtual ~Font() = default;

        /**
         * Gets the width of a string. The width of a string is not necesserily
         * the sum of all the widths of it's glyphs.
         *
         * @param text The string to return the width of.
         * @return The width of a string.
         */
        virtual int getWidth(std::string const & text) const = 0;

        /**
         * Gets the height of the glyphs in the font.
         *
         * @return The height of the glyphs int the font.
         */
        virtual int getHeight() const = 0;

        /**
         * Gets a string index in a string providing an x coordinate.
         * Used to retrive a string index (for a character in a
         * string) at a certain x position. It is especially useful
         * when a mouse clicks in a TextField and you want to know which
         * character was clicked.
         *
         * @return A string index in a string providing an x coordinate.
         */
        virtual int getStringIndexAt(std::string const & text, int x) const;

        /**
         * Draws a string.
         *
         * NOTE: You normally won't use this function to draw text since
         *       Graphics contains better functions for drawing text.
         *
         * @param graphics A Graphics object to use for drawing.
         * @param text The string to draw.
         * @param x The x coordinate where to draw the string.
         * @param y The y coordinate where to draw the string.
         */
        virtual void drawString(Graphics* graphics, std::string const & text, int x, int y) = 0;
    };
} // namespace fcn

#endif // end FCN_FONT_HPP
