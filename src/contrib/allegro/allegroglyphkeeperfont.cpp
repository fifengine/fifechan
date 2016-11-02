/***************************************************************************
 *   Copyright (c) 2016 by the fifechan team                               *
 *   https://github.com/fifengine/fifechan                                 *
 *   This file is part of fifechan.                                        *
 *                                                                         *
 *   fifechan is free software; you can redistribute it and/or             *
 *   modify it under the terms of the GNU Lesser General Public            *
 *   License as published by the Free Software Foundation; either          *
 *   version 2.1 of the License, or (at your option) any later version.    *
 *                                                                         *
 *   This library is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU     *
 *   Lesser General Public License for more details.                       *
 *                                                                         *
 *   You should have received a copy of the GNU Lesser General Public      *
 *   License along with this library; if not, write to the                 *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA          *
 ***************************************************************************/

/*      _______   __   __   __   ______   __   __   _______   __   __
 *     / _____/\ / /\ / /\ / /\ / ____/\ / /\ / /\ / ___  /\ /  |\/ /\
 *    / /\____\// / // / // / // /\___\// /_// / // /\_/ / // , |/ / /
 *   / / /__   / / // / // / // / /    / ___  / // ___  / // /| ' / /
 *  / /_// /\ / /_// / // / // /_/_   / / // / // /\_/ / // / |  / /
 * /______/ //______/ //_/ //_____/\ /_/ //_/ //_/ //_/ //_/ /|_/ /
 * \______\/ \______\/ \_\/ \_____\/ \_\/ \_\/ \_\/ \_\/ \_\/ \_\/
 *
 * Copyright (c) 2004 - 2008 Olof Naess�n and Per Larsson
 *
 *
 * Per Larsson a.k.a finalman
 * Olof Naess�n a.k.a jansem/yakslem
 *
 * Visit: http://guichan.sourceforge.net
 *
 * License: (BSD)
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name of Guichan nor the names of its contributors may
 *    be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 * TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 * For comments regarding functions please see the header file.
 */

#include "fifechan/contrib/allegro/allegroglyphkeeperfont.hpp"

#include "fifechan/allegro.hpp"
#include "fifechan/exception.hpp"
#include "fifechan/graphics.hpp"

#include <glyph.h>

namespace fcn
{
    namespace contrib
    {
        void AllegroGlyphKeeperFont::load(const std::string& filename, int w, int h)
        {
            mKeeper = gk_create_keeper(0,0);

            if (mKeeper == NULL)
            {
                throw FCN_EXCEPTION("Can't create keeper.");
            }

            mFace = gk_load_face_from_file(filename.c_str(), 0);

            if (mFace == NULL)
            {
                throw FCN_EXCEPTION("Can't load font from file.");
            }

            mRend = gk_create_renderer(mFace,mKeeper);
        
            if (mRend == NULL)
            {
                throw FCN_EXCEPTION("Can't create renderer.");
            }

            gk_rend_set_hinting_off(mRend);
            gk_rend_set_size_pixels(mRend, w, h);
            gk_rend_set_text_color_rgb(mRend, 0, 0, 0);
        }

        AllegroGlyphKeeperFont::AllegroGlyphKeeperFont(const std::string& filename, int size)
                : mFace(0), 
                  mRend(0), 
                  mKeeper(0)
        {
            load(filename, size, size);
        }

        AllegroGlyphKeeperFont::AllegroGlyphKeeperFont(const std::string& filename, int w, int h)
                : mFace(0), 
                  mRend(0), 
                  mKeeper(0)
        {
            load(filename, w, h);
        }

        AllegroGlyphKeeperFont::~AllegroGlyphKeeperFont()
        {
            if (mRend != NULL)
            {
                gk_done_renderer(mRend);
            }

            mRend = NULL;

            if (mFace != NULL)
            {
                gk_unload_face(mFace);
            }

            mFace = NULL;

            if (mKeeper != NULL)
            {
                gk_done_keeper(mKeeper);
            }

            mKeeper = NULL;
        }

        int AllegroGlyphKeeperFont::getWidth(const std::string& text) const
        {
            return gk_text_width_utf8(mRend, text.c_str());
        }

        int AllegroGlyphKeeperFont::getHeight() const
        {
            return gk_rend_height_pixels(mRend);
        }

        void AllegroGlyphKeeperFont::drawString(fcn::Graphics* graphics, const std::string& text, int x, int y)
        {
            fcn::AllegroGraphics* const allegroGraphics = dynamic_cast<fcn::AllegroGraphics*>(graphics);

            if (!allegroGraphics)
            {
                throw FCN_EXCEPTION("Graphics is not of type AllegroGraphics");
            }

            BITMAP* const target = allegroGraphics->getTarget();
        
            gk_rend_set_text_color_combined(mRend, allegroGraphics->getAllegroColor());
            gk_rend_set_text_alpha(mRend, allegroGraphics->getColor().a);
        
            const fcn::ClipRectangle& rec = graphics->getCurrentClipArea();
        
            gk_render_line_utf8(target,
                                mRend, 
                                text.c_str(), 
                                x + rec.xOffset, 
                                y + rec.yOffset + gk_rend_ascender_pixels(mRend));
        }
    }
}




