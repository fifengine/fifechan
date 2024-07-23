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

    int DefaultFont::drawGlyph(Graphics* graphics, unsigned char /*glyph*/, int x, int y)
    {
        graphics->drawRectangle(x, y, 8, 8);

        return 8;
    }

    void DefaultFont::drawString(Graphics* graphics, std::string const & text, int x, int y)
    {
        for (int i = 0; i < text.size(); ++i) {
            drawGlyph(graphics, text.at(i), x, y);
            x += getWidth(text);
        }
    }

    int DefaultFont::getStringIndexAt(std::string const & text, int x) const
    {
        if (x > static_cast<int>(text.size()) * 8) {
            return text.size();
        }

        return x / 8;
    }
} // namespace fcn
