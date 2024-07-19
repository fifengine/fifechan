// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#include "fifechan/opengl/opengloglftfont.hpp"

#include "fifechan/exception.hpp"
#include "fifechan/opengl/openglgraphics.hpp"
#include <math.h>

namespace fcn
{
        OGLFTFont::OGLFTFont(std::string const & filename, int size)
        {
            mRowSpacing = 0;
            mFilename   = filename;
            mFont       = NULL;
            mSize       = size;

            mFont = new OGLFT::TranslucentTexture(filename.c_str(), size, 72);

            if (mFont == NULL || !mFont->isValid()) {
                fcn::throwException(
                    ("Invalid True Type Font."), static_cast<char const *>(__FUNCTION__), __FILE__, __LINE__);
            }

            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        }

        OGLFTFont::~OGLFTFont()
        {
            delete mFont;
        }

        int OGLFTFont::getWidth(std::string const & text) const
        {
            OGLFT::BBox bbox = mFont->measure(text.c_str());

            return (int)ceil(bbox.x_max_) + (int)ceil(bbox.x_min_);
        }

        int OGLFTFont::getHeight() const
        {
            return mSize + mRowSpacing;
        }

        void OGLFTFont::setRowSpacing(int spacing)
        {
            mRowSpacing = spacing;
        }

        int OGLFTFont::getRowSpacing()
        {
            return mRowSpacing;
        }

        void OGLFTFont::drawString(fcn::Graphics* graphics, std::string const & text, int x, int y)
        {
            if (text == "") {
                return;
            }

            fcn::OpenGLGraphics* glGraphics = dynamic_cast<fcn::OpenGLGraphics*>(graphics);

            if (glGraphics == NULL) {
                fcn::throwException(
                    ("Graphics object not an OpenGL graphics object!"),
                    static_cast<char const *>(__FUNCTION__),
                    __FILE__,
                    __LINE__);
            }

            fcn::ClipRectangle const & top = glGraphics->getCurrentClipArea();

            Color col = glGraphics->getColor();
            mFont->setForegroundColor(col.r / 255, col.g / 255, col.b / 255);

            glPushMatrix();
            glEnable(GL_TEXTURE_2D);
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            glTranslated(x + top.xOffset, y + top.yOffset + (mSize / 2) + 5, 0.);
            glRotatef(180., 1., 0., 0.);

            mFont->draw(text.c_str());

            glDisable(GL_BLEND);
            glDisable(GL_TEXTURE_2D);
            glPopMatrix();
        }
} // namespace fcn
