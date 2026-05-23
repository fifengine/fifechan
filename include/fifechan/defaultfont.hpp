// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2026 Fifengine contributors

#ifndef INCLUDE_FIFECHAN_DEFAULTFONT_HPP_
#define INCLUDE_FIFECHAN_DEFAULTFONT_HPP_

// Standard library includes
#include <memory>
#include <string>

// Project headers (subdirs before local)
#include "fifechan/font.hpp"

namespace fcn
{
    /**
     * A basic font implementation capable only of drawing rectangles (placeholder).
     *
     * This font is used as a default font when no other font is specified.
     *
     * @ingroup fonts
     */
    class FIFEGUI_API DefaultFont : public Font
    {
        public:
            // Inherited from Font

            int getWidth(std::string_view text) const override;

            int getHeight() const override;

            int getStringIndexAt(std::string_view text, int x) const override;

            auto renderToSurface(std::string_view text) const
                -> std::unique_ptr<SDL_Surface, SDL_SurfaceDeleter> override;
    };
} // namespace fcn

#endif // INCLUDE_FIFECHAN_DEFAULTFONT_HPP_
