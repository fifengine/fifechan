// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#include "fifechan/backends/sdl2/truetypefont.hpp"

#include <SDL2/SDL_render.h>

#include <string>

#include "fifechan/backends/sdl2/graphics.hpp"
#include "fifechan/exception.hpp"
#include "fifechan/graphics.hpp"
#include "fifechan/image.hpp"

namespace fcn::sdl2
{
    TrueTypeFont::TrueTypeFont(std::string const & filename, int size) :
        mRowSpacing(0),
        mGlyphSpacing(0),
        mAntiAlias(true),
        mFilename(filename),
        mFont(TTF_OpenFont(filename.c_str(), size))
    {

        if (mFont == nullptr) {
            throwException("TrueTypeFont::TrueTypeFont. " + std::string(TTF_GetError()));
        }
    }

    TrueTypeFont::~TrueTypeFont()
    {
        TTF_CloseFont(mFont);
    }

    int TrueTypeFont::getWidth(std::string const & text) const
    {
        int w = 0;
        int h = 0;
        TTF_SizeText(mFont, text.c_str(), &w, &h);

        return w;
    }

    int TrueTypeFont::getHeight() const
    {
        return TTF_FontHeight(mFont) + mRowSpacing;
    }

    void TrueTypeFont::drawString(fcn::Graphics* graphics, std::string const & text, int x, int y)
    {
        if (text.empty()) {
            return;
        }

        auto* sdlGraphics = dynamic_cast<fcn::sdl2::Graphics*>(graphics);

        if (sdlGraphics == nullptr) {
            throwException("TrueTypeFont::drawString. Graphics object must be fcn::sdl2::Graphics!");
            return;
        }

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
            throwException("TrueTypeFont::drawString. " + std::string(TTF_GetError()));
            return;
        }

        SDL_Renderer* renderer = sdlGraphics->getRenderTarget();
        SDL_Texture* texture   = SDL_CreateTextureFromSurface(renderer, textSurface);
        if (texture == nullptr) {
            SDL_FreeSurface(textSurface);
            throwException("TrueTypeFont::drawString. Failed to create texture: " + std::string(SDL_GetError()));
            return;
        }

        SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
#if SDL_VERSION_ATLEAST(2, 0, 12)
        SDL_SetTextureScaleMode(texture, SDL_ScaleModeNearest);
#endif

        SDL_Rect dst;
        SDL_Rect src;
        dst.x = x;
        dst.y = y + yoffset;
        src.w = textSurface->w;
        src.h = textSurface->h;
        src.x = 0;
        src.y = 0;

        sdlGraphics->drawSDLTexture(texture, src, dst);

        SDL_DestroyTexture(texture);
        SDL_FreeSurface(textSurface);
    }

    void TrueTypeFont::setRowSpacing(int spacing)
    {
        mRowSpacing = spacing;
    }

    int TrueTypeFont::getRowSpacing()
    {
        return mRowSpacing;
    }

    void TrueTypeFont::setGlyphSpacing(int spacing)
    {
        mGlyphSpacing = spacing;
    }

    int TrueTypeFont::getGlyphSpacing()
    {
        return mGlyphSpacing;
    }

    void TrueTypeFont::setAntiAlias(bool antiAlias)
    {
        mAntiAlias = antiAlias;
    }

    bool TrueTypeFont::isAntiAlias()
    {
        return mAntiAlias;
    }
} // namespace fcn::sdl2
