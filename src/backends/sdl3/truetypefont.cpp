// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2026 Fifengine contributors

// Corresponding header include
#include "fifechan/backends/sdl3/truetypefont.hpp"

// Standard library includes
#include <memory>
#include <string>

// Third-party library includes
#include <SDL3/SDL.h>

// Project headers (subdirs before local)
#include "fifechan/exception.hpp"
#include "fifechan/graphics.hpp"

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

    int TrueTypeFont::getWidth(std::string_view text) const
    {
        int w = 0;
        int h = 0;
        // Use UTF-8 aware measurement to handle multi-byte glyphs (emoji)
        TTF_GetStringSize(mFont, text.data(), text.size(), &w, &h);

        return w;
    }

    int TrueTypeFont::getHeight() const
    {
        return TTF_GetFontHeight(mFont) + mRowSpacing;
    }

    auto TrueTypeFont::renderToSurface(std::string_view text) const -> std::unique_ptr<SDL_Surface, SDL_SurfaceDeleter>
    {
        if (text.empty()) {
            return std::unique_ptr<SDL_Surface, SDL_SurfaceDeleter>(nullptr, SDL_SurfaceDeleter{});
        }

        // Render in white; drawSurface() will apply colour-modulation
        // using the Graphics colour, giving the desired tint.
        SDL_Color sdlCol;
        sdlCol.r = 255;
        sdlCol.g = 255;
        sdlCol.b = 255;
        sdlCol.a = 255;

        SDL_Surface* surface = nullptr;
        if (mAntiAlias) {
            surface = TTF_RenderText_Blended(mFont, text.data(), text.size(), sdlCol);
        } else {
            surface = TTF_RenderText_Solid(mFont, text.data(), text.size(), sdlCol);
        }

        if (surface == nullptr) {
            throwException("TrueTypeFont::renderToSurface. " + std::string(SDL_GetError()));
        }

        return std::unique_ptr<SDL_Surface, SDL_SurfaceDeleter>(surface, SDL_SurfaceDeleter{});
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
