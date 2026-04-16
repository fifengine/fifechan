// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2026 Fifengine contributors

// Corresponding header include
#include <fifechan/widgets/textbox.hpp>

// Standard library includes
#include <cassert>
#include <string>

// Third-party library includes
#include <utf8cpp/utf8.h>

// Project headers (subdirs before local)
#include "fifechan/font.hpp"
#include "fifechan/graphics.hpp"
#include "fifechan/key.hpp"
#include "fifechan/mouseinput.hpp"
#include "fifechan/text.hpp"
#include "fifechan/utf8stringeditor.hpp"

namespace fcn
{
    TextBox::TextBox(std::string const & text) : mText(new Text(text)), mStringEditor(new UTF8StringEditor)
    {
        assert("mText is not null" && mText != nullptr);
        assert("mStringEditor is not null" && mStringEditor != nullptr);

        setFocusable(true);

        addMouseListener(this);
        addKeyListener(this);
        adjustSizeImpl();
    }

    TextBox::~TextBox()
    {
        delete mText;
        delete mStringEditor;
    }

    void TextBox::setText(std::string const & text)
    {
        assert("text is valid utf8" && utf8::is_valid(text.begin(), text.end()));

        mText->setContent(text);
        adjustSizeImpl();
    }

    void TextBox::draw(Graphics* graphics)
    {
        assert("font is not null" && getFont() != nullptr);

        if (mOpaque) {
            graphics->setColor(getBackgroundColor());
            graphics->fillRectangle(0, 0, getWidth(), getHeight());
        }

        if (isFocused() && isEditable()) {
            drawCaret(graphics, mText->getCaretX(getFont()), mText->getCaretY(getFont()));
        }

        graphics->setColor(getForegroundColor());
        graphics->setFont(getFont());

        unsigned int i = 0;
        for (i = 0; i < mText->getNumberOfRows(); i++) {
            // Move the text one pixel so we can have a caret before a letter.
            graphics->drawText(mText->getRow(i), 1, i * getFont()->getHeight());
        }
    }

    void TextBox::drawCaret(Graphics* graphics, int x, int y)
    {
        assert("font is not null" && getFont() != nullptr);

        graphics->setColor(getForegroundColor());
        graphics->drawLine(x, y, x, y + getFont()->getHeight());
    }

    void TextBox::mousePressed(MouseEvent& mouseEvent)
    {
        assert("font is not null" && getFont() != nullptr);

        if (mouseEvent.getButton() == MouseEvent::Button::Left) {
            mText->setCaretPosition(mouseEvent.getX(), mouseEvent.getY(), getFont());
            mouseEvent.consume();
        }
    }

    void TextBox::mouseDragged(MouseEvent& mouseEvent)
    {
        mouseEvent.consume();
    }

