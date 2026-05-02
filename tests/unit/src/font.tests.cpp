// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2026 Jens A. Koch.
// This file is part of fifengine/fifechan.
//
// Unit tests for fifechan::Font

#include "fifechan/font.hpp"

#include <memory>
#include <string>

#include <catch2/catch_test_macros.hpp>
#include "fifechan/imagefont.hpp"

using fcn::Font;
using fcn::Graphics;
using fcn::ImageFont;

// Mock font for testing the base Font class functionality
class MockFont : public Font
{
    public:
        MockFont(int charWidth, int height) : m_charWidth(charWidth), m_height(height)
        {
        }

        int getWidth(std::string const & text) const override
        {
            return static_cast<int>(text.size()) * m_charWidth;
        }

        int getHeight() const override
        {
            return m_height;
        }

        void drawString(Graphics* graphics, std::string const & text, int x, int y) override
        {
            // Mock implementation - do nothing
        }

    private:
        int m_charWidth;
        int m_height;
};

TEST_CASE("Font::getStringIndexAt returns correct index for position", "[font]")
{
    MockFont font(10, 16);

    // Test with empty string
    REQUIRE(font.getStringIndexAt("", 0) == 0);
    REQUIRE(font.getStringIndexAt("", 5) == 0);

    // Test with simple string
    std::string text = "Hello";
    // The function returns the index i where getWidth(text.substr(0, i)) > x
    // For 10px per char: ""=0, "H"=10, "He"=20, "Hel"=30, "Hell"=40, "Hello"=50
    REQUIRE(font.getStringIndexAt(text, 0) == 1);   // getWidth("")=0 not >0, getWidth("H")=10>0, return 1
    REQUIRE(font.getStringIndexAt(text, 5) == 1);   // getWidth("H")=10>5, return 1
    REQUIRE(font.getStringIndexAt(text, 10) == 2);  // getWidth("H")=10 not >10, getWidth("He")=20>10, return 2
    REQUIRE(font.getStringIndexAt(text, 15) == 2);  // getWidth("He")=20>15, return 2
    REQUIRE(font.getStringIndexAt(text, 25) == 3);  // getWidth("Hel")=30>25, return 3
    REQUIRE(font.getStringIndexAt(text, 35) == 4);  // getWidth("Hell")=40>35, return 4
    REQUIRE(font.getStringIndexAt(text, 45) == 5);  // getWidth("Hello")=50>45, return 5
    REQUIRE(font.getStringIndexAt(text, 50) == 5);  // getWidth("Hello")=50 not >50, loop ends, return 5
    REQUIRE(font.getStringIndexAt(text, 100) == 5); // Loop ends, return 5
}

TEST_CASE("Font::getStringIndexAt handles single character", "[font]")
{
    MockFont font(10, 16);

    std::string text = "A";
    // getWidth("")=0, getWidth("A")=10
    REQUIRE(font.getStringIndexAt(text, 0) == 1);  // getWidth("A")=10>0, return 1
    REQUIRE(font.getStringIndexAt(text, 5) == 1);  // getWidth("A")=10>5, return 1
    REQUIRE(font.getStringIndexAt(text, 10) == 1); // getWidth("A")=10 not >10, loop ends, return 1
    REQUIRE(font.getStringIndexAt(text, 20) == 1); // loop ends, return 1
}

TEST_CASE("Font::getStringIndexAt handles multi-byte UTF-8 characters", "[font]")
{
    MockFont font(10, 16);

    // UTF-8 string with multi-byte characters
    // "Hello 🎉" = 10 bytes: H(1) + e(1) + l(1) + l(1) + o(1) + space(1) + emoji(4)
    std::string text = "Hello 🎉";
    // Width calculation in MockFont uses text.size() (bytes) * charWidth
    // So "Hello " (6 bytes) = 60px, "Hello 🎉" (10 bytes) = 100px

    // getWidth("")=0, "H"=10, "He"=20, "Hel"=30, "Hell"=40, "Hello"=50, "Hello "=60,
    // and the emoji adds 4 more bytes making the full string 10 bytes long.
    REQUIRE(font.getStringIndexAt(text, 0) == 1);    // getWidth("H")=10>0, return 1
    REQUIRE(font.getStringIndexAt(text, 30) == 4);   // getWidth("Hell")=40>30, return 4
    REQUIRE(font.getStringIndexAt(text, 60) == 7);   // position falls into bytes after the space, return 7
    REQUIRE(font.getStringIndexAt(text, 100) == 10); // loop ends, return text.size()=10
}

TEST_CASE("ImageFont getStringIndexAt works correctly", "[font]")
{
    // Use ImageFont to test through concrete implementation
    // Note: This requires SDL to be initialized, so we test the base class behavior
    MockFont font(8, 16);

    std::string text = "ABC";
    // Width: 3 * 8 = 24
    // getWidth("")=0, "A"=8, "AB"=16, "ABC"=24
    REQUIRE(font.getStringIndexAt(text, 0) == 1);   // getWidth("A")=8>0, return 1
    REQUIRE(font.getStringIndexAt(text, 8) == 2);   // getWidth("AB")=16>8, return 2
    REQUIRE(font.getStringIndexAt(text, 16) == 3);  // getWidth("ABC")=24>16, return 3
    REQUIRE(font.getStringIndexAt(text, 24) == 3);  // loop ends, return 3
    REQUIRE(font.getStringIndexAt(text, 100) == 3); // loop ends, return 3
}
