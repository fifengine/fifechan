// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#include "fifechan/allegro/allegrofont.hpp"
#include "fifechan/allegro/allegrographics.hpp"

#include "fifechan/exception.hpp"

namespace fcn
{

    AllegroFont::AllegroFont(FONT* font) : mAutoFree(false), mAllegroFont(font)

    {
        if (font == NULL) {
            fcn::throwException(
                "Allegro font is not usable. Have you forgotten to load it?",
                static_cast<char const *>(__FUNCTION__),
                __FILE__,
                __LINE__);
        }
    }

    AllegroFont::AllegroFont(std::string const & filename) :
        mAutoFree(true), mAllegroFont(load_font(filename.c_str(), 0, 0))

    {
        if (mAllegroFont == NULL) {
            fcn::throwException(
                "Unable to load Allegro font from file.", static_cast<char const *>(__FUNCTION__), __FILE__, __LINE__);
        }
    }

    AllegroFont::~AllegroFont()
    {
        if (mAutoFree) {
            if (mAllegroFont != NULL) {
                destroy_font(mAllegroFont);
            }

            mAllegroFont = NULL;
        }
    }

    int AllegroFont::getWidth(std::string const & text) const
    {
        return text_length(mAllegroFont, text.c_str());
    }

    int AllegroFont::getHeight() const
    {
        return text_height(mAllegroFont);
    }

    void AllegroFont::drawString(fcn::Graphics* graphics, std::string const & text, int x, int y)
    {
        fcn::AllegroGraphics* const allegroGraphics = dynamic_cast<fcn::AllegroGraphics*>(graphics);
        if (!allegroGraphics) {
            fcn::throwException(
                "Graphics is not of type AllegroGraphics", static_cast<char const *>(__FUNCTION__), __FILE__, __LINE__);
        }

        BITMAP* const db = allegroGraphics->getTarget();

        fcn::ClipRectangle const & rec = graphics->getCurrentClipArea();

        textout_ex(db, mAllegroFont, text.c_str(), x + rec.x, y + rec.y, allegroGraphics->getAllegroColor(), -1);
    }
} // namespace fcn
