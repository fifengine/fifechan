// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#include "fifechan/cairo/cairofont.hpp"
#include "fifechan/cairo/cairographics.hpp"
#include "fifechan/exception.hpp"

namespace fcn
{
    CairoFont::CairoFont(cairo_scaled_font_t* fontface)
    {
        mFontFace = fontface;
        cairo_scaled_font_reference(mFontFace);
        cairo_scaled_font_extents(mFontFace, &mFontExtents);
        SetFontColor(Color(0, 0, 0, 255));
    }

    /*void CairoFont::SetFontFace(cairo_font_face_t* fontface)
    {

    }*/

    void CairoFont::drawString(fcn::Graphics* graphics, std::string const & text, int x, int y)
    {
        CairoGraphics* gr = dynamic_cast<CairoGraphics*>(graphics);
        if (gr == NULL) {
            fcn::throwException(
                "The graphics object passed as parameter is not of type CairoGraphics",
                static_cast<char const *>(__FUNCTION__),
                __FILE__,
                __LINE__);
        }
        cairo_t* target = gr->GetContext();
        ClipRectangle r = gr->getCurrentClipArea();
        cairo_set_scaled_font(target, mFontFace);
        cairo_move_to(target, r.xOffset + x, mFontExtents.ascent + r.yOffset + y);
        cairo_set_source_rgba(target, mColorR, mColorG, mColorB, mColorA);
        cairo_show_text(target, text.c_str());
    }

    int CairoFont::getHeight() const
    {
        return (int)mFontExtents.height;
    }

    int CairoFont::getWidth(std::string const & text) const
    {
        cairo_text_extents_t textext;
        cairo_scaled_font_text_extents(mFontFace, text.c_str(), &textext);
        return (int)textext.width;
    }

    CairoFont::~CairoFont()
    {
        cairo_scaled_font_destroy(mFontFace);
    }

    void CairoFont::SetFontColor(Color const & color)
    {
        mColor  = color;
        mColorR = mColor.r / 255.0;
        mColorG = mColor.g / 255.0;
        mColorB = mColor.b / 255.0;
        mColorA = mColor.a / 255.0;
    }
} // namespace fcn
