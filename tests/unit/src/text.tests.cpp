// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2026 Fifengine contributors

// Standard library includes
#include <algorithm>
#include <stdexcept>
#include <string>
#include <utility>

// Third-party library includes
#include <catch2/catch_test_macros.hpp>

// Project headers (subdirs before local)
#include "fifechan/defaultfont.hpp"
#include "fifechan/exception.hpp"
#include "fifechan/text.hpp"

TEST_CASE("Text constructors initialize properly", "[unit][text]")
{
    SECTION("default constructor creates empty text")
    {
        fcn::Text const text;
        REQUIRE(text.getContent().empty());
        REQUIRE(text.getNumberOfRows() == 1);
    }

    SECTION("string constructor creates rows")
    {
        fcn::Text const text("Line1\nLine2\nLine3");
        REQUIRE(text.getNumberOfRows() == 3);
    }

    SECTION("string constructor handles no newlines")
    {
        fcn::Text const text("Single line");
        REQUIRE(text.getNumberOfRows() == 1);
        REQUIRE(text.getContent() == "Single line");
    }
}

TEST_CASE("Text getContent reconstructs original", "[unit][text]")
{
    SECTION("single line")
    {
        fcn::Text const text("Hello");
        REQUIRE(text.getContent() == "Hello");
    }

    SECTION("multiple lines")
    {
        fcn::Text const text("Line1\nLine2\nLine3");
        REQUIRE(text.getContent() == "Line1\nLine2\nLine3");
    }

    SECTION("empty text")
    {
        fcn::Text const text;
        REQUIRE(text.getContent().empty());
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
        REQUIRE(text.getRow(1) == "1");
        REQUIRE(text.getRow(2) == "Line2");
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
        fcn::Text const text("ABC");
        REQUIRE(text.getNumberOfCharacters() == 4);
    }

    SECTION("multiple lines")
    {
        fcn::Text const text("AB\nCD");
        REQUIRE(text.getNumberOfCharacters() == 6);
    }
}

