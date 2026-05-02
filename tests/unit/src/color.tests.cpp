// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2013 - 2026 Fifengine contributors

// Corresponding header include

// Standard library includes
#include <sstream>
#include <stdexcept>

// Third-party library includes
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

// Project headers (subdirs before local)
#include "fifechan/color.hpp"

using namespace fcn;

// ============================================================================
// Constructors
// ============================================================================

TEST_CASE("Color hex integer constructor", "[unit][color]")
{
    Color const color(0x112233);
    REQUIRE(color.r == 0x11);
    REQUIRE(color.g == 0x22);
    REQUIRE(color.b == 0x33);
    REQUIRE(color.a == 0xFF);
}

TEST_CASE("Color int RGBA constructor", "[unit][color]")
{
    Color const color(10, 20, 30, 40);
    REQUIRE(color.r == 10);
    REQUIRE(color.g == 20);
    REQUIRE(color.b == 30);
    REQUIRE(color.a == 40);
}

TEST_CASE("Color uint8_t RGBA constructor", "[unit][color]")
{
    Color const color(
        static_cast<uint8_t>(10), static_cast<uint8_t>(20), static_cast<uint8_t>(30), static_cast<uint8_t>(40));
    REQUIRE(color.r == 10);
    REQUIRE(color.g == 20);
    REQUIRE(color.b == 30);
    REQUIRE(color.a == 40);
}

TEST_CASE("Color default constructor", "[unit][color]")
{
    Color const color;
    REQUIRE(color.r == 0);
    REQUIRE(color.g == 0);
    REQUIRE(color.b == 0);
    REQUIRE(color.a == 255);
}

// ============================================================================
// String constructors and parsing
// ============================================================================

TEST_CASE("Color hex string constructor", "[unit][color]")
{
    Color const color("#AABBCC");
    REQUIRE(color.r == 0xAA);
    REQUIRE(color.g == 0xBB);
    REQUIRE(color.b == 0xCC);
    REQUIRE(color.a == 255);
}

TEST_CASE("Color rgb string constructor", "[unit][color]")
{
    Color const color("rgb(1,2,3)");
    REQUIRE(color.r == 1);
    REQUIRE(color.g == 2);
    REQUIRE(color.b == 3);
    REQUIRE(color.a == 255);
}

TEST_CASE("Color rgba string constructor", "[unit][color]")
{
    Color const color("rgba(10,20,30,40)");
    REQUIRE(color.r == 10);
    REQUIRE(color.g == 20);
    REQUIRE(color.b == 30);
    REQUIRE(color.a == 40);
}

TEST_CASE("Color invalid hex string throws", "[unit][color]")
{
    // Hex code must be exactly 6 characters after '#'
    REQUIRE_THROWS_AS(Color("#12345"), std::invalid_argument);   // too short
    REQUIRE_THROWS_AS(Color("#1234567"), std::invalid_argument); // too long
}

TEST_CASE("Color invalid rgb string throws", "[unit][color]")
{
    REQUIRE_THROWS_AS(Color("rgb(1,2)"), std::invalid_argument);
    REQUIRE_THROWS_AS(Color("rgb(1,2,3"), std::invalid_argument);
    REQUIRE_THROWS_AS(Color("rgb 1,2,3)"), std::invalid_argument);
}

TEST_CASE("Color invalid rgba string throws", "[unit][color]")
{
    REQUIRE_THROWS_AS(Color("rgba(1,2,3)"), std::invalid_argument);
    REQUIRE_THROWS_AS(Color("rgba(1,2,3,4"), std::invalid_argument);
}

TEST_CASE("Color invalid component values throw", "[unit][color]")
{
    REQUIRE_THROWS_AS(Color("rgb(abc,2,3)"), std::invalid_argument);
    REQUIRE_THROWS_AS(Color("rgb(1,abc,3)"), std::invalid_argument);
    REQUIRE_THROWS_AS(Color("rgb(1,2,abc)"), std::invalid_argument);
}

// ============================================================================
// Arithmetic operators (Color + Color)
// ============================================================================

