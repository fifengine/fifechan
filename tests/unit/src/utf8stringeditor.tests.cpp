// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2013 - 2026 Fifengine contributors

// Standard library includes
#include <string>

// Third-party library includes
#include <catch2/catch_test_macros.hpp>

// Project headers (subdirs before local)
#include "fifechan/utf8stringeditor.hpp"

TEST_CASE("UTF8StringEditor nextChar moves to next character", "[unit][utf8stringeditor]")
{
    SECTION("ASCII text")
    {
        std::string const text = "ABC";
        REQUIRE(fcn::UTF8StringEditor::nextChar(text, 0) == 1);
        REQUIRE(fcn::UTF8StringEditor::nextChar(text, 1) == 2);
        REQUIRE(fcn::UTF8StringEditor::nextChar(text, 2) == 3);
    }

    SECTION("UTF-8 text with multibyte characters")
    {
        std::string const text = "A\u00E9BC";
        REQUIRE(fcn::UTF8StringEditor::nextChar(text, 0) == 1);
        REQUIRE(fcn::UTF8StringEditor::nextChar(text, 1) == 3);
        REQUIRE(fcn::UTF8StringEditor::nextChar(text, 3) == 4);
        REQUIRE(fcn::UTF8StringEditor::nextChar(text, 4) == 5);
    }
}

TEST_CASE("UTF8StringEditor prevChar moves to previous character", "[unit][utf8stringeditor]")
{
    SECTION("ASCII text")
    {
        std::string const text = "ABC";
        REQUIRE(fcn::UTF8StringEditor::prevChar(text, 1) == 0);
        REQUIRE(fcn::UTF8StringEditor::prevChar(text, 2) == 1);
        REQUIRE(fcn::UTF8StringEditor::prevChar(text, 3) == 2);
    }

    SECTION("UTF-8 text with multibyte characters")
    {
        std::string const text = "A\u00E9BC";
        REQUIRE(fcn::UTF8StringEditor::prevChar(text, 3) == 1);
        REQUIRE(fcn::UTF8StringEditor::prevChar(text, 4) == 3);
        REQUIRE(fcn::UTF8StringEditor::prevChar(text, 5) == 4);
    }
}

TEST_CASE("UTF8StringEditor eraseChar removes character", "[unit][utf8stringeditor]")
{
    SECTION("ASCII text")
    {
        std::string text = "ABC";
        REQUIRE(fcn::UTF8StringEditor::eraseChar(text, 1) == 1);
        REQUIRE(text == "AC");
    }

    SECTION("UTF-8 text with multibyte characters")
    {
        std::string text = "A\u00E9BC";
        REQUIRE(fcn::UTF8StringEditor::eraseChar(text, 1) == 1);
        REQUIRE(text == "ABC");
    }
}

TEST_CASE("UTF8StringEditor insertChar inserts character", "[unit][utf8stringeditor]")
{
    SECTION("ASCII character in ASCII text")
    {
        std::string text = "ABC";
        REQUIRE(fcn::UTF8StringEditor::insertChar(text, 1, 'X') == 2);
        REQUIRE(text == "AXBC");
    }

    SECTION("UTF-8 character in ASCII text")
    {
        std::string text = "ABC";
        REQUIRE(fcn::UTF8StringEditor::insertChar(text, 1, 0x00E9) == 3);
        REQUIRE(text == "A\u00E9BC");
    }

    SECTION("Insert at beginning")
    {
        std::string text = "ABC";
        REQUIRE(fcn::UTF8StringEditor::insertChar(text, 0, 'X') == 1);
        REQUIRE(text == "XABC");
    }

    SECTION("Insert at end")
    {
        std::string text = "ABC";
        REQUIRE(fcn::UTF8StringEditor::insertChar(text, 3, 'X') == 4);
        REQUIRE(text == "ABCX");
    }
}

