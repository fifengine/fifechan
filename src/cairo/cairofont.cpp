/*      _______   __   __   __   ______   __   __   _______   __   __
 *     / _____/\ / /\ / /\ / /\ / ____/\ / /\ / /\ / ___  /\ /  |\/ /\
 *    / /\____\// / // / // / // /\___\// /_// / // /\_/ / // , |/ / /
 *   / / /__   / / // / // / // / /    / ___  / // ___  / // /| ' / /
 *  / /_// /\ / /_// / // / // /_/_   / / // / // /\_/ / // / |  / /
 * /______/ //______/ //_/ //_____/\ /_/ //_/ //_/ //_/ //_/ /|_/ /
 * \______\/ \______\/ \_\/ \_____\/ \_\/ \_\/ \_\/ \_\/ \_\/ \_\/
 *
 * Copyright (c) 2008 Mehdi Abbad a.k.a slyf0x
 *
 *
 * Per Larsson a.k.a finalman
 * Olof Naessén a.k.a jansem/yakslem
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

#include "guichan/cairo/cairofont.hpp"
#include "guichan/cairo/cairographics.hpp"
#include "guichan/exception.hpp"

namespace gcn
{
    CairoFont::CairoFont(cairo_scaled_font_t* fontface)
    {
        mFontFace=fontface;
        cairo_scaled_font_reference(mFontFace);
        cairo_scaled_font_extents(mFontFace,&mFontExtents);
        SetFontColor(Color(0,0,0,255));
    }


    /*void CairoFont::SetFontFace(cairo_font_face_t* fontface)
    {

    }*/

    void CairoFont::drawString(gcn::Graphics* graphics, const std::string& text, int x, int y)
    {
        CairoGraphics* gr=dynamic_cast<CairoGraphics*>(graphics);
        if (gr==NULL)
        {
            GCN_EXCEPTION("The graphics object passed as parameter is not of type CairoGraphics");
        }
        cairo_t* target=gr->GetContext();
        ClipRectangle r=gr->getCurrentClipArea();
        cairo_set_scaled_font(target,mFontFace);
        cairo_move_to(target,r.xOffset+x,mFontExtents.ascent+r.yOffset+y);
        cairo_set_source_rgba(target,mColorR,mColorG,mColorB,mColorA);
        cairo_show_text(target,text.c_str());
    }

    int CairoFont::getHeight() const
    {
        return (int)mFontExtents.height;
    }

    int CairoFont::getWidth(const std::string& text) const
    {
        cairo_text_extents_t textext;
        cairo_scaled_font_text_extents(mFontFace,text.c_str(),&textext);
        return (int)textext.width;
    }


    CairoFont::~CairoFont()
    {
        cairo_scaled_font_destroy(mFontFace);
    }

    void CairoFont::SetFontColor(const Color& color)
    {
        mColor = color;
        mColorR= mColor.r/255.0;
        mColorG= mColor.g/255.0;
        mColorB= mColor.b/255.0;
        mColorA= mColor.a/255.0;
    }
}
