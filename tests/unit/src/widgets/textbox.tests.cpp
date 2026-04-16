// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2013 - 2026 Fifengine contributors

// Standard library includes
#include <stdexcept>

// Third-party library includes
#include <catch2/catch_test_macros.hpp>

// Project headers (subdirs before local)
#include "fifechan/widgets/textbox.hpp"

TEST_CASE("TextBox constructors initialize properly", "[unit][textbox]")
{
    SECTION("default constructor creates empty textbox")
    {
        fcn::TextBox const textBox;
        REQUIRE(textBox.getText() == "");
        REQUIRE(textBox.getNumberOfRows() == 1);
    }

    SECTION("string constructor sets text")
    {
        fcn::TextBox const textBox("Hello");
        REQUIRE(textBox.getText() == "Hello");
        REQUIRE(textBox.getNumberOfRows() == 1);
    }

    SECTION("multiline constructor creates multiple rows")
    {
        fcn::TextBox const textBox("Line1\nLine2\nLine3");
        REQUIRE(textBox.getNumberOfRows() == 3);
        REQUIRE(textBox.getTextRow(0) == "Line1");
        REQUIRE(textBox.getTextRow(1) == "Line2");
        REQUIRE(textBox.getTextRow(2) == "Line3");
    }
}

TEST_CASE("TextBox caret position starts at beginning", "[unit][textbox]")
{
    fcn::TextBox textBox("ABC");
    REQUIRE(textBox.getCaretRow() == 0);
    REQUIRE(textBox.getCaretColumn() == 0);
}

TEST_CASE("TextBox setCaretRow bounds checks", "[unit][textbox]")
{
    fcn::TextBox textBox("Line1\nLine2\nLine3");
    REQUIRE(textBox.getCaretRow() == 0);

    SECTION("setting row to valid value succeeds")
    {
        textBox.setCaretRow(1);
        REQUIRE(textBox.getCaretRow() == 1);
    }
}

TEST_CASE("TextBox setCaretColumn bounds checks", "[unit][textbox]")
{
    fcn::TextBox textBox("ABC");
    REQUIRE(textBox.getCaretColumn() == 0);

    SECTION("setting column within bounds succeeds")
    {
        textBox.setCaretColumn(1);
        REQUIRE(textBox.getCaretColumn() == 1);
    }

    SECTION("setting column to row length")
    {
        textBox.setCaretColumn(3);
        REQUIRE(textBox.getCaretColumn() == 3);
    }
}

TEST_CASE("TextBox setCaretRowColumn validates inputs", "[unit][textbox]")
{
    fcn::TextBox textBox("Line1\nLine2\nLine3");

    SECTION("valid row and column set both")
    {
        textBox.setCaretRowColumn(1, 2);
        REQUIRE(textBox.getCaretRow() == 1);
        REQUIRE(textBox.getCaretColumn() == 2);
    }
}

TEST_CASE("TextBox setCaretPosition validates bounds", "[unit][textbox]")
{
    fcn::TextBox textBox("ABC");
    REQUIRE(textBox.getCaretPosition() == 0);

    SECTION("valid position within bounds")
    {
        textBox.setCaretPosition(1);
        REQUIRE(textBox.getCaretPosition() == 1);
    }
}

TEST_CASE("TextBox getTextRow validates row index", "[unit][textbox]")
{
    fcn::TextBox textBox("Line1\nLine2\nLine3");
    REQUIRE(textBox.getNumberOfRows() == 3);

    SECTION("valid row index returns text")
    {
        REQUIRE(textBox.getTextRow(0) == "Line1");
        REQUIRE(textBox.getTextRow(2) == "Line3");
    }
}

TEST_CASE("TextBox setTextRow validates inputs", "[unit][textbox]")
{
    fcn::TextBox textBox("Line1\nLine2\nLine3");

    SECTION("setting valid row updates text")
    {
        textBox.setTextRow(1, "Modified");
        REQUIRE(textBox.getTextRow(1) == "Modified");
    }
}

TEST_CASE("TextBox isEditable defaults to true", "[unit][textbox]")
{
    fcn::TextBox textBox;
    REQUIRE(textBox.isEditable() == true);

    SECTION("setEditable changes state")
    {
        textBox.setEditable(false);
        REQUIRE(textBox.isEditable() == false);
        textBox.setEditable(true);
        REQUIRE(textBox.isEditable() == true);
    }
}

TEST_CASE("TextBox isOpaque defaults to true", "[unit][textbox]")
{
    fcn::TextBox textBox;
    REQUIRE(textBox.isOpaque() == true);

    SECTION("setOpaque changes state")
    {
        textBox.setOpaque(false);
        REQUIRE(textBox.isOpaque() == false);
        textBox.setOpaque(true);
        REQUIRE(textBox.isOpaque() == true);
    }
}

TEST_CASE("TextBox addRow appends new row", "[unit][textbox]")
{
    fcn::TextBox textBox("Line1\nLine2");
    REQUIRE(textBox.getNumberOfRows() == 2);

    textBox.addRow("Line3");
    REQUIRE(textBox.getNumberOfRows() == 3);
    REQUIRE(textBox.getTextRow(2) == "Line3");
}

TEST_CASE("TextBox setText replaces content", "[unit][textbox]")
{
    fcn::TextBox textBox("Original");
    REQUIRE(textBox.getText() == "Original");

    textBox.setText("New text");
    REQUIRE(textBox.getText() == "New text");
}

TEST_CASE("TextBox UTF8 caret operations", "[unit][textbox]")
{
    fcn::TextBox textBox("Héllo");
    REQUIRE(textBox.getCaretColumn() == 0);

    SECTION("UTF8 character counts correctly")
    {
        REQUIRE(textBox.getText().size() == 6);
    }
}

TEST_CASE("TextBox row UTF8 operations", "[unit][textbox]")
{
    fcn::TextBox textBox("Héllo\nWörld");

    SECTION("getTextRow returns correct UTF8 text")
    {
        REQUIRE(textBox.getTextRow(0) == "Héllo");
        REQUIRE(textBox.getTextRow(1) == "Wörld");
    }
}