TEST_CASE("UTF8StringEditor countChars counts characters", "[unit][utf8stringeditor]")
{
    SECTION("ASCII text")
    {
        std::string const text = "ABC";
        REQUIRE(fcn::UTF8StringEditor::countChars(text, 0) == 0);
        REQUIRE(fcn::UTF8StringEditor::countChars(text, 1) == 1);
        REQUIRE(fcn::UTF8StringEditor::countChars(text, 2) == 2);
        REQUIRE(fcn::UTF8StringEditor::countChars(text, 3) == 3);
    }

    SECTION("UTF-8 text with multibyte characters")
    {
        std::string const text = "A\u00E9BC";
        REQUIRE(fcn::UTF8StringEditor::countChars(text, 0) == 0);
        REQUIRE(fcn::UTF8StringEditor::countChars(text, 1) == 1);
        REQUIRE(fcn::UTF8StringEditor::countChars(text, 3) == 2);
        REQUIRE(fcn::UTF8StringEditor::countChars(text, 4) == 3);
        REQUIRE(fcn::UTF8StringEditor::countChars(text, 5) == 4);
    }
}

TEST_CASE("UTF8StringEditor getOffset gets byte offset for character index", "[unit][utf8stringeditor]")
{
    SECTION("ASCII text")
    {
        std::string const text = "ABC";
        REQUIRE(fcn::UTF8StringEditor::getOffset(text, 0) == 0);
        REQUIRE(fcn::UTF8StringEditor::getOffset(text, 1) == 1);
        REQUIRE(fcn::UTF8StringEditor::getOffset(text, 2) == 2);
        REQUIRE(fcn::UTF8StringEditor::getOffset(text, 3) == 3);
    }

    SECTION("UTF-8 text with multibyte characters")
    {
        std::string const text = "A\u00E9BC";
        REQUIRE(fcn::UTF8StringEditor::getOffset(text, 0) == 0);
        REQUIRE(fcn::UTF8StringEditor::getOffset(text, 1) == 1);
        REQUIRE(fcn::UTF8StringEditor::getOffset(text, 2) == 3);
        REQUIRE(fcn::UTF8StringEditor::getOffset(text, 3) == 4);
        REQUIRE(fcn::UTF8StringEditor::getOffset(text, 4) == 5);
    }

    SECTION("Index beyond string length")
    {
        std::string const text = "ABC";
        REQUIRE(fcn::UTF8StringEditor::getOffset(text, 10) == 3);
    }
}

// cppcheck-suppress-begin knownConditionTrueFalse
// The warning suggests to remove sections that are always true or false.
// False positive: analyzer cannot model UTF-8 encoding of \UXXXXXXXX literals,
// so `text.size()` for multi-byte characters is evaluated wrongly.

TEST_CASE("UTF8StringEditor handles hello world in multiple languages", "[unit][utf8stringeditor]")
{
    SECTION("Chinese")
    {
        std::string const text = "\u4F60\u597D\u4E16\u754C";
        REQUIRE(text == "你好世界");
        REQUIRE(fcn::UTF8StringEditor::countChars(text, static_cast<int>(text.size())) == 4);
    }

    SECTION("Japanese")
    {
        std::string const text = "\u3053\u3093\u306B\u3061\u306F";
        REQUIRE(text == "こんにちは");
        REQUIRE(fcn::UTF8StringEditor::countChars(text, static_cast<int>(text.size())) == 5);
    }

    SECTION("Arabic")
    {
        std::string const text = "\u0645\u0631\u062D\u0628\u0627\u064B";
        REQUIRE(text == "مرحباً");
        REQUIRE(fcn::UTF8StringEditor::countChars(text, static_cast<int>(text.size())) == 6);
    }

    SECTION("Russian")
    {
        std::string const text = "\u041F\u0440\u0438\u0432\u0435\u0442";
        REQUIRE(text == "Привет");
        REQUIRE(fcn::UTF8StringEditor::countChars(text, static_cast<int>(text.size())) == 6);
    }

    SECTION("Greek")
    {
        std::string const text = "\u0393\u03B5\u03B9\u03AC";
        REQUIRE(text == "Γειά");
        REQUIRE(fcn::UTF8StringEditor::countChars(text, static_cast<int>(text.size())) == 4);
    }

    SECTION("Korean")
    {
        std::string const text = "\uC548\uB155\uD558\uC138\uC694";
        REQUIRE(text == "안녕하세요");
        REQUIRE(fcn::UTF8StringEditor::countChars(text, static_cast<int>(text.size())) == 5);
    }

    SECTION("Hebrew")
    {
        std::string const text = "\u05E9\u05DC\u05D5\u05DD";
        REQUIRE(text == "שלום");
        REQUIRE(fcn::UTF8StringEditor::countChars(text, static_cast<int>(text.size())) == 4);
    }

    SECTION("Thai")
    {
        std::string const text = "\u0E2A\u0E27\u0E31\u0E2A\u0E14\u0E35";
        REQUIRE(text == "สวัสดี");
        REQUIRE(fcn::UTF8StringEditor::countChars(text, static_cast<int>(text.size())) == 6);
    }
}

