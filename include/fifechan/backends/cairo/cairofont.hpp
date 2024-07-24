// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#ifndef INCLUDE_FIFECHAN_BACKENDS_CAIRO_CAIROFONT_HPP_
#define INCLUDE_FIFECHAN_BACKENDS_CAIRO_CAIROFONT_HPP_

#include <string>

#include "cairo.h"
#include "fifechan/color.hpp"
#include "fifechan/font.hpp"
#include "fifechan/platform.hpp"

namespace fcn
{
    /**
     * Cairo font implementation of Fonts used by Cairo.
     */
    class FIFEGUI_EXT_API CairoFont : public Font
    {
    public:
        /**
         * Constructor.
         * The new created object will hold a reference to the passed fontface.
         *
         * @param fontface a Cairo scaled font face created with one of the font backends.
         */
        explicit CairoFont(cairo_scaled_font_t* fontface);

        ~CairoFont();

        // void SetFontFace(cairo_font_face_t* fontface);

        void SetFontColor(Color const & color);

        // Inherited from Font

        virtual int getWidth(std::string const & text) const;

        virtual int getHeight() const;

        virtual void drawString(fcn::Graphics* graphics, std::string const & text, int x, int y);

    private:
        cairo_scaled_font_t* mFontFace;
        cairo_font_extents_t mFontExtents;
        Color mColor;
        double mColorR, mColorG, mColorB, mColorA;
    };
} // namespace fcn

#endif // INCLUDE_FIFECHAN_BACKENDS_CAIRO_CAIROFONT_HPP_
