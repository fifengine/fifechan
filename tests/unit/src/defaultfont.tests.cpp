// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2013 - 2026 Fifengine contributors

// Standard library includes
#include <string>

// Third-party library includes
#include <catch2/catch_test_macros.hpp>

// Project headers
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

TEST_CASE("DefaultFont getWidth handles very long strings", "[unit][defaultfont][long]")
{
    DefaultFont font;

    // Test with a very long string
    std::string longString(1000, 'A');
    REQUIRE(font.getWidth(longString) == 8000);

    // Test with single character
    REQUIRE(font.getWidth("x") == 8);
}

TEST_CASE("DefaultFont getStringIndexAt edge cases", "[unit][defaultfont][index]")
{
    DefaultFont font;

    // x at start of string
    REQUIRE(font.getStringIndexAt("Hello", 0) == 0);

    // x within first character (width per char = 8)
    REQUIRE(font.getStringIndexAt("Hello", 1) == 0);
    REQUIRE(font.getStringIndexAt("Hello", 4) == 0);
    REQUIRE(font.getStringIndexAt("Hello", 7) == 0);

    // x at exact character boundaries
    REQUIRE(font.getStringIndexAt("Hello", 8) == 1);  // start of 2nd char
    REQUIRE(font.getStringIndexAt("Hello", 16) == 2); // start of 3rd char
    REQUIRE(font.getStringIndexAt("Hello", 24) == 3); // start of 4th char
    REQUIRE(font.getStringIndexAt("Hello", 32) == 4); // start of 5th char

    // x beyond string end
    REQUIRE(font.getStringIndexAt("Hello", 100) == 5); // length of "Hello"
    REQUIRE(font.getStringIndexAt("Hello", 1000) == 5);

    // x negative should wrap to 0 (edge case)
    REQUIRE(font.getStringIndexAt("Hello", -1) == 0);
    REQUIRE(font.getStringIndexAt("Hello", -100) == 0);
}

TEST_CASE("DefaultFont getStringIndexAt with empty string", "[unit][defaultfont][index][empty]")
{
    DefaultFont font;

    // Empty string with any x should return 0
    REQUIRE(font.getStringIndexAt("", 0) == 0);
    REQUIRE(font.getStringIndexAt("", 5) == 0);
    REQUIRE(font.getStringIndexAt("", 100) == 0);
}

TEST_CASE("DefaultFont renderToSurface empty string returns nullptr", "[unit][defaultfont][render][empty]")
{
    DefaultFont font;

    // Rendering empty string should return nullptr
    auto surface = font.renderToSurface("");
    REQUIRE(surface == nullptr);
}

TEST_CASE("DefaultFont renderToSurface non-empty string returns valid surface", "[unit][defaultfont][render][happy]")
{
    DefaultFont font;

    // Rendering non-empty string should return valid surface
    auto surface = font.renderToSurface("Hello");
    REQUIRE(surface != nullptr);
    REQUIRE(surface->w == 40); // 5 chars * 8 pixels each
    REQUIRE(surface->h == 8);  // fixed height
}

TEST_CASE("DefaultFont renderToSurface single character", "[unit][defaultfont][render][single]")
{
    DefaultFont font;

    // Rendering single character
    auto surface = font.renderToSurface("A");
    REQUIRE(surface != nullptr);
    REQUIRE(surface->w == 8); // 1 char * 8 pixels
    REQUIRE(surface->h == 8); // fixed height
}

TEST_CASE("DefaultFont renderToSurface multiple characters", "[unit][defaultfont][render][multi]")
{
    DefaultFont font;

    // Rendering multiple characters
    auto surface = font.renderToSurface("ABC");
    REQUIRE(surface != nullptr);
    REQUIRE(surface->w == 24); // 3 chars * 8 pixels each
    REQUIRE(surface->h == 8);  // fixed height
}

TEST_CASE("DefaultFont renderToSurface with spaces", "[unit][defaultfont][render][spaces]")
{
    DefaultFont font;

    // Rendering string with spaces
    auto surface = font.renderToSurface("A B C");
    REQUIRE(surface != nullptr);
    // "A B C" is 5 characters including spaces
    REQUIRE(surface->w == 40); // 5 * 8
    REQUIRE(surface->h == 8);
}
