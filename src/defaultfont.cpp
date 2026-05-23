// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2026 Fifengine contributors

// Corresponding header include
#include "fifechan/defaultfont.hpp"

// Standard library includes
#include <memory>
#include <string>

// Project headers (subdirs before local)
#include "fifechan/exception.hpp"

// Third-party library includes
#include <SDL3/SDL.h>

namespace fcn
{
    int DefaultFont::getHeight() const
    {
        return 8;
    }

    int DefaultFont::getWidth(std::string_view text) const
    {
        return 8 * static_cast<int>(text.size());
    }

    int DefaultFont::getStringIndexAt(std::string_view text, int x) const
    {
        int const glyphWidth = getWidth(" ");
        if (x > static_cast<int>(text.size()) * glyphWidth) {
            return static_cast<int>(text.size());
        }

        return x / glyphWidth;
    }

    auto DefaultFont::renderToSurface(std::string_view text) const -> std::unique_ptr<SDL_Surface, SDL_SurfaceDeleter>
    {
        if (text.empty()) {
            return std::unique_ptr<SDL_Surface, SDL_SurfaceDeleter>(nullptr, SDL_SurfaceDeleter{});
        }

        int const totalWidth = getWidth(text);
        int const height     = getHeight();

        SDL_Surface* surface = SDL_CreateSurface(totalWidth, height, SDL_PIXELFORMAT_RGBA8888);
        if (surface == nullptr) {
            throwException(std::string("DefaultFont::renderToSurface – ") + SDL_GetError());
        }

        // Fill the entire surface with white pixels so that drawSurface
        // colour-modulation will tint them to the desired colour.
        SDL_FillSurfaceRect(
            surface, nullptr, SDL_MapRGBA(SDL_GetPixelFormatDetails(surface->format), nullptr, 255, 255, 255, 255));

        return std::unique_ptr<SDL_Surface, SDL_SurfaceDeleter>(surface, SDL_SurfaceDeleter{});
    }
} // namespace fcn
