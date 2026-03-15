// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#include "fifechan/defaultfont.hpp"

#include <string>

#include "fifechan/graphics.hpp"

namespace fcn
{
    int DefaultFont::getHeight() const
    {
        return 8;
    }

    int DefaultFont::getWidth(std::string const & text) const
    {
        return 8 * text.size();
    }

    int DefaultFont::drawGlyph(Graphics* graphics, unsigned char glyph, int x, int y)
    {
        (void)glyph; // unused parameter

        graphics->drawRectangle(x, y, 8, 8);

        return 8;
    }

    void DefaultFont::drawString(Graphics* graphics, std::string const & text, int x, int y)
    {
        int const glyphWidth = getWidth(" ");
        for (char const & ch : text) {
            drawGlyph(graphics, ch, x, y);
            x += glyphWidth;
        }
    }

    int DefaultFont::getStringIndexAt(std::string const & text, int x) const
    {
        int const glyphWidth = getWidth(" ");
        if (x > static_cast<int>(text.size()) * glyphWidth) {
            return text.size();
        }

        return x / glyphWidth;
    }
} // namespace fcn
