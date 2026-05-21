// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2026 Fifengine contributors

// Standard library includes
#include <algorithm>

// Third-party library includes
#include <catch2/catch_test_macros.hpp>

// Project headers
#include "fifechan/fontloader.hpp"
#include "fifechan/graphics.hpp"

using fcn::font::FontLoader;

TEST_CASE("FontLoader getDefaultSearchPaths returns non-empty", "[unit][fontloader]")
{
    auto paths = FontLoader::getDefaultSearchPaths();
    REQUIRE(!paths.empty());
}

TEST_CASE("FontLoader getDefaultSearchPaths contains current directory", "[unit][fontloader]")
{
    auto paths       = FontLoader::getDefaultSearchPaths();
    auto currentPath = std::filesystem::current_path();
    bool const found = std::ranges::any_of(paths, [&](auto const & p) {
        return p == currentPath;
    });
    REQUIRE(found);
}

TEST_CASE("FontLoader findFontFile returns empty for nonexistent font", "[unit][fontloader]")
{
    auto result = FontLoader::findFontFile("nonexistent_font_xyz123", {});
    REQUIRE(result.empty());
}

TEST_CASE("FontLoader findFontFile with custom search path", "[unit][fontloader]")
{
    auto result = FontLoader::findFontFile("nonexistent_font_xyz123", {"/tmp"});
    REQUIRE(result.empty());
}

TEST_CASE("FontLoader findFontFile with extension searches exact match", "[unit][fontloader]")
{
    auto result = FontLoader::findFontFile("nonexistent_font.ttf", {"/tmp"});
    REQUIRE(result.empty());
}
