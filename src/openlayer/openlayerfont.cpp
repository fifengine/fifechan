// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#include "fifechan/openlayer/openlayerfont.hpp"
#include "fifechan/openlayer/openlayergraphics.hpp"

#include "fifechan/exception.hpp"

namespace fcn
{
    OpenLayerTTFont::OpenLayerTTFont(std::string const & filename, int size) :
        mTextRenderer(filename.c_str(), size, size)
    {
        if (!mTextRenderer.IsValid()) {
            fcn::throwException(("Unable to load font."), static_cast<char const *>(__FUNCTION__), __FILE__, __LINE__);
        }
    }

    OpenLayerTTFont::OpenLayerTTFont(std::string const & filename, int width, int height) :
        mTextRenderer(filename.c_str(), width, height)
    {
        if (!mTextRenderer.IsValid()) {
            fcn::throwException(("Unable to load font."), static_cast<char const *>(__FUNCTION__), __FILE__, __LINE__);
        }
    }

    OpenLayerTTFont::~OpenLayerTTFont() { }

    int OpenLayerTTFont::getWidth(std::string const & text) const
    {
        return mTextRenderer.Width(text);
    }

    int OpenLayerTTFont::getHeight() const
    {
        return mTextRenderer.FontTotalHeight();
    }

    void OpenLayerTTFont::drawString(Graphics* graphics, std::string const & text, int x, int y)
    {
        ClipRectangle const & rec = graphics->getCurrentClipArea();

        OpenLayerGraphics* const openLayerGraphics = dynamic_cast<fcn::OpenLayerGraphics*>(graphics);
        if (!openLayerGraphics) {
            fcn::throwException(
                ("Graphics is not of type OpenLayerGraphics"),
                static_cast<char const *>(__FUNCTION__),
                __FILE__,
                __LINE__);
        }

        mTextRenderer.SetColor(openLayerGraphics->getOpenLayerColor());
        mTextRenderer.Print(text, x + rec.xOffset, y + rec.yOffset + mTextRenderer.Height(text));
    }
} // namespace fcn
