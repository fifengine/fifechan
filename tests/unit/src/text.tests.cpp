// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2013 - 2026 Fifengine contributors

// Standard library includes
#include <stdexcept>

// Third-party library includes
#include <catch2/catch_test_macros.hpp>

// Project headers (subdirs before local)
#include "fifechan/text.hpp"

TEST_CASE("Text constructors initialize properly", "[unit][text]")
{
    SECTION("default constructor creates empty text")
    {
        fcn::Text text;
        REQUIRE(text.getContent() == "");
        REQUIRE(text.getNumberOfRows() == 1);
    }

    SECTION("string constructor creates rows")
    {
        fcn::Text text("Line1\nLine2\nLine3");
        REQUIRE(text.getNumberOfRows() == 3);
    }

    SECTION("string constructor handles no newlines")
    {
        fcn::Text text("Single line");
        REQUIRE(text.getNumberOfRows() == 1);
        REQUIRE(text.getContent() == "Single line");
    }
}

TEST_CASE("Text getContent reconstructs original", "[unit][text]")
{
    SECTION("single line")
    {
        fcn::Text text("Hello");
        REQUIRE(text.getContent() == "Hello");
    }

    SECTION("multiple lines")
    {
        fcn::Text text("Line1\nLine2\nLine3");
        REQUIRE(text.getContent() == "Line1\nLine2\nLine3");
    }

    SECTION("empty text")
    {
        fcn::Text text;
        REQUIRE(text.getContent() == "");
    }
}

TEST_CASE("Text setContent replaces content", "[unit][text]")
{
    fcn::Text text("Original");
    text.setContent("New content");
    REQUIRE(text.getContent() == "New content");
}

TEST_CASE("Text row operations", "[unit][text]")
{
    fcn::Text text("Line1\nLine2\nLine3");

    SECTION("getRow returns correct row")
    {
        REQUIRE(text.getRow(0) == "Line1");
        REQUIRE(text.getRow(1) == "Line2");
        REQUIRE(text.getRow(2) == "Line3");
    }

    SECTION("setRow updates row")
    {
        text.setRow(1, "Modified");
        REQUIRE(text.getRow(1) == "Modified");
    }

    SECTION("addRow appends row")
    {
        text.addRow("Line4");
        REQUIRE(text.getNumberOfRows() == 4);
        REQUIRE(text.getRow(3) == "Line4");
    }

    SECTION("insertRow inserts at position")
    {
        text.insertRow("Inserted", 1);
        REQUIRE(text.getNumberOfRows() == 4);
        REQUIRE(text.getRow(0) == "Line1");
        REQUIRE(text.getRow(1) == "Inserted");
        REQUIRE(text.getRow(2) == "Line2");
    }

    SECTION("insertRow at end calls addRow")
    {
        text.insertRow("Line4", 3);
        REQUIRE(text.getNumberOfRows() == 4);
        REQUIRE(text.getRow(3) == "Line4");
    }

    SECTION("eraseRow removes row")
    {
        text.eraseRow(1);
        REQUIRE(text.getNumberOfRows() == 2);
        REQUIRE(text.getRow(0) == "Line1");
        REQUIRE(text.getRow(1) == "Line3");
    }
}

TEST_CASE("Text addRow rejects newlines", "[unit][text]")
{
    fcn::Text text;
    REQUIRE_THROWS_AS(text.addRow("Line with\nnewline"), std::runtime_error);
}

TEST_CASE("Text insertRow rejects newlines", "[unit][text]")
{
    fcn::Text text("Line1\nLine2");
    REQUIRE_THROWS_AS(text.insertRow("Line with\nnewline", 1), std::runtime_error);
}

