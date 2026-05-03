// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2013 - 2026 Fifengine contributors

// Third-party library includes
#include <catch2/catch_test_macros.hpp>

// Project headers (subdirs before local)
#include "fifechan/defaultfont.hpp"

using fcn::DefaultFont;

// ============================================================================
// DefaultFont basic properties
// ============================================================================

TEST_CASE("DefaultFont getHeight returns 8", "[unit][defaultfont]")
{
    DefaultFont font;
    REQUIRE(font.getHeight() == 8);
}

TEST_CASE("DefaultFont getWidth returns 8 times string length", "[unit][defaultfont]")
{
    DefaultFont font;

    REQUIRE(font.getWidth("") == 0);
    REQUIRE(font.getWidth("A") == 8);
    REQUIRE(font.getWidth("ABC") == 24);
    REQUIRE(font.getWidth("Hello") == 40);
}

TEST_CASE("DefaultFont getStringIndexAt calculates correct index", "[unit][defaultfont]")
{
    DefaultFont font;

    // x at start of string
    REQUIRE(font.getStringIndexAt("Hello", 0) == 0);

    // x within first character
    REQUIRE(font.getStringIndexAt("Hello", 4) == 0);

    // x at second character
    REQUIRE(font.getStringIndexAt("Hello", 8) == 1);

    // x at third character
    REQUIRE(font.getStringIndexAt("Hello", 16) == 2);

    // x beyond string end
    REQUIRE(font.getStringIndexAt("Hello", 100) == 5); // length of "Hello"
}