TEST_CASE("UTF8StringEditor handles UTF emojis", "[unit][utf8stringeditor]")
{
    SECTION("waving hand emoji")
    {
        std::string const text = "\U0001F44B";
        REQUIRE(text == "\U0001F44B");
        REQUIRE(fcn::UTF8StringEditor::countChars(text, static_cast<int>(text.size())) == 1);
        REQUIRE(text.size() == 4);
    }

    SECTION("smiling face with sunglasses")
    {
        std::string const text = "\U0001F60E";
        REQUIRE(fcn::UTF8StringEditor::countChars(text, static_cast<int>(text.size())) == 1);
        REQUIRE(text.size() == 4);
    }

    SECTION("fire emoji")
    {
        std::string const text = "\U0001F525";
        REQUIRE(fcn::UTF8StringEditor::countChars(text, static_cast<int>(text.size())) == 1);
        REQUIRE(text.size() == 4);
    }

    SECTION("emoji followed by text")
    {
        std::string const text = "\U0001F44BHello";
        REQUIRE(fcn::UTF8StringEditor::countChars(text, static_cast<int>(text.size())) == 6);
        REQUIRE(fcn::UTF8StringEditor::getOffset(text, 1) == 4);
        REQUIRE(fcn::UTF8StringEditor::getOffset(text, 5) == 8);
    }

    SECTION("flag emoji")
    {
        std::string const text = "\U0001F1FA\U0001F1F8";
        REQUIRE(fcn::UTF8StringEditor::countChars(text, static_cast<int>(text.size())) == 2);
        REQUIRE(text.size() == 8);
    }

    SECTION("mixed emoji and CJK")
    {
        std::string const text = "\U0001F44B \u4F60\u597D";
        REQUIRE(fcn::UTF8StringEditor::countChars(text, static_cast<int>(text.size())) == 4);
        REQUIRE(fcn::UTF8StringEditor::nextChar(text, 0) == 4);
        REQUIRE(fcn::UTF8StringEditor::nextChar(text, 5) == 8);
    }

    SECTION("erase emoji")
    {
        std::string text = "\U0001F44B\U0001F60E\U0001F525";
        REQUIRE(fcn::UTF8StringEditor::eraseChar(text, 0) == 0);
        REQUIRE(text.size() == 8);
        REQUIRE(fcn::UTF8StringEditor::countChars(text, static_cast<int>(text.size())) == 2);
    }

    SECTION("insert emoji")
    {
        std::string text = "Hi!";
        REQUIRE(fcn::UTF8StringEditor::insertChar(text, 2, 0x1F44B) == 6);
        REQUIRE(text == "Hi\U0001F44B!");
        REQUIRE(fcn::UTF8StringEditor::countChars(text, static_cast<int>(text.size())) == 4);
    }
}

// cppcheck-suppress-end knownConditionTrueFalse
