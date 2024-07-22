// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#include "fifechan/allegro/allegroglyphkeeperfont.hpp"

#include "fifechan/allegro.hpp"
#include "fifechan/exception.hpp"
#include "fifechan/graphics.hpp"
#include <glyph.h>

namespace fcn
{
    void AllegroGlyphKeeperFont::load(std::string const & filename, int w, int h)
    {
        mKeeper = gk_create_keeper(0, 0);

        if (mKeeper == NULL) {
            fcn::throwException(("Can't create keeper."), static_cast<char const *>(__FUNCTION__), __FILE__, __LINE__);
        }

        mFace = gk_load_face_from_file(filename.c_str(), 0);

        if (mFace == NULL) {
            fcn::throwException(
                ("Can't load font from file."), static_cast<char const *>(__FUNCTION__), __FILE__, __LINE__);
        }

        mRend = gk_create_renderer(mFace, mKeeper);

        if (mRend == NULL) {
            fcn::throwException(
                ("Can't create renderer."), static_cast<char const *>(__FUNCTION__), __FILE__, __LINE__);
        }

        gk_rend_set_hinting_off(mRend);
        gk_rend_set_size_pixels(mRend, w, h);
        gk_rend_set_text_color_rgb(mRend, 0, 0, 0);
    }

    AllegroGlyphKeeperFont::AllegroGlyphKeeperFont(std::string const & filename, int size) :
        mFace(0), mRend(0), mKeeper(0)
    {
        load(filename, size, size);
    }

    AllegroGlyphKeeperFont::AllegroGlyphKeeperFont(std::string const & filename, int w, int h) :
        mFace(0), mRend(0), mKeeper(0)
    {
        load(filename, w, h);
    }

    AllegroGlyphKeeperFont::~AllegroGlyphKeeperFont()
    {
        if (mRend != NULL) {
            gk_done_renderer(mRend);
        }

        mRend = NULL;

        if (mFace != NULL) {
            gk_unload_face(mFace);
        }

        mFace = NULL;

        if (mKeeper != NULL) {
            gk_done_keeper(mKeeper);
        }

        mKeeper = NULL;
    }

    int AllegroGlyphKeeperFont::getWidth(std::string const & text) const
    {
        return gk_text_width_utf8(mRend, text.c_str());
    }

    int AllegroGlyphKeeperFont::getHeight() const
    {
        return gk_rend_height_pixels(mRend);
    }

    void AllegroGlyphKeeperFont::drawString(fcn::Graphics* graphics, std::string const & text, int x, int y)
    {
        fcn::AllegroGraphics* const allegroGraphics = dynamic_cast<fcn::AllegroGraphics*>(graphics);

        if (!allegroGraphics) {
            fcn::throwException(
                ("Graphics is not of type AllegroGraphics"),
                static_cast<char const *>(__FUNCTION__),
                __FILE__,
                __LINE__);
        }

        BITMAP* const target = allegroGraphics->getTarget();

        gk_rend_set_text_color_combined(mRend, allegroGraphics->getAllegroColor());
        gk_rend_set_text_alpha(mRend, allegroGraphics->getColor().a);

        fcn::ClipRectangle const & rec = graphics->getCurrentClipArea();

        gk_render_line_utf8(
            target, mRend, text.c_str(), x + rec.xOffset, y + rec.yOffset + gk_rend_ascender_pixels(mRend));
    }
} // namespace fcn