TEST_CASE("Color operator+ clamps and keeps alpha", "[unit][color]")
{
    Color const lhs(250, 10, 5, 200);
    Color const rhs(10, 20, 30, 100);

    Color const result = lhs + rhs;
    REQUIRE(result.r == 255);
    REQUIRE(result.g == 30);
    REQUIRE(result.b == 35);
    REQUIRE(result.a == 200); // Alpha unchanged
}

TEST_CASE("Color operator- clamps and keeps alpha", "[unit][color]")
{
    Color const lhs(250, 10, 5, 200);
    Color const rhs(10, 20, 30, 100);

    Color const result = lhs - rhs;
    REQUIRE(result.r == 240);
    REQUIRE(result.g == 0);
    REQUIRE(result.b == 0);
    REQUIRE(result.a == 200); // Alpha unchanged
}

TEST_CASE("Color operator* scales and keeps alpha", "[unit][color]")
{
    Color const color(100, 150, 200, 128);

    Color const result = color * 2.0f;
    REQUIRE(result.r == 200);
    REQUIRE(result.g == 255); // Clamped
    REQUIRE(result.b == 255); // Clamped
    REQUIRE(result.a == 128); // Alpha unchanged
}

// ============================================================================
// Arithmetic operators (Color + float, Color - float)
// ============================================================================

TEST_CASE("Color operator+ float clamps and keeps alpha", "[unit][color]")
{
    Color const color(250, 10, 5, 200);

    Color const result = color + 20.0f;
    REQUIRE(result.r == 255); // Clamped
    REQUIRE(result.g == 30);
    REQUIRE(result.b == 25);
    REQUIRE(result.a == 200); // Alpha unchanged
}

TEST_CASE("Color operator- float clamps and keeps alpha", "[unit][color]")
{
    Color const color(10, 20, 30, 200);

    Color const result = color - 20.0f;
    REQUIRE(result.r == 0); // Clamped
    REQUIRE(result.g == 0); // Clamped
    REQUIRE(result.b == 10);
    REQUIRE(result.a == 200); // Alpha unchanged
}

// ============================================================================
// Compound assignment operators
// ============================================================================

TEST_CASE("Color operator+= modifies and keeps alpha", "[unit][color]")
{
    Color color(250, 10, 5, 200);
    Color const rhs(10, 20, 30, 100);

    color += rhs;
    REQUIRE(color.r == 255); // Clamped
    REQUIRE(color.g == 30);
    REQUIRE(color.b == 35);
    REQUIRE(color.a == 200); // Alpha unchanged
}

TEST_CASE("Color operator-= modifies and keeps alpha", "[unit][color]")
{
    Color color(250, 10, 5, 200);
    Color const rhs(10, 20, 30, 100);

    color -= rhs;
    REQUIRE(color.r == 240);
    REQUIRE(color.g == 0);   // Clamped
    REQUIRE(color.b == 0);   // Clamped
    REQUIRE(color.a == 200); // Alpha unchanged
}

TEST_CASE("Color operator*= modifies and keeps alpha", "[unit][color]")
{
    Color color(100, 150, 200, 128);

    color *= 2.0f;
    REQUIRE(color.r == 200);
    REQUIRE(color.g == 255); // Clamped
    REQUIRE(color.b == 255); // Clamped
    REQUIRE(color.a == 128); // Alpha unchanged
}

// ============================================================================
// Color modifications
// ============================================================================

TEST_CASE("Color lighten increases brightness", "[unit][color]")
{
    Color const color(100, 100, 100, 128);

    Color const result = color.lighten(0.5f);
    REQUIRE(result.r == 150);
    REQUIRE(result.g == 150);
    REQUIRE(result.b == 150);
    REQUIRE(result.a == 128); // Alpha unchanged
}

TEST_CASE("Color lighten clamps to 255", "[unit][color]")
{
    Color const color(200, 200, 200, 128);

    Color const result = color.lighten(1.0f);
    REQUIRE(result.r == 255);
    REQUIRE(result.g == 255);
    REQUIRE(result.b == 255);
    REQUIRE(result.a == 128);
}

TEST_CASE("Color darken decreases brightness", "[unit][color]")
{
    Color const color(100, 100, 100, 128);

    Color const result = color.darken(0.5f);
    REQUIRE(result.r == 50);
    REQUIRE(result.g == 50);
    REQUIRE(result.b == 50);
    REQUIRE(result.a == 128); // Alpha unchanged
}

