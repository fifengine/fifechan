// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2013 - 2026 Fifengine contributors

// Standard library includes
#include <stdexcept>
#include <string>
#include <utility>

// Third-party library includes
#include <catch2/catch_test_macros.hpp>

// Project headers (subdirs before local)
#include "fifechan/events/keyevent.hpp"
#include "fifechan/events/textinputevent.hpp"
#include "fifechan/key.hpp"
#include "fifechan/widgets/textbox.hpp"

namespace
{
    /// Helper that creates a pressed KeyEvent.
    fcn::KeyEvent makeKey(fcn::TextBox& tb, int keyValue)
    {
        return {&tb, nullptr, false, false, false, false, fcn::KeyEvent::Type::Pressed, false, fcn::Key(keyValue)};
    }
} // namespace

TEST_CASE("TextBox constructors initialize properly", "[unit][textbox]")
{
    SECTION("default constructor creates empty textbox")
    {
        fcn::TextBox const textBox;
        REQUIRE(textBox.getText().empty());
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
    fcn::TextBox const textBox("ABC");
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

    SECTION("setCaretPosition on empty textbox")
    {
        fcn::TextBox empty;
        empty.setCaretPosition(0);
        REQUIRE(empty.getCaretPosition() == 0);
    }
}

TEST_CASE("TextBox getTextRow validates row index", "[unit][textbox]")
{
    fcn::TextBox const textBox("Line1\nLine2\nLine3");
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

    SECTION("set empty string")
    {
        textBox.setText("");
        REQUIRE(textBox.getText().empty());
        REQUIRE(textBox.getNumberOfRows() == 1);
    }

    SECTION("setText with multi-line")
    {
        textBox.setText("A\nB\nC");
        REQUIRE(textBox.getNumberOfRows() == 3);
        REQUIRE(textBox.getTextRow(0) == "A");
        REQUIRE(textBox.getTextRow(1) == "B");
        REQUIRE(textBox.getTextRow(2) == "C");
    }
}

TEST_CASE("TextBox UTF8 caret operations", "[unit][textbox]")
{
    fcn::TextBox const textBox("Héllo");
    REQUIRE(textBox.getCaretColumn() == 0);

    SECTION("UTF8 character counts correctly")
    {
        REQUIRE(textBox.getText().size() == 6);
    }
}

TEST_CASE("TextBox row UTF8 operations", "[unit][textbox]")
{
    fcn::TextBox const textBox("Héllo\nWörld");

    SECTION("getTextRow returns correct UTF8 text")
    {
        REQUIRE(textBox.getTextRow(0) == "Héllo");
        REQUIRE(textBox.getTextRow(1) == "Wörld");
    }
}

TEST_CASE("TextBox keyPressed LEFT", "[unit][textbox]")
{
    fcn::TextBox tb("Line1\nLine2\nLine3");

    SECTION("moves caret left within row")
    {
        tb.setCaretColumn(3);
        auto ke = makeKey(tb, fcn::Key::LEFT);
        tb.keyPressed(ke);
        REQUIRE(tb.getCaretColumn() == 2);
    }

    SECTION("at column 0 wraps to previous row end")
    {
        tb.setCaretRowColumn(1, 0);
        auto ke = makeKey(tb, fcn::Key::LEFT);
        tb.keyPressed(ke);
        REQUIRE(tb.getCaretRow() == 0);
        REQUIRE(tb.getCaretColumn() == 5);
    }

    SECTION("at top-left does nothing")
    {
        auto ke = makeKey(tb, fcn::Key::LEFT);
        tb.keyPressed(ke);
        REQUIRE(tb.getCaretRow() == 0);
        REQUIRE(tb.getCaretColumn() == 0);
    }
}

TEST_CASE("TextBox keyPressed RIGHT", "[unit][textbox]")
{
    fcn::TextBox tb("Line1\nLine2\nLine3");

    SECTION("moves caret right within row")
    {
        tb.setCaretColumn(2);
        auto ke = makeKey(tb, fcn::Key::RIGHT);
        tb.keyPressed(ke);
        REQUIRE(tb.getCaretColumn() == 3);
    }

    SECTION("at end of row moves to next row start")
    {
        tb.setCaretRowColumn(0, 5);
        auto ke = makeKey(tb, fcn::Key::RIGHT);
        tb.keyPressed(ke);
        REQUIRE(tb.getCaretRow() == 1);
        REQUIRE(tb.getCaretColumn() == 0);
    }

    SECTION("at end of last row does nothing")
    {
        tb.setCaretRowColumn(2, 5);
        auto ke = makeKey(tb, fcn::Key::RIGHT);
        tb.keyPressed(ke);
        REQUIRE(tb.getCaretRow() == 2);
        REQUIRE(tb.getCaretColumn() == 5);
    }
}

TEST_CASE("TextBox keyPressed UP/DOWN", "[unit][textbox]")
{
    fcn::TextBox tb("Line1\nLine2\nLine3");

    SECTION("DOWN moves to next row preserving column")
    {
        tb.setCaretRowColumn(0, 2);
        auto ke = makeKey(tb, fcn::Key::DOWN);
        tb.keyPressed(ke);
        REQUIRE(tb.getCaretRow() == 1);
        REQUIRE(tb.getCaretColumn() == 2);
    }

    SECTION("DOWN at last row stays")
    {
        tb.setCaretRowColumn(2, 2);
        auto ke = makeKey(tb, fcn::Key::DOWN);
        tb.keyPressed(ke);
        REQUIRE(tb.getCaretRow() == 2);
    }

    SECTION("UP moves to previous row preserving column")
    {
        tb.setCaretRowColumn(1, 3);
        auto ke = makeKey(tb, fcn::Key::UP);
        tb.keyPressed(ke);
        REQUIRE(tb.getCaretRow() == 0);
        REQUIRE(tb.getCaretColumn() == 3);
    }

    SECTION("UP at first row stays")
    {
        auto ke = makeKey(tb, fcn::Key::UP);
        tb.keyPressed(ke);
        REQUIRE(tb.getCaretRow() == 0);
    }
}

TEST_CASE("TextBox keyPressed HOME/END", "[unit][textbox]")
{
    fcn::TextBox tb("Line1\nLine2\nLine3");

    SECTION("HOME moves to column 0")
    {
        tb.setCaretColumn(3);
        auto ke = makeKey(tb, fcn::Key::HOME);
        tb.keyPressed(ke);
        REQUIRE(tb.getCaretColumn() == 0);
    }

    SECTION("END moves to end of row")
    {
        tb.setCaretRowColumn(1, 0);
        auto ke = makeKey(tb, fcn::Key::END);
        tb.keyPressed(ke);
        REQUIRE(tb.getCaretColumn() == 5);
    }
}

TEST_CASE("TextBox keyPressed RETURN inserts new row", "[unit][textbox]")
{
    fcn::TextBox tb("Line1\nLine2\nLine3");

    SECTION("splits current row at caret")
    {
        // Row 1 is "Line2", caret at column 2 = between 'n' and 'e'
        tb.setCaretRowColumn(1, 2);
        auto ke = makeKey(tb, fcn::Key::KEY_RETURN);
        tb.keyPressed(ke);
        REQUIRE(tb.getNumberOfRows() == 4);
        REQUIRE(tb.getTextRow(1) == "Li");   // text before caret
        REQUIRE(tb.getTextRow(2) == "ne2");  // text after caret
        REQUIRE(tb.getCaretRow() == 2);
        REQUIRE(tb.getCaretColumn() == 0);
    }

    SECTION("non-editable ignores RETURN")
    {
        tb.setEditable(false);
        unsigned const rowsBefore = tb.getNumberOfRows();
        auto ke = makeKey(tb, fcn::Key::KEY_RETURN);
        tb.keyPressed(ke);
        REQUIRE(tb.getNumberOfRows() == rowsBefore);
    }
}

TEST_CASE("TextBox keyPressed BACKSPACE", "[unit][textbox]")
{
    fcn::TextBox tb("Line1\nLine2\nLine3");

    SECTION("deletes character before caret within row")
    {
        // "Line1", caret at column 2 (between 'i' and 'n')
        tb.setCaretRowColumn(0, 2);
        auto ke = makeKey(tb, fcn::Key::BACKSPACE);
        tb.keyPressed(ke);
        REQUIRE(tb.getTextRow(0) == "Lne1");
        REQUIRE(tb.getCaretColumn() == 1);
    }

    SECTION("at start of row merges with previous row")
    {
        tb.setCaretRowColumn(1, 0);
        auto ke = makeKey(tb, fcn::Key::BACKSPACE);
        tb.keyPressed(ke);
        REQUIRE(tb.getNumberOfRows() == 2);
        REQUIRE(tb.getTextRow(0) == "Line1Line2");
        REQUIRE(tb.getCaretRow() == 0);
        REQUIRE(tb.getCaretColumn() == 5);
    }

    SECTION("non-editable ignores BACKSPACE")
    {
        tb.setEditable(false);
        std::string const saved = tb.getText();
        auto ke = makeKey(tb, fcn::Key::BACKSPACE);
        tb.keyPressed(ke);
        REQUIRE(tb.getText() == saved);
    }
}

TEST_CASE("TextBox keyPressed DELETE", "[unit][textbox]")
{
    fcn::TextBox tb("Line1\nLine2\nLine3");

    SECTION("deletes character after caret within row")
    {
        tb.setCaretRowColumn(0, 2);
        auto ke = makeKey(tb, fcn::Key::KEY_DELETE);
        tb.keyPressed(ke);
        REQUIRE(tb.getTextRow(0) == "Lie1");
        REQUIRE(tb.getCaretColumn() == 2);
    }

    SECTION("at end of row merges with next row")
    {
        tb.setCaretRowColumn(0, 5);
        auto ke = makeKey(tb, fcn::Key::KEY_DELETE);
        tb.keyPressed(ke);
        REQUIRE(tb.getNumberOfRows() == 2);
        REQUIRE(tb.getTextRow(0) == "Line1Line2");
    }

    SECTION("non-editable ignores DELETE")
    {
        tb.setEditable(false);
        std::string const saved = tb.getText();
        auto ke = makeKey(tb, fcn::Key::KEY_DELETE);
        tb.keyPressed(ke);
        REQUIRE(tb.getText() == saved);
    }
}

TEST_CASE("TextBox keyPressed TAB inserts spaces", "[unit][textbox]")
{
    fcn::TextBox tb("abc");
    tb.setCaretColumn(1);

    auto ke = makeKey(tb, fcn::Key::TAB);
    tb.keyPressed(ke);
    REQUIRE(tb.getTextRow(0) == "a   bc");
    REQUIRE(tb.getCaretColumn() == 4);
}

TEST_CASE("TextBox keyPressed non-editable ignores all editing keys", "[unit][textbox]")
{
    fcn::TextBox tb("hello");
    tb.setEditable(false);
    std::string const saved = tb.getText();

    {
        auto ke = makeKey(tb, fcn::Key::KEY_RETURN);
        tb.keyPressed(ke);
    }
    {
        auto ke = makeKey(tb, fcn::Key::BACKSPACE);
        tb.keyPressed(ke);
    }
    {
        auto ke = makeKey(tb, fcn::Key::KEY_DELETE);
        tb.keyPressed(ke);
    }
    {
        auto ke = makeKey(tb, fcn::Key::TAB);
        tb.keyPressed(ke);
    }
    REQUIRE(tb.getText() == saved);
}

TEST_CASE("TextBox textInput inserts text", "[unit][textbox]")
{
    fcn::TextBox tb("ab");
    tb.setCaretColumn(1);

    fcn::TextInputEvent tie(&tb, nullptr, false, false, false, false, "XYZ");
    tb.textInput(tie);

    REQUIRE(tb.getTextRow(0) == "aXYZb");
    REQUIRE(tb.getCaretColumn() == 4);
}

TEST_CASE("TextBox addRow edge cases", "[unit][textbox]")
{
    fcn::TextBox tb("Line1");

    SECTION("addRow empty string")
    {
        tb.addRow("");
        REQUIRE(tb.getNumberOfRows() == 2);
        REQUIRE(tb.getTextRow(1).empty());
    }
}

TEST_CASE("TextBox setTextRow empty string", "[unit][textbox]")
{
    fcn::TextBox tb("Line1\nLine2");
    tb.setTextRow(1, "");
    REQUIRE(tb.getTextRow(1).empty());
}

TEST_CASE("TextBox adjustSizeImpl isFixedSize", "[unit][textbox]")
{
    fcn::TextBox tb("long text");
    int const w = tb.getWidth();
    int const h = tb.getHeight();

    tb.setSize(200, 50);

    SECTION("without fixed size, adjustSize resizes to content")
    {
        tb.adjustSize();
        REQUIRE(tb.getWidth() < 200);
        REQUIRE(tb.getHeight() < 50);
    }

    SECTION("with fixed size, adjustSize skips resize")
    {
        tb.setSize(200, 50);
        tb.setFixedSize(fcn::Size(200, 50));
        auto ke = makeKey(tb, fcn::Key::RIGHT);
        tb.keyPressed(ke);

        REQUIRE(tb.getWidth() == 200);
        REQUIRE(tb.getHeight() == 50);
    }
}

TEST_CASE("TextBox adjustSizeImpl expand flags", "[unit][textbox]")
{
    fcn::TextBox tb("content");

    tb.setSize(200, 50);

    SECTION("horizontal expand keeps width")
    {
        tb.setHorizontalExpand(true);
        tb.adjustSize();
        REQUIRE(tb.getWidth() == 200);
        REQUIRE(tb.getHeight() < 50);
    }

    SECTION("vertical expand keeps height")
    {
        tb.setVerticalExpand(true);
        tb.adjustSize();
        REQUIRE(tb.getWidth() < 200);
        REQUIRE(tb.getHeight() == 50);
    }
}

TEST_CASE("TextBox setCaretRowUTF8 preserves character position", "[unit][textbox]")
{
    fcn::TextBox tb("Héllo\nWörld");
    tb.setCaretRowColumn(0, 1);

    auto down = makeKey(tb, fcn::Key::DOWN);
    tb.keyPressed(down);
    REQUIRE(tb.getCaretRow() == 1);
    REQUIRE(tb.getCaretColumn() == 1);

    auto up = makeKey(tb, fcn::Key::UP);
    tb.keyPressed(up);
    REQUIRE(tb.getCaretRow() == 0);
    REQUIRE(tb.getCaretColumn() == 1);
}

TEST_CASE("TextBox setCaretRowUTF8 clamps row bounds", "[unit][textbox]")
{
    fcn::TextBox tb("a\nb\nc");

    SECTION("DOWN past last row clamps")
    {
        tb.setCaretRowColumn(2, 0);
        auto ke = makeKey(tb, fcn::Key::DOWN);
        tb.keyPressed(ke);
        REQUIRE(tb.getCaretRow() == 2);
    }

    SECTION("UP past first row clamps")
    {
        auto ke = makeKey(tb, fcn::Key::UP);
        tb.keyPressed(ke);
        REQUIRE(tb.getCaretRow() == 0);
    }
}

TEST_CASE("TextBox caret row clamp to shorter row", "[unit][textbox]")
{
    fcn::TextBox tb("abc\nde\nfghij");

    SECTION("DOWN to shorter row clamps column")
    {
        tb.setCaretRowColumn(0, 2);
        auto ke = makeKey(tb, fcn::Key::DOWN);
        tb.keyPressed(ke);
        REQUIRE(tb.getCaretRow() == 1);
        REQUIRE(tb.getCaretColumn() == 2);
    }

    SECTION("UP to shorter row clamps column")
    {
        tb.setCaretRowColumn(2, 4);
        auto ke = makeKey(tb, fcn::Key::UP);
        tb.keyPressed(ke);
        REQUIRE(tb.getCaretRow() == 1);
        REQUIRE(tb.getCaretColumn() == 2);
    }
}

TEST_CASE("TextBox getText returns correct content after edits", "[unit][textbox]")
{
    fcn::TextBox tb("A\nB");
    REQUIRE(tb.getText() == "A\nB");

    tb.setText("X\nY\nZ");
    REQUIRE(tb.getText() == "X\nY\nZ");
}
