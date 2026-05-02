// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2013 - 2026 Fifengine contributors

#ifndef INCLUDE_FIFECHAN_FONTLOADER_HPP
#define INCLUDE_FIFECHAN_FONTLOADER_HPP

// Standard library includes
#include <filesystem>
#include <memory>
#include <string>
#include <vector>

// Project headers (subdirs before local)
#include "fifechan/font.hpp"
#include "fifechan/graphics.hpp"

namespace fcn::font
{

    /**
     * Helper class for searching and loading fonts.
     *
     * This provides the same functionality as the previous free functions
     * but grouped inside an exported class to improve encapsulation and
     * allow stateful extensions in the future.
     */
    class FIFEGUI_API FontLoader
    {
        public:
            static std::vector<std::filesystem::path> getDefaultSearchPaths();

            static std::filesystem::path findFontFile(
                std::string const & fontName, std::vector<std::filesystem::path> const & searchPaths = {});

            static std::shared_ptr<Font> loadFont(
                Graphics& graphics,
                std::string const & fontName,
                int size,
                std::vector<std::filesystem::path> const & searchPaths = {});
    };

} // namespace fcn::font

#endif // INCLUDE_FIFECHAN_FONTLOADER_HPP
