/*      _______   __   __   __   ______   __   __   _______   __   __
 *     / _____/\ / /\ / /\ / /\ / ____/\ / /\ / /\ / ___  /\ /  |\/ /\
 *    / /\____\// / // / // / // /\___\// /_// / // /\_/ / // , |/ / /
 *   / / /__   / / // / // / // / /    / ___  / // ___  / // /| ' / /
 *  / /_// /\ / /_// / // / // /_/_   / / // / // /\_/ / // / |  / /
 * /______/ //______/ //_/ //_____/\ /_/ //_/ //_/ //_/ //_/ /|_/ /
 * \______\/ \______\/ \_\/ \_____\/ \_\/ \_\/ \_\/ \_\/ \_\/ \_\/
 *
 * Copyright (c) 2004, 2005, 2006, 2007 Olof Naessén and Per Larsson
 *
 *                                                         Js_./
 * Per Larsson a.k.a finalman                          _RqZ{a<^_aa
 * Olof Naessén a.k.a jansem/yakslem                _asww7!uY`>  )\a//
 *                                                 _Qhm`] _f "'c  1!5m
 * Visit: http://guichan.darkbits.org             )Qk<P ` _: :+' .'  "{[
 *                                               .)j(] .d_/ '-(  P .   S
 * License: (BSD)                                <Td/Z <fP"5(\"??"\a.  .L
 * Redistribution and use in source and          _dV>ws?a-?'      ._/L  #'
 * binary forms, with or without                 )4d[#7r, .   '     )d`)[
 * modification, are permitted provided         _Q-5'5W..j/?'   -?!\)cam'
 * that the following conditions are met:       j<<WP+k/);.        _W=j f
 * 1. Redistributions of source code must       .$%w\/]Q  . ."'  .  mj$
 *    retain the above copyright notice,        ]E.pYY(Q]>.   a     J@\
 *    this list of conditions and the           j(]1u<sE"L,. .   ./^ ]{a
 *    following disclaimer.                     4'_uomm\.  )L);-4     (3=
 * 2. Redistributions in binary form must        )_]X{Z('a_"a7'<a"a,  ]"[
 *    reproduce the above copyright notice,       #}<]m7`Za??4,P-"'7. ).m
 *    this list of conditions and the            ]d2e)Q(<Q(  ?94   b-  LQ/
 *    following disclaimer in the                <B!</]C)d_, '(<' .f. =C+m
 *    documentation and/or other materials      .Z!=J ]e []('-4f _ ) -.)m]'
 *    provided with the distribution.          .w[5]' _[ /.)_-"+?   _/ <W"
 * 3. Neither the name of Guichan nor the      :$we` _! + _/ .        j?
 *    names of its contributors may be used     =3)= _f  (_yQmWW$#(    "
 *    to endorse or promote products derived     -   W,  sQQQQmZQ#Wwa]..
 *    from this software without specific        (js, \[QQW$QWW#?!V"".
 *    prior written permission.                    ]y:.<\..          .
 *                                                 -]n w/ '         [.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT       )/ )/           !
 * HOLDERS AND CONTRIBUTORS "AS IS" AND ANY         <  (; sac    ,    '
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING,               ]^ .-  %
 * BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF            c <   r
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR            aga<  <La
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE          5%  )P'-3L
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR        _bQf` y`..)a
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,          ,J?4P'.P"_(\?d'.,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES               _Pa,)!f/<[]/  ?"
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT      _2-..:. .r+_,.. .
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,     ?a.<%"'  " -'.a_ _,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION)                     ^
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
 * IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 * For comments regarding functions please see the header file. 
 */

#include "oglftfont.hpp"

#include "guichan/exception.hpp"
#include "guichan/opengl/openglgraphics.hpp"

namespace gcn
{

    OGLFTFont::OGLFTFont (const std::string& filename, int size)
    {
        mRowSpacing = 0;
        mFilename = filename;
        mFont = NULL;
        mSize = size;

        mFont = new OGLFT::TranslucentTexture(filename.c_str(), size, 72);

        if(mFont == NULL || !mFont->isValid())
        {
            throw GCN_EXCEPTION("Invalid True Type Font.");
        }

        glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );

        mFontColor = gcn::Color(0, 0, 0, 0);
    }

    OGLFTFont::~OGLFTFont()
    {
        delete mFont;
    }

    int OGLFTFont::getWidth(const std::string& text) const
    {
        OGLFT::BBox bbox = mFont->measure(text.c_str());

        return (int)bbox.x_max_ + (int)bbox.x_min_;
    }

    int OGLFTFont::getHeight() const
    {
        return mSize - 1 + mRowSpacing;
    }

    void OGLFTFont::setRowSpacing(int spacing)
    {
        mRowSpacing = spacing;
    }

    int OGLFTFont::getRowSpacing()
    {
        return mRowSpacing;
    }

    gcn::Color OGLFTFont::getColor()
    {
        return mFontColor;
    }

    void OGLFTFont::setColor(gcn::Color color)
    {
        mFontColor = color;
    }

    void OGLFTFont::drawString(Graphics* graphics, const std::string& text, int x, int y)
    {
        if (text == "")
        {
            return;
        }

        gcn::OpenGLGraphics* glGraphics = (gcn::OpenGLGraphics*) graphics;

        if(glGraphics == NULL)
        {
            throw GCN_EXCEPTION("Graphics object not an OpenGL graphics object!");
        }

        const ClipRectangle& top = glGraphics->getCurrentClipArea();

        mFont->setForegroundColor(mFontColor.r/255, mFontColor.g/255, mFontColor.b/255);

        glPushMatrix();
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glTranslated(x + top.xOffset, y + top.yOffset + (mSize/2)+5, 0.);
		glRotatef(180., 1., 0., 0.);

		mFont->draw(text.c_str());

		glDisable(GL_BLEND);
		glDisable(GL_TEXTURE_2D);
        glPopMatrix();
    }
}