TEST_CASE("Text caret position operations", "[unit][text]")
{
    fcn::Text text("Line1\nLine2\nLine3");

    SECTION("initial caret position")
    {
        REQUIRE(text.getCaretPosition() == 0);
        REQUIRE(text.getCaretRow() == 0);
        REQUIRE(text.getCaretColumn() == 0);
    }

    SECTION("setCaretPosition moves caret")
    {
        text.setCaretPosition(7);
        REQUIRE(text.getCaretPosition() == 7);
        REQUIRE(text.getCaretRow() == 1);
        REQUIRE(text.getCaretColumn() == 1);
    }

    SECTION("setCaretPosition clamps to end")
    {
        text.setCaretPosition(100);
        REQUIRE(text.getCaretRow() == 2);
    }

    SECTION("setCaretColumn")
    {
        text.setCaretColumn(3);
        REQUIRE(text.getCaretColumn() == 3);
    }

    SECTION("setCaretRow")
    {
        text.setCaretRow(2);
        REQUIRE(text.getCaretRow() == 2);
    }

    SECTION("setCaretRow clamps to last row")
    {
        text.setCaretRow(100);
        REQUIRE(text.getCaretRow() == 2);
    }
}

TEST_CASE("Text insert character", "[unit][text]")
{
    SECTION("insert at beginning")
    {
        fcn::Text text("BCD");
        text.setCaretPosition(0);
        text.insert('A');
        REQUIRE(text.getContent() == "ABCD");
    }

    SECTION("insert newline splits row")
    {
        fcn::Text text("Line1\nLine2");
        text.setCaretPosition(4);
        text.insert('\n');
        REQUIRE(text.getNumberOfRows() == 3);
        REQUIRE(text.getRow(0) == "Line");
        REQUIRE(text.getRow(1) == "1Line2");
    }
}

TEST_CASE("Text remove characters", "[unit][text]")
{
    SECTION("remove forward")
    {
        fcn::Text text("ABCD");
        text.setCaretPosition(1);
        text.remove(2);
        REQUIRE(text.getContent() == "AD");
    }

    SECTION("remove backward")
    {
        fcn::Text text("ABCD");
        text.setCaretPosition(3);
        text.remove(-2);
        REQUIRE(text.getContent() == "AD");
    }

    SECTION("remove merges rows at boundary")
    {
        fcn::Text text("Line1\nLine2");
        text.setCaretPosition(5);
        text.remove(-1);
        REQUIRE(text.getContent() == "Line1Line2");
        REQUIRE(text.getNumberOfRows() == 1);
    }
}

TEST_CASE("Text getNumberOfCharacters", "[unit][text]")
{
    SECTION("single line")
    {
        fcn::Text text("ABC");
        REQUIRE(text.getNumberOfCharacters() == 4);
    }

    SECTION("multiple lines")
    {
        fcn::Text text("AB\nCD");
        REQUIRE(text.getNumberOfCharacters() == 6);
    }
}

TEST_CASE("Text getNumberOfCharacters for row", "[unit][text]")
{
    fcn::Text text("AB\nCD\nEF");
    REQUIRE(text.getNumberOfCharacters(0) == 2);
    REQUIRE(text.getNumberOfCharacters(1) == 2);
    REQUIRE(text.getNumberOfCharacters(2) == 2);
    REQUIRE(text.getNumberOfCharacters(99) == 0);
}

TEST_CASE("Text handles UTF-8", "[unit][text]")
{
    SECTION("Chinese characters")
    {
        fcn::Text text("你好世界");
        REQUIRE(text.getNumberOfRows() == 1);
        REQUIRE(text.getRow(0) == "你好世界");
    }

    SECTION("emoji")
    {
        fcn::Text text("Hello \U0001F44B");
        REQUIRE(text.getNumberOfRows() == 1);
    }

    SECTION("multiline with UTF-8")
    {
        fcn::Text text("你好\n世界\nHello");
        REQUIRE(text.getNumberOfRows() == 3);
        REQUIRE(text.getRow(0) == "你好");
        REQUIRE(text.getRow(1) == "世界");
        REQUIRE(text.getRow(2) == "Hello");
    }
}

TEST_CASE("Text empty state", "[unit][text]")
{
    fcn::Text text;

    SECTION("always has at least one row")
    {
        REQUIRE(text.getNumberOfRows() == 1);
    }

    SECTION("addRow works on empty text")
    {
        text.addRow("First");
        REQUIRE(text.getNumberOfRows() == 2);
        REQUIRE(text.getRow(1) == "First");
    }
}