    void TextBox::keyPressed(KeyEvent& keyEvent)
    {
        Key const key = keyEvent.getKey();

        if (key.getValue() == Key::Left) {
            // Move caret left, or to end of previous row if at column 0
            if (getCaretColumn() == 0) {
                if (getCaretRow() > 0) {
                    setCaretRow(getCaretRow() - 1);
                    setCaretColumn(getTextRow(getCaretRow()).size());
                }
            } else {
                setCaretColumn(fcn::UTF8StringEditor::prevChar(getTextRow(getCaretRow()), getCaretColumn()));
            }
        } else if (key.getValue() == Key::Right) {
            // Move caret right, or to start of next row if at end of line
            if (getCaretColumn() < getTextRow(getCaretRow()).size()) {
                setCaretColumn(fcn::UTF8StringEditor::nextChar(getTextRow(getCaretRow()), getCaretColumn()));
            } else {
                if (getCaretRow() < getNumberOfRows() - 1) {
                    setCaretRow(getCaretRow() + 1);
                    setCaretColumn(0);
                }
            }
        } else if (key.getValue() == Key::Down) {
            setCaretRowUTF8(getCaretRow() + 1);
        } else if (key.getValue() == Key::Up) {
            setCaretRowUTF8(getCaretRow() - 1);
        } else if (key.getValue() == Key::Home) {
            setCaretColumn(0);
        } else if (key.getValue() == Key::End) {
            setCaretColumn(getTextRow(getCaretRow()).size());
        } else if (key.getValue() == Key::Enter && mEditable) {
            // Split current row at caret: text after caret becomes new row
            mText->insertRow(
                getTextRow(getCaretRow()).substr(getCaretColumn(), getTextRow(getCaretRow()).size() - getCaretColumn()),
                getCaretRow() + 1);

            mText->getRow(getCaretRow()).resize(getCaretColumn());
            setCaretRow(getCaretRow() + 1);
            setCaretColumn(0);
        } else if (key.getValue() == Key::Backspace && getCaretColumn() != 0 && mEditable) {
            // Delete character before caret within current row
            std::string& currRow = mText->getRow(getCaretRow());
            setCaretColumn(fcn::UTF8StringEditor::prevChar(currRow, static_cast<int>(getCaretColumn())));
            setCaretColumn(fcn::UTF8StringEditor::eraseChar(currRow, static_cast<int>(getCaretColumn())));
        } else if (key.getValue() == Key::Backspace && getCaretColumn() == 0 && getCaretRow() != 0 && mEditable) {
            // Merge with previous row: delete row, append to previous row
            unsigned const newCaretColumn = getTextRow(getCaretRow() - 1).size();
            mText->getRow(getCaretRow() - 1) += getTextRow(getCaretRow());
            mText->eraseRow(getCaretRow());
            setCaretRow(getCaretRow() - 1);
            setCaretColumn(newCaretColumn);
        } else if (
            key.getValue() == Key::Delete && getCaretColumn() < static_cast<int>(getTextRow(getCaretRow()).size()) &&
            mEditable) {
            // Delete character after caret within current row
            setCaretColumn(fcn::UTF8StringEditor::eraseChar(mText->getRow(getCaretRow()), getCaretColumn()));
        } else if (
            key.getValue() == Key::Delete && getCaretColumn() == static_cast<int>(getTextRow(getCaretRow()).size()) &&
            getCaretRow() < (static_cast<int>(getNumberOfRows()) - 1) && mEditable) {
            // Merge with next row: append next row to current, then delete next row
            mText->getRow(getCaretRow()) += getTextRow((getCaretRow() + 1));
            mText->eraseRow(getCaretRow() + 1);
        } else if (key.getValue() == Key::PageUp) {
            // Move caret up by rowsPerPage, preserving column position
            Widget* par = getParent();

            if (par != nullptr) {
                int const rowsPerPage = par->getChildrenArea().height / getFont()->getHeight();
                int const chars       = fcn::UTF8StringEditor::countChars(getTextRow(getCaretRow()), getCaretColumn());
                int const newCaretRow = getCaretRow() - rowsPerPage;
                if (newCaretRow >= 0) {
                    setCaretRow(newCaretRow);
                } else {
                    setCaretRow(0);
                }
                setCaretColumn(fcn::UTF8StringEditor::getOffset(getTextRow(getCaretRow()), chars));
            }
        } else if (key.getValue() == Key::PageDown) {
            // Move caret down by rowsPerPage, preserving column position
            Widget* par = getParent();

            if (par != nullptr) {
                int const rowsPerPage = par->getChildrenArea().height / getFont()->getHeight();
                int const chars       = fcn::UTF8StringEditor::countChars(getTextRow(getCaretRow()), getCaretColumn());
                setCaretRow(getCaretRow() + rowsPerPage);

                if (getCaretRow() >= static_cast<int>(getNumberOfRows())) {
                    setCaretRow(getNumberOfRows() - 1);
                }

                setCaretColumn(fcn::UTF8StringEditor::getOffset(getTextRow(getCaretRow()), chars));
            }
        } else if (key.getValue() == Key::Tab && mEditable) {
            // Insert spaces to align to next tab stop
            int constexpr tabSize    = 4;
            int const spacesToInsert = tabSize - (getCaretColumn() % tabSize);
            mText->getRow(getCaretRow()).insert(getCaretColumn(), std::string(spacesToInsert, ' '));
            setCaretColumn(getCaretColumn() + spacesToInsert);
        } else if (mEditable) {
            // Insert character at caret position (ASCII or valid Unicode code point)
            int const keyValue = key.getValue();
            bool const isValidCodePoint =
                keyValue >= 0 && keyValue <= 0x10FFFF && (keyValue < 0xD800 || keyValue > 0xDFFF);
            if (key.isCharacter() || (keyValue > 255 && isValidCodePoint)) {
                setCaretColumn(
                    fcn::UTF8StringEditor::insertChar(mText->getRow(getCaretRow()), getCaretColumn(), keyValue));
            }
        }

        adjustSizeImpl();
        scrollToCaret();

        std::string const & row = getTextRow(getCaretRow());

        assert("text row is valid utf8" && utf8::is_valid(row.begin(), row.end()));
        assert("caret column position is valid" && utf8::is_valid(row.begin(), row.begin() + getCaretColumn()));

        keyEvent.consume();
    }

