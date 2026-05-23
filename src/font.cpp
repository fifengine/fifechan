// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2026 Fifengine contributors

// Corresponding header include
#include "fifechan/font.hpp"

// Standard library includes
#include <cassert>
#include <string>

// Platform config include
#include "fifechan/platform.hpp"

// Third-party library includes
#include <SDL3/SDL.h>

// Project headers (subdirs before local)
#include "fifechan/graphics.hpp"

namespace fcn
{
    void Font::drawString(Graphics* graphics, std::string_view text, int x, int y) const
    {
        auto surface = renderToSurface(text);
        if (surface) {
            graphics->drawSurface(surface.get(), x, y);
        }
    }

    int Font::getStringIndexAt(std::string_view text, int x) const
    {
        assert("x must be non-negative" && x >= 0);

        for (std::size_t i = 0; i < text.size(); ++i) {
            if (getWidth(text.substr(0, i)) > x) {
                return static_cast<int>(i);
            }
        }
        return static_cast<int>(text.size());
    }
} // namespace fcn