TEST_CASE("Color darken clamps to 0", "[unit][color]")
{
    Color const color(100, 100, 100, 128);

    Color const result = color.darken(1.0f);
    REQUIRE(result.r == 0);
    REQUIRE(result.g == 0);
    REQUIRE(result.b == 0);
    REQUIRE(result.a == 128);
}

TEST_CASE("Color toGrayScale converts correctly", "[unit][color]")
{
    // Gray = r*0.3 + g*0.59 + b*0.11
    Color const color(100, 200, 50, 128);

    Color const result = color.toGrayScale();
    // Expected: 100*0.3 + 200*0.59 + 50*0.11 = 30 + 118 + 5.5 = 153.5 ≈ 153
    REQUIRE(result.r == 153);
    REQUIRE(result.g == 153);
    REQUIRE(result.b == 153);
    REQUIRE(result.a == 128); // Alpha unchanged
}

// ============================================================================
// Blend
// ============================================================================

TEST_CASE("Color blendWith blends two colors", "[unit][color]")
{
    Color const color1(100, 100, 100, 128); // Semi-transparent gray
    Color const color2(200, 0, 0, 128);     // Semi-transparent red

    Color const result = color1.blendWith(color2);
    // Blended color should have some red component
    REQUIRE(result.r > color1.r);
    REQUIRE(result.g < color1.g);
    REQUIRE(result.b < color1.b);
    // Alpha should be blended too
    REQUIRE(result.a > color1.a);
}

TEST_CASE("Color blendWith with opaque colors", "[unit][color]")
{
    Color const color1(100, 100, 100, 255); // Opaque gray
    Color const color2(200, 0, 0, 255);     // Opaque red

    Color const result = color1.blendWith(color2);
    // When both are opaque, result should be color2
    REQUIRE(result.r == 200);
    REQUIRE(result.g == 0);
    REQUIRE(result.b == 0);
    REQUIRE(result.a == 255);
}

// ============================================================================
// Comparison operators
// ============================================================================

TEST_CASE("Color operator== returns true for identical colors", "[unit][color]")
{
    Color const color1(10, 20, 30, 40);
    Color const color2(10, 20, 30, 40);

    REQUIRE(color1 == color2);
    REQUIRE_FALSE(color1 != color2);
}

TEST_CASE("Color operator== returns false for different colors", "[unit][color]")
{
    Color const color1(10, 20, 30, 40);
    Color const color2(11, 20, 30, 40);

    REQUIRE_FALSE(color1 == color2);
    REQUIRE(color1 != color2);
}

TEST_CASE("Color operator!= returns true for different alpha", "[unit][color]")
{
    Color const color1(10, 20, 30, 40);
    Color const color2(10, 20, 30, 41);

    REQUIRE(color1 != color2);
}

// ============================================================================
// String conversions
// ============================================================================

TEST_CASE("Color toHexString returns correct format", "[unit][color]")
{
    Color const color(0xAB, 0xCD, 0xEF, 255);

    std::string const hex = color.toHexString();
    REQUIRE(hex == "#abcdef");
}

TEST_CASE("Color toRGBString returns correct format", "[unit][color]")
{
    Color const color(10, 20, 30, 255);

    std::string const rgb = color.toRGBString();
    REQUIRE(rgb == "rgb(10,20,30)");
}

TEST_CASE("Color toRGBAString returns correct format", "[unit][color]")
{
    Color const color(10, 20, 30, 40);

    std::string const rgba = color.toRGBAString();
    REQUIRE(rgba == "rgba(10,20,30,40)");
}

// ============================================================================
// Stream operator
// ============================================================================

TEST_CASE("Color stream operator outputs correct format", "[unit][color]")
{
    Color const color(10, 20, 30, 40);

    std::stringstream ss;
    ss << color;

    std::string const output = ss.str();
    REQUIRE(output.find("r = 10") != std::string::npos);
    REQUIRE(output.find("g = 20") != std::string::npos);
    REQUIRE(output.find("b = 30") != std::string::npos);
    REQUIRE(output.find("a = 40") != std::string::npos);
}