    void TextBox::resizeToContent(bool recursion)
    {
        static_cast<void>(recursion);
        adjustSizeImpl();
    }

    void TextBox::adjustSize()
    {
        adjustSizeImpl();
    }

    void TextBox::adjustSizeImpl()
    {
        assert("font is not null" && getFont() != nullptr);

        Rectangle const & dim = mText->getDimension(getFont());
        setSize(dim.width, dim.height);
    }

    void TextBox::setCaretPosition(unsigned int position)
    {
        assert("position is within bounds" && position <= mText->getContent().size());

        mText->setCaretPosition(position);
    }

    unsigned int TextBox::getCaretPosition() const
    {
        return mText->getCaretPosition();
    }

    void TextBox::setCaretRowColumn(int row, int column)
    {
        assert("row is non-negative" && row >= 0);
        assert("column is non-negative" && column >= 0);

        mText->setCaretRow(row);
        mText->setCaretColumn(column);
    }

    void TextBox::setCaretRow(int row)
    {
        assert("row is non-negative" && row >= 0);

        mText->setCaretRow(row);
    }

    unsigned int TextBox::getCaretRow() const
    {
        return mText->getCaretRow();
    }

    void TextBox::setCaretColumn(int column)
    {
        assert("column is non-negative" && column >= 0);

        mText->setCaretColumn(column);
    }

    unsigned int TextBox::getCaretColumn() const
    {
        return mText->getCaretColumn();
    }

    std::string TextBox::getTextRow(int row) const
    {
        assert("row is non-negative" && row >= 0);
        assert("row is within bounds" && static_cast<unsigned>(row) < getNumberOfRows());

        return mText->getRow(row);
    }

    void TextBox::setTextRow(int row, std::string const & text)
    {
        assert("row is non-negative" && row >= 0);
        assert("row is within bounds" && static_cast<unsigned>(row) < getNumberOfRows());
        assert("text is valid utf8" && utf8::is_valid(text.begin(), text.end()));

        mText->setRow(row, text);
        adjustSizeImpl();
    }

    unsigned int TextBox::getNumberOfRows() const
    {
        return mText->getNumberOfRows();
    }

    std::string TextBox::getText() const
    {
        return mText->getContent();
    }

    void TextBox::fontChanged()
    {
        adjustSizeImpl();
    }

    void TextBox::scrollToCaret()
    {
        assert("font is not null" && getFont() != nullptr);

        showPart(mText->getCaretDimension(getFont()));
    }

    void TextBox::setEditable(bool editable)
    {
        mEditable = editable;
    }

    bool TextBox::isEditable() const
    {
        return mEditable;
    }

    void TextBox::addRow(std::string const & row)
    {
        assert("row text is valid utf8" && utf8::is_valid(row.begin(), row.end()));

        mText->addRow(row);
        adjustSizeImpl();
    }

    bool TextBox::isOpaque() const
    {
        return mOpaque;
    }

    void TextBox::setOpaque(bool opaque)
    {
        mOpaque = opaque;
    }

    void TextBox::setCaretColumnUTF8(int column)
    {
        assert("column is non-negative" && column >= 0);

        // no need to clip the column, mStringEditor handles it automatically
        setCaretColumn(fcn::UTF8StringEditor::getOffset(getTextRow(getCaretRow()), column));
    }

    void TextBox::setCaretRowUTF8(int row)
    {
        assert("row is non-negative" && row >= 0);

        int const chars = fcn::UTF8StringEditor::countChars(getTextRow(getCaretRow()), getCaretColumn());
        if (row < 0) {
            row = 0;
        } else if (row >= getNumberOfRows()) {
            row = getNumberOfRows() - 1;
        }
        setCaretRow(row);
        setCaretColumn(fcn::UTF8StringEditor::getOffset(getTextRow(getCaretRow()), chars));
    }

    void TextBox::setCaretRowColumnUTF8(int row, int column)
    {
        assert("row is non-negative" && row >= 0);
        assert("column is non-negative" && column >= 0);

        setCaretRowUTF8(row);
        setCaretColumnUTF8(column);
    }
} // namespace fcn
