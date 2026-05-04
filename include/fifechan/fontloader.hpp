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
            /**
             * Returns default search paths used when locating font files
             * on the current platform.
             */
            static std::vector<std::filesystem::path> getDefaultSearchPaths();

            /**
             * Find a font file by name searching the provided paths.
             *
             * @param fontName Font family or filename to search for.
             * @param searchPaths Optional list of paths to search. If empty
             *                    the default search paths are used.
             * @return Path to the found font file or empty path when not found.
             */
            static std::filesystem::path findFontFile(
                std::string const & fontName, std::vector<std::filesystem::path> const & searchPaths = {});

            /**
             * Load a `Font` object for the given font name and size using
             * the provided `Graphics` implementation.
             *
             * @param graphics Graphics instance used for creating font resources.
             * @param fontName Font family or filename.
             * @param size Requested font size in points.
             * @param searchPaths Optional search paths for locating font files.
             * @return Shared pointer to a `Font` instance or nullptr on failure.
             */
            static std::shared_ptr<Font> loadFont(
                Graphics& graphics,
                std::string const & fontName,
                int size,
                std::vector<std::filesystem::path> const & searchPaths = {});
    };

} // namespace fcn::font

#endif // INCLUDE_FIFECHAN_FONTLOADER_HPP
