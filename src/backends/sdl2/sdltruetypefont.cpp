// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#include "fifechan/backends/sdl2/sdltruetypefont.hpp"

#include <string>

#include "fifechan/backends/sdl2/sdl2graphics.hpp"
#include "fifechan/exception.hpp"
#include "fifechan/graphics.hpp"
#include "fifechan/image.hpp"

namespace fcn
{
    SDLTrueTypeFont::SDLTrueTypeFont(std::string const & filename, int size) :
        mRowSpacing(0),
        mGlyphSpacing(0),
        mAntiAlias(true),
        mFilename(filename),
        mFont(TTF_OpenFont(filename.c_str(), size))
    {

        if (mFont == nullptr) {
            throwException("SDLTrueTypeFont::SDLTrueTypeFont. " + std::string(TTF_GetError()));
        }
    }

    SDLTrueTypeFont::~SDLTrueTypeFont()
    {
        TTF_CloseFont(mFont);
    }

    int SDLTrueTypeFont::getWidth(std::string const & text) const
    {
        int w = 0;
        int h = 0;
        TTF_SizeText(mFont, text.c_str(), &w, &h);

        return w;
    }

    int SDLTrueTypeFont::getHeight() const
    {
        return TTF_FontHeight(mFont) + mRowSpacing;
    }

    void SDLTrueTypeFont::drawString(fcn::Graphics* graphics, std::string const & text, int x, int y)
    {
        if (text.empty()) {
            return;
        }

        auto* sdlGraphics  = dynamic_cast<fcn::SDLGraphics*>(graphics);
        auto* sdl2Graphics = dynamic_cast<fcn::SDL2Graphics*>(graphics);

        if (sdlGraphics == nullptr && sdl2Graphics == nullptr) {
            throwException("SDLTrueTypeFont::drawString. Graphics object not an SDL/SDL2 graphics object!");
            return;
        }

        // This is needed for drawing the Glyph in the middle if we have spacing
        int const yoffset = getRowSpacing() / 2;

        Color const col = graphics->getColor();

        SDL_Color sdlCol;
        sdlCol.b = col.b;
        sdlCol.r = col.r;
        sdlCol.g = col.g;
        sdlCol.a = col.a;

        SDL_Surface* textSurface = nullptr;
        if (mAntiAlias) {
            textSurface = TTF_RenderText_Blended(mFont, text.c_str(), sdlCol);
        } else {
            textSurface = TTF_RenderText_Solid(mFont, text.c_str(), sdlCol);
        }

        if (textSurface == nullptr) {
            throwException("SDLTrueTypeFont::drawString. " + std::string(TTF_GetError()));
            return;
        }

        SDL_Rect dst;
        SDL_Rect src;
        dst.x = x;
        dst.y = y + yoffset;
        src.w = textSurface->w;
        src.h = textSurface->h;
        src.x = 0;
        src.y = 0;

        if (sdlGraphics != nullptr) {
            sdlGraphics->drawSDLSurface(textSurface, src, dst);
        } else {
            sdl2Graphics->drawSDLSurface(textSurface, src, dst);
        }
        SDL_FreeSurface(textSurface);
    }

    void SDLTrueTypeFont::setRowSpacing(int spacing)
    {
        mRowSpacing = spacing;
    }

    int SDLTrueTypeFont::getRowSpacing()
    {
        return mRowSpacing;
    }

    void SDLTrueTypeFont::setGlyphSpacing(int spacing)
    {
        mGlyphSpacing = spacing;
    }

    int SDLTrueTypeFont::getGlyphSpacing()
    {
        return mGlyphSpacing;
    }

    void SDLTrueTypeFont::setAntiAlias(bool antiAlias)
    {
        mAntiAlias = antiAlias;
    }

    bool SDLTrueTypeFont::isAntiAlias()
    {
        return mAntiAlias;
    }
} // namespace fcn
