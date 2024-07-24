// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#include "fifechan/backends/hge/hgeimagefont.hpp"

#include "fifechan/color.hpp"
#include "fifechan/graphics.hpp"

namespace fcn
{
    HGEImageFont::HGEImageFont(std::string const & filename)
    {
        mHGEFont = new hgeFont(filename.c_str());
    }

    HGEImageFont::~HGEImageFont()
    {
        delete mHGEFont;
    }

    hgeFont* HGEImageFont::getFont() const
    {
        return mHGEFont;
    }

    int HGEImageFont::getWidth(std::string const & text) const
    {
        return static_cast<int>(mHGEFont->GetStringWidth(text.c_str())) + 1;
    }

    int HGEImageFont::getWidth(char character) const
    {
        char buf[2];

        snprintf(buf, "%c", character);

        return mHGEFont->GetStringWidth(buf);
    }

    int HGEImageFont::getHeight() const
    {
        return static_cast<int>(mHGEFont->GetHeight());
    }

    int HGEImageFont::getStringIndexAt(std::string const & text, int x) const
    {
        unsigned int i;
        int size = 0;

        for (i = 0; i < text.size(); ++i) {
            size += getWidth(text.at(i));

            if (size > x) {
                return i;
            }
        }

        return text.size();
    }

    void HGEImageFont::drawString(Graphics* graphics, std::string const & text, int x, int y)
    {
        ClipRectangle const & top = graphics->getCurrentClipArea();

        x += top.xOffset;
        y += top.yOffset;

        Color color = graphics->getColor();

        mHGEFont->SetColor(ARGB(color.a, color.r, color.g, color.b));
        mHGEFont->Render(x, y, HGETEXT_LEFT, text.c_str());
    }
} // namespace fcn