TEST_CASE("Text getNumberOfCharacters for row", "[unit][text]")
{
    fcn::Text const text("AB\nCD\nEF");
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
        fcn::Text const text("Hello \U0001F44B");
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

// ============================================================================
// Additional coverage tests for text.cpp
// ============================================================================

TEST_CASE("Text string constructor edge cases", "[unit][text]")
{
    SECTION("empty string creates single empty row")
    {
        fcn::Text const text("");
        REQUIRE(text.getNumberOfRows() == 1);
        REQUIRE(text.getContent().empty());
    }

    SECTION("string ending with newline")
    {
        fcn::Text text("Line1\nLine2\n");
        REQUIRE(text.getNumberOfRows() == 2);
        REQUIRE(text.getRow(0) == "Line1");
        REQUIRE(text.getRow(1) == "Line2");
    }

    SECTION("string with only newline")
    {
        fcn::Text const text("\n");
        REQUIRE(text.getNumberOfRows() == 1);
        REQUIRE(text.getContent().empty());
    }

    SECTION("string with consecutive newlines")
    {
        fcn::Text text("A\n\nB");
        REQUIRE(text.getNumberOfRows() == 3);
        REQUIRE(text.getRow(0) == "A");
        REQUIRE(text.getRow(1).empty());
        REQUIRE(text.getRow(2) == "B");
    }

    SECTION("string with multiple trailing newlines")
    {
        fcn::Text text("A\n\n\n");
        REQUIRE(text.getNumberOfRows() == 3);
        REQUIRE(text.getRow(0) == "A");
        REQUIRE(text.getRow(1).empty());
        REQUIRE(text.getRow(2).empty());
    }
}

TEST_CASE("Text setContent edge cases", "[unit][text]")
{
    SECTION("setContent to empty string")
    {
        fcn::Text text("Original");
        text.setContent("");
        REQUIRE(text.getNumberOfRows() == 1);
        REQUIRE(text.getContent().empty());
    }

    SECTION("setContent resets caret position")
    {
        fcn::Text text("Line1\nLine2");
        text.setCaretPosition(10);
        text.setContent("New");
        REQUIRE(text.getCaretPosition() == 0);
        REQUIRE(text.getCaretRow() == 0);
        REQUIRE(text.getCaretColumn() == 0);
    }

    SECTION("setContent with string ending in newline")
    {
        fcn::Text text("Old");
        text.setContent("A\nB\n");
        REQUIRE(text.getNumberOfRows() == 2);
        REQUIRE(text.getRow(0) == "A");
        REQUIRE(text.getRow(1) == "B");
    }

    SECTION("setContent with only newlines")
    {
        fcn::Text text("Old");
        text.setContent("\n\n");
        REQUIRE(text.getNumberOfRows() == 2);
        REQUIRE(text.getRow(0).empty());
        REQUIRE(text.getRow(1).empty());
    }
}

TEST_CASE("Text row operations out of bounds", "[unit][text]")
{
    fcn::Text text("Line1\nLine2");

    SECTION("setRow out of bounds throws")
    {
        REQUIRE_THROWS_AS(text.setRow(5, "Bad"), fcn::Exception);
    }

    SECTION("getRow out of bounds throws")
    {
        REQUIRE_THROWS_AS(text.getRow(10), fcn::Exception);
    }

    SECTION("eraseRow out of bounds throws")
    {
        REQUIRE_THROWS_AS(text.eraseRow(100), fcn::Exception);
    }

    SECTION("insertRow position out of bounds throws")
    {
        REQUIRE_THROWS_AS(text.insertRow("Bad", 10), fcn::Exception);
    }

    SECTION("setRow on valid row succeeds")
    {
        text.setRow(0, "Modified");
        REQUIRE(text.getRow(0) == "Modified");
    }
}

TEST_CASE("Text insert into empty text", "[unit][text]")
{
    SECTION("insert character into empty text")
    {
        fcn::Text text;
        text.insert('A');
        REQUIRE(text.getContent() == "A");
        REQUIRE(text.getNumberOfRows() == 1);
        REQUIRE(text.getCaretPosition() == 1);
    }

    SECTION("insert newline into empty text")
    {
        fcn::Text text;
        text.insert('\n');
        REQUIRE(text.getNumberOfRows() == 2);
        REQUIRE(text.getRow(0).empty());
        REQUIRE(text.getRow(1).empty());
    }

    SECTION("insert multiple characters into empty text")
    {
        fcn::Text text;
        text.insert('H');
        text.insert('i');
        REQUIRE(text.getContent() == "Hi");
    }
}

TEST_CASE("Text remove edge cases", "[unit][text]")
{
    SECTION("remove zero characters does nothing")
    {
        fcn::Text text("ABC");
        text.setCaretPosition(2);
        text.remove(0);
        REQUIRE(text.getContent() == "ABC");
        REQUIRE(text.getCaretPosition() == 2);
    }

    SECTION("remove backward at position 0 does nothing")
    {
        fcn::Text text("ABC");
        text.setCaretPosition(0);
        text.remove(-5);
        REQUIRE(text.getContent() == "ABC");
        REQUIRE(text.getCaretPosition() == 0);
    }

    SECTION("remove forward at end of row does nothing")
    {
        fcn::Text text("ABC");
        text.setCaretPosition(3);
        text.remove(5);
        REQUIRE(text.getContent() == "ABC");
    }

    SECTION("remove forward merges rows at boundary")
    {
        fcn::Text text("Line1\nLine2\nLine3");
        text.setCaretRow(0);
        text.setCaretColumn(5); // end of "Line1"
        text.remove(1);         // merges Line1 with Line2
        REQUIRE(text.getNumberOfRows() == 2);
        REQUIRE(text.getRow(0) == "Line1Line2");
        REQUIRE(text.getRow(1) == "Line3");
    }

    SECTION("remove backward merges rows at start of row")
    {
        fcn::Text text("Line1\nLine2");
        text.setCaretPosition(6); // start of "Line2"
        text.remove(-1);          // removes newline
        REQUIRE(text.getNumberOfRows() == 1);
        REQUIRE(text.getContent() == "Line1Line2");
    }

    SECTION("remove backward across multiple rows")
    {
        fcn::Text text("A\nB\nC");
        text.setCaretPosition(4); // start of "C"
        text.remove(-2);          // merges C into B, then removes B's last char
        REQUIRE(text.getNumberOfRows() == 2);
        REQUIRE(text.getRow(0) == "A");
        REQUIRE(text.getRow(1) == "B");
    }

    SECTION("remove forward across multiple rows")
    {
        fcn::Text text("A\nB\nC");
        text.setCaretPosition(1); // end of "A"
        text.remove(2);           // removes newline and "B"
        REQUIRE(text.getNumberOfRows() == 2);
        REQUIRE(text.getRow(0) == "A");
        REQUIRE(text.getRow(1) == "C");
    }

    SECTION("remove on empty text does nothing")
    {
        fcn::Text text;
        text.remove(5);
        REQUIRE(text.getNumberOfRows() == 1);
        REQUIRE(text.getContent().empty());
    }

    SECTION("remove backward on empty text does nothing")
    {
        fcn::Text text;
        text.remove(-5);
        REQUIRE(text.getNumberOfRows() == 1);
        REQUIRE(text.getContent().empty());
    }
}

TEST_CASE("Text setCaretPosition edge cases", "[unit][text]")
{
    SECTION("negative position clamps to 0")
    {
        fcn::Text text("ABC");
        text.setCaretPosition(-10);
        REQUIRE(text.getCaretPosition() == 0);
        REQUIRE(text.getCaretRow() == 0);
        REQUIRE(text.getCaretColumn() == 0);
    }

    SECTION("position on empty text clamps to 0")
    {
        fcn::Text text;
        text.setCaretPosition(100);
        REQUIRE(text.getCaretPosition() == 0);
        REQUIRE(text.getCaretRow() == 0);
        REQUIRE(text.getCaretColumn() == 0);
    }

    SECTION("position at boundary between rows")
    {
        fcn::Text text("ABC\nDEF");
        text.setCaretPosition(3); // end of "ABC"
        REQUIRE(text.getCaretRow() == 1);
        REQUIRE(text.getCaretColumn() == 0);
        REQUIRE(text.getCaretPosition() == 3);
    }

    SECTION("position at end of last row")
    {
        fcn::Text text("ABC\nDEF");
        text.setCaretPosition(7); // end of "DEF" (3 + 1 + 3)
        REQUIRE(text.getCaretRow() == 1);
        REQUIRE(text.getCaretColumn() == 3);
        REQUIRE(text.getCaretPosition() == 7);
    }

    SECTION("position beyond content clamps to end")
    {
        fcn::Text text("ABC\nDEF");
        text.setCaretPosition(100);
        REQUIRE(text.getCaretRow() == 1);
        REQUIRE(text.getCaretColumn() == 3);
    }

    SECTION("position in middle of row")
    {
        fcn::Text text("ABC\nDEF");
        text.setCaretPosition(5); // middle of "DEF"
        REQUIRE(text.getCaretRow() == 1);
        REQUIRE(text.getCaretColumn() == 1);
        REQUIRE(text.getCaretPosition() == 5);
    }
}

TEST_CASE("Text setCaretColumn edge cases", "[unit][text]")
{
    SECTION("negative column clamps to 0")
    {
        fcn::Text text("ABC");
        text.setCaretColumn(-5);
        REQUIRE(text.getCaretColumn() == 0);
    }

    SECTION("column greater than row size clamps")
    {
        fcn::Text text("ABC");
        text.setCaretColumn(100);
        REQUIRE(text.getCaretColumn() == 3);
    }

    SECTION("column on empty text clamps to 0")
    {
        fcn::Text text;
        text.setCaretColumn(5);
        REQUIRE(text.getCaretColumn() == 0);
    }

    SECTION("valid column sets correctly")
    {
        fcn::Text text("ABC");
        text.setCaretColumn(2);
        REQUIRE(text.getCaretColumn() == 2);
    }
}

TEST_CASE("Text setCaretRow edge cases", "[unit][text]")
{
    SECTION("negative row clamps to 0")
    {
        fcn::Text text("A\nB\nC");
        text.setCaretRow(-5);
        REQUIRE(text.getCaretRow() == 0);
    }

    SECTION("row beyond last clamps to last")
    {
        fcn::Text text("A\nB\nC");
        text.setCaretRow(100);
        REQUIRE(text.getCaretRow() == 2);
    }

    SECTION("row on empty text clamps to 0")
    {
        fcn::Text text;
        text.setCaretRow(5);
        REQUIRE(text.getCaretRow() == 0);
    }

    SECTION("valid row sets correctly")
    {
        fcn::Text text("A\nB\nC");
        text.setCaretRow(1);
        REQUIRE(text.getCaretRow() == 1);
    }
}

TEST_CASE("Text font-dependent methods with empty text", "[unit][text]")
{
    fcn::DefaultFont font;
    fcn::Text text;

    SECTION("getCaretX on empty text returns 0")
    {
        REQUIRE(text.getCaretX(&font) == 0);
    }

    SECTION("getCaretY on empty text returns 0")
    {
        REQUIRE(text.getCaretY(&font) == 0);
    }

    SECTION("getDimension on empty text returns space dimension")
    {
        fcn::Rectangle const dim = text.getDimension(&font);
        REQUIRE(dim.width == font.getWidth(" "));
        REQUIRE(dim.height == font.getHeight());
    }

    SECTION("getCaretDimension on empty text")
    {
        fcn::Rectangle const dim = text.getCaretDimension(&font);
        REQUIRE(dim.x == 0);
        REQUIRE(dim.y == 0);
        REQUIRE(dim.width == font.getWidth(" "));
        REQUIRE(dim.height == font.getHeight() + 2);
    }

    SECTION("setCaretPosition with coordinates on empty text does nothing")
    {
        text.setCaretPosition(10, 10, &font);
        REQUIRE(text.getCaretPosition() == 0);
    }
}

TEST_CASE("Text font-dependent methods with content", "[unit][text]")
{
    fcn::DefaultFont font;
    fcn::Text text("ABC\nDE");

    SECTION("getCaretX calculates correctly")
    {
        text.setCaretPosition(2);
        REQUIRE(text.getCaretX(&font) == font.getWidth("AB"));
    }

    SECTION("getCaretY calculates correctly")
    {
        text.setCaretPosition(5); // in second row
        REQUIRE(text.getCaretY(&font) == font.getHeight());
    }

    SECTION("getDimension calculates correctly")
    {
        fcn::Rectangle const dim = text.getDimension(&font);
        // Width is max row width + space width
        int const maxWidth = std::max(font.getWidth("ABC"), font.getWidth("DE"));
        REQUIRE(dim.width == maxWidth + font.getWidth(" "));
        // Height is font height * number of rows
        REQUIRE(dim.height == font.getHeight() * 2);
    }

    SECTION("getCaretDimension calculates correctly")
    {
        text.setCaretPosition(2);
        fcn::Rectangle const dim = text.getCaretDimension(&font);
        REQUIRE(dim.x == font.getWidth("AB"));
        REQUIRE(dim.y == 0);
        REQUIRE(dim.width == font.getWidth(" "));
        REQUIRE(dim.height == font.getHeight() + 2);
    }

    SECTION("setCaretPosition with coordinates")
    {
        text.setCaretPosition(16, 8, &font); // x=16 -> col 2, y=8 -> row 1
        REQUIRE(text.getCaretRow() == 1);
        REQUIRE(text.getCaretColumn() == 2);
    }

    SECTION("getCaretY for second row")
    {
        text.setCaretRow(1);
        REQUIRE(text.getCaretY(&font) == font.getHeight());
    }
}

TEST_CASE("Text getWidth and getMaximumCaretRow", "[unit][text]")
{
    fcn::DefaultFont font;
    fcn::Text const text("ABC\nDE");

    SECTION("getWidth always returns 0")
    {
        REQUIRE(text.getWidth(0, &font) == 0);
        REQUIRE(text.getWidth(1, &font) == 0);
        REQUIRE(text.getWidth(99, &font) == 0);
    }

    SECTION("getMaximumCaretRow always returns 0")
    {
        REQUIRE(text.getMaximumCaretRow() == 0);
    }

    SECTION("getMaximumCaretRow with row parameter always returns 0")
    {
        REQUIRE(text.getMaximumCaretRow(0) == 0);
        REQUIRE(text.getMaximumCaretRow(5) == 0);
    }
}

TEST_CASE("Text getNumberOfCharacters edge cases", "[unit][text]")
{
    SECTION("empty text returns 1 (for the implicit newline)")
    {
        fcn::Text const text;
        REQUIRE(text.getNumberOfCharacters() == 1);
    }

    SECTION("single character returns 2")
    {
        fcn::Text const text("A");
        REQUIRE(text.getNumberOfCharacters() == 2);
    }

    SECTION("getNumberOfCharacters for out of bounds row returns 0")
    {
        fcn::Text const text("ABC");
        REQUIRE(text.getNumberOfCharacters(100) == 0);
    }

    SECTION("getNumberOfCharacters for empty row returns 0")
    {
        fcn::Text const text("A\n\nB");
        REQUIRE(text.getNumberOfCharacters(1) == 0);
    }
}

TEST_CASE("Text insert at various positions", "[unit][text]")
{
    SECTION("insert at end of row")
    {
        fcn::Text text("ABC");
        text.setCaretPosition(3);
        text.insert('D');
        REQUIRE(text.getContent() == "ABCD");
    }

    SECTION("insert in middle of row")
    {
        fcn::Text text("AC");
        text.setCaretPosition(1);
        text.insert('B');
        REQUIRE(text.getContent() == "ABC");
    }

    SECTION("insert newline at beginning of row")
    {
        fcn::Text text("Line1\nLine2");
        text.setCaretPosition(0);
        text.insert('\n');
        REQUIRE(text.getNumberOfRows() == 3);
        REQUIRE(text.getRow(0).empty());
        REQUIRE(text.getRow(1) == "Line1");
        REQUIRE(text.getRow(2) == "Line2");
    }

    SECTION("insert newline at end of row")
    {
        fcn::Text text("Line1\nLine2");
        text.setCaretPosition(5);
        text.insert('\n');
        REQUIRE(text.getNumberOfRows() == 3);
        REQUIRE(text.getRow(0) == "Line1");
        REQUIRE(text.getRow(1).empty());
        REQUIRE(text.getRow(2) == "Line2");
    }

    SECTION("insert newline in middle of row")
    {
        fcn::Text text("ABCDE");
        text.setCaretPosition(2);
        text.insert('\n');
        REQUIRE(text.getNumberOfRows() == 2);
        REQUIRE(text.getRow(0) == "AB");
        REQUIRE(text.getRow(1) == "CDE");
    }
}

TEST_CASE("Text special characters and edge cases", "[unit][text]")
{
    SECTION("text with spaces")
    {
        fcn::Text const text("Hello World");
        REQUIRE(text.getContent() == "Hello World");
        REQUIRE(text.getNumberOfRows() == 1);
    }

    SECTION("text with tabs")
    {
        fcn::Text const text("A\tB");
        REQUIRE(text.getContent() == "A\tB");
    }

    SECTION("very long single line")
    {
        std::string longStr(10000, 'A');
        fcn::Text const text(longStr);
        REQUIRE(text.getNumberOfRows() == 1);
        REQUIRE(text.getContent() == longStr);
    }

    SECTION("many rows")
    {
        std::string multiRow;
        for (int i = 0; i < 100; ++i) {
            multiRow += "Row" + std::to_string(i) + "\n";
        }
        fcn::Text const text(multiRow);
        REQUIRE(text.getNumberOfRows() == 100);
    }

    SECTION("setContent preserves empty rows between content")
    {
        fcn::Text text("A\n\n\nB");
        REQUIRE(text.getNumberOfRows() == 4);
        REQUIRE(text.getRow(0) == "A");
        REQUIRE(text.getRow(1).empty());
        REQUIRE(text.getRow(2).empty());
        REQUIRE(text.getRow(3) == "B");
    }
}

TEST_CASE("Text copy and move operations", "[unit][text]")
{
    SECTION("copy constructor")
    {
        fcn::Text original("ABC\nDEF");
        original.setCaretPosition(4);
        fcn::Text const copy(original);
        REQUIRE(copy.getContent() == "ABC\nDEF");
        REQUIRE(copy.getCaretPosition() == 4);
    }

    SECTION("copy assignment")
    {
        fcn::Text original("ABC\nDEF");
        original.setCaretPosition(5);
        fcn::Text copy;
        copy = original;
        REQUIRE(copy.getContent() == "ABC\nDEF");
        REQUIRE(copy.getCaretPosition() == 5);
    }

    SECTION("move constructor")
    {
        fcn::Text original("ABC\nDEF");
        original.setCaretPosition(3);
        fcn::Text const moved(std::move(original));
        REQUIRE(moved.getContent() == "ABC\nDEF");
        REQUIRE(moved.getCaretPosition() == 3);
    }

    SECTION("move assignment")
    {
        fcn::Text original("ABC\nDEF");
        original.setCaretPosition(6);
        fcn::Text moved;
        moved = std::move(original);
        REQUIRE(moved.getContent() == "ABC\nDEF");
        REQUIRE(moved.getCaretPosition() == 6);
    }
}

TEST_CASE("Text calculateCaretPositionFromRowAndColumn", "[unit][text]")
{
    SECTION("first row column 0")
    {
        fcn::Text text("ABC\nDEF");
        text.setCaretRow(0);
        text.setCaretColumn(0);
        REQUIRE(text.getCaretPosition() == 0);
    }

    SECTION("first row column 2")
    {
        fcn::Text text("ABC\nDEF");
        text.setCaretRow(0);
        text.setCaretColumn(2);
        REQUIRE(text.getCaretPosition() == 2);
    }

    SECTION("second row column 0")
    {
        fcn::Text text("ABC\nDEF");
        text.setCaretRow(1);
        text.setCaretColumn(0);
        REQUIRE(text.getCaretPosition() == 4);
    }

    SECTION("second row column 3")
    {
        fcn::Text text("ABC\nDEF");
        text.setCaretRow(1);
        text.setCaretColumn(3);
        REQUIRE(text.getCaretPosition() == 7);
    }

    SECTION("third row after multiple newlines")
    {
        fcn::Text text("A\nB\nC");
        text.setCaretRow(2);
        text.setCaretColumn(0);
        REQUIRE(text.getCaretPosition() == 4);
    }
}

TEST_CASE("Text eraseRow edge cases", "[unit][text]")
{
    SECTION("erase first row")
    {
        fcn::Text text("A\nB\nC");
        text.eraseRow(0);
        REQUIRE(text.getNumberOfRows() == 2);
        REQUIRE(text.getRow(0) == "B");
        REQUIRE(text.getRow(1) == "C");
    }

    SECTION("erase last row")
    {
        fcn::Text text("A\nB\nC");
        text.eraseRow(2);
        REQUIRE(text.getNumberOfRows() == 2);
        REQUIRE(text.getRow(0) == "A");
        REQUIRE(text.getRow(1) == "B");
    }

    SECTION("erase middle row")
    {
        fcn::Text text("A\nB\nC");
        text.eraseRow(1);
        REQUIRE(text.getNumberOfRows() == 2);
        REQUIRE(text.getRow(0) == "A");
        REQUIRE(text.getRow(1) == "C");
    }

    SECTION("erase row from single row text")
    {
        fcn::Text text("ABC");
        text.eraseRow(0);
        REQUIRE(text.getNumberOfRows() == 0);
    }

    SECTION("erase row 0 from empty text throws")
    {
        fcn::Text text;
        text.eraseRow(0);
        REQUIRE(text.getNumberOfRows() == 0);
    }
}

TEST_CASE("Text getContent with various row states", "[unit][text]")
{
    SECTION("getContent after erasing all rows")
    {
        fcn::Text text("ABC");
        text.eraseRow(0);
        REQUIRE(text.getContent().empty());
    }

    SECTION("getContent with empty rows")
    {
        fcn::Text const text("A\n\nB");
        REQUIRE(text.getContent() == "A\n\nB");
    }

    SECTION("getContent after adding empty row")
    {
        fcn::Text text("A");
        text.addRow("");
        REQUIRE(text.getContent() == "A\n");
    }
}
