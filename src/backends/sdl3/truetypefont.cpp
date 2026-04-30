// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2026 Fifengine contributors

// Corresponding header include
#include "fifechan/backends/sdl3/truetypefont.hpp"

// Standard library includes
#include <string>

// Third-party library includes
#include <SDL3/SDL.h>

// Project headers (subdirs before local)
#include "fifechan/backends/sdl3/graphics.hpp"
#include "fifechan/exception.hpp"
#include "fifechan/graphics.hpp"
#include "fifechan/image.hpp"

namespace fcn::sdl3
{
    TrueTypeFont::TrueTypeFont(std::string const & filename, int size) :
        mRowSpacing(0),
        mGlyphSpacing(0),
        mAntiAlias(true),
        mFilename(filename),
        mFont(TTF_OpenFont(filename.c_str(), static_cast<float>(size)))
    {

        if (mFont == nullptr) {
            throwException("TrueTypeFont::TrueTypeFont. " + std::string(SDL_GetError()));
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
        // Use UTF-8 aware measurement to handle multi-byte glyphs (emoji)
        TTF_GetStringSize(mFont, text.c_str(), text.length(), &w, &h);

        return w;
    }

    int TrueTypeFont::getHeight() const
    {
        return TTF_GetFontHeight(mFont) + mRowSpacing;
    }

    void TrueTypeFont::drawString(fcn::Graphics* graphics, std::string const & text, int x, int y)
    {
        if (text.empty()) {
            return;
        }

        auto* sdlGraphics = dynamic_cast<fcn::sdl3::Graphics*>(graphics);

        if (sdlGraphics == nullptr) {
            throwException("TrueTypeFont::drawString. Graphics object must be fcn::sdl3::Graphics!");
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
        // Use UTF-8 aware rendering to avoid mangling multi-byte sequences
        if (mAntiAlias) {
            textSurface = TTF_RenderText_Blended(mFont, text.c_str(), text.length(), sdlCol);
        } else {
            textSurface = TTF_RenderText_Solid(mFont, text.c_str(), text.length(), sdlCol);
        }

        if (textSurface == nullptr) {
            throwException("TrueTypeFont::drawString. " + std::string(SDL_GetError()));
            return;
        }

        SDL_Renderer* renderer = sdlGraphics->getRenderTarget();
        SDL_Texture* texture   = SDL_CreateTextureFromSurface(renderer, textSurface);
        if (texture == nullptr) {
            SDL_DestroySurface(textSurface);
            throwException("TrueTypeFont::drawString. Failed to create texture: " + std::string(SDL_GetError()));
            return;
        }

        SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
        SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_NEAREST);

        SDL_FRect dst;
        SDL_FRect src;
        dst.x = static_cast<float>(x);
        dst.y = static_cast<float>(y + yoffset);
        src.w = static_cast<float>(textSurface->w);
        src.h = static_cast<float>(textSurface->h);
        src.x = 0.0F;
        src.y = 0.0F;

        sdlGraphics->drawSDLTexture(texture, src, dst);

        SDL_DestroyTexture(texture);
        SDL_DestroySurface(textSurface);
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
} // namespace fcn::sdl3
