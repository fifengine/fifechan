// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2026 Fifengine contributors

#ifndef TESTS_UNIT_SRC_MOCK_FONT_HPP_
#define TESTS_UNIT_SRC_MOCK_FONT_HPP_

#include <SDL3/SDL.h>

#include <memory>
#include <string>

#include "fifechan/font.hpp"
#include "fifechan/graphics.hpp"

// Shared mock font used by multiple unit tests to avoid ODR violations.
class MockFont : public fcn::Font
{
    public:
        mutable bool renderToSurfaceCalled = false;
        explicit MockFont(int charWidth = 10, int height = 16) : m_charWidth(charWidth), m_height(height)
        {
        }

        int getWidth(std::string_view text) const override
        {
            return static_cast<int>(text.size()) * m_charWidth;
        }

        int getHeight() const override
        {
            return m_height;
        }

        auto renderToSurface(std::string_view text) const -> std::unique_ptr<SDL_Surface, SDL_SurfaceDeleter> override
        {
            renderToSurfaceCalled = true;
            if (text.empty()) {
                return std::unique_ptr<SDL_Surface, SDL_SurfaceDeleter>(nullptr, SDL_SurfaceDeleter{});
            }
            int const w          = getWidth(text);
            int const h          = getHeight();
            SDL_Surface* surface = SDL_CreateSurface(w, h, SDL_PIXELFORMAT_RGBA8888);
            SDL_FillSurfaceRect(surface, nullptr, 0);
            return std::unique_ptr<SDL_Surface, SDL_SurfaceDeleter>(surface, SDL_SurfaceDeleter{});
        }

    private:
        int m_charWidth;
        int m_height;
};

#endif // TESTS_UNIT_SRC_MOCK_FONT_HPP_
