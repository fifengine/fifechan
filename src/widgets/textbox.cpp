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
#include "fifechan/events/textinputevent.hpp"
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

        for (int i = 0; i < mText->getNumberOfRows(); ++i) {
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

        if (key.getValue() == fcn::Key::LEFT) {
            // Move caret left, or to end of previous row if at column 0
            if (getCaretColumn() == 0) {
                if (getCaretRow() > 0) {
                    setCaretRow(getCaretRow() - 1);
                    setCaretColumn(static_cast<int>(getTextRow(getCaretRow()).size()));
                }
            } else {
                setCaretColumn(fcn::UTF8StringEditor::prevChar(getTextRow(getCaretRow()), getCaretColumn()));
            }
        } else if (key.getValue() == fcn::Key::RIGHT) {
            // Move caret right, or to start of next row if at end of line
            if (getCaretColumn() < static_cast<int>(getTextRow(getCaretRow()).size())) {
                setCaretColumn(fcn::UTF8StringEditor::nextChar(getTextRow(getCaretRow()), getCaretColumn()));
            } else {
                if (getCaretRow() < getNumberOfRows() - 1) {
                    setCaretRow(getCaretRow() + 1);
                    setCaretColumn(0);
                }
            }
        } else if (key.getValue() == fcn::Key::DOWN) {
            setCaretRowUTF8(getCaretRow() + 1);
        } else if (key.getValue() == fcn::Key::UP) {
            setCaretRowUTF8(getCaretRow() - 1);
        } else if (key.getValue() == fcn::Key::HOME) {
            setCaretColumn(0);
        } else if (key.getValue() == fcn::Key::END) {
            setCaretColumn(static_cast<int>(getTextRow(getCaretRow()).size()));
        } else if (key.getValue() == fcn::Key::KEY_RETURN && mEditable) {
            // Split current row at caret: text after caret becomes new row
            int const caretRow              = getCaretRow();
            int const caretCol              = getCaretColumn();
            std::string const & currRowText = getTextRow(caretRow);
            mText->insertRow(
                currRowText.substr(static_cast<size_t>(caretCol), currRowText.size() - static_cast<size_t>(caretCol)),
                caretRow + 1);

            mText->getRow(caretRow).resize(static_cast<size_t>(caretCol));
            setCaretRow(caretRow + 1);
            setCaretColumn(0);
        } else if (key.getValue() == fcn::Key::BACKSPACE && getCaretColumn() != 0 && mEditable) {
            // Delete character before caret within current row
            std::string& currRow = mText->getRow(getCaretRow());
            setCaretColumn(fcn::UTF8StringEditor::prevChar(currRow, getCaretColumn()));
            setCaretColumn(fcn::UTF8StringEditor::eraseChar(currRow, getCaretColumn()));
        } else if (key.getValue() == fcn::Key::BACKSPACE && getCaretColumn() == 0 && getCaretRow() != 0 && mEditable) {
            // Merge with previous row: delete row, append to previous row
            int const prevRow        = getCaretRow() - 1;
            int const newCaretColumn = static_cast<int>(getTextRow(prevRow).size());
            mText->getRow(prevRow) += getTextRow(getCaretRow());
            mText->eraseRow(getCaretRow());
            setCaretRow(prevRow);
            setCaretColumn(newCaretColumn);
        } else if (
            key.getValue() == fcn::Key::KEY_DELETE &&
            getCaretColumn() < static_cast<int>(getTextRow(getCaretRow()).size()) && mEditable) {
            // Delete character after caret within current row
            setCaretColumn(fcn::UTF8StringEditor::eraseChar(mText->getRow(getCaretRow()), getCaretColumn()));
        } else if (
            key.getValue() == fcn::Key::KEY_DELETE &&
            getCaretColumn() == static_cast<int>(getTextRow(getCaretRow()).size()) &&
            getCaretRow() < getNumberOfRows() - 1 && mEditable) {
            // Merge with next row: append next row to current, then delete next row
            mText->getRow(getCaretRow()) += getTextRow(getCaretRow() + 1);
            mText->eraseRow(getCaretRow() + 1);
        } else if (key.getValue() == fcn::Key::PAGEUP) {
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
        } else if (key.getValue() == fcn::Key::PAGEDOWN) {
            // Move caret down by rowsPerPage, preserving column position
            Widget* par = getParent();

            if (par != nullptr) {
                int const rowsPerPage = par->getChildrenArea().height / getFont()->getHeight();
                int const chars       = fcn::UTF8StringEditor::countChars(getTextRow(getCaretRow()), getCaretColumn());
                setCaretRow(getCaretRow() + rowsPerPage);

                if (getCaretRow() >= getNumberOfRows()) {
                    setCaretRow(getNumberOfRows() - 1);
                }

                setCaretColumn(fcn::UTF8StringEditor::getOffset(getTextRow(getCaretRow()), chars));
            }
        } else if (key.getValue() == fcn::Key::TAB && mEditable) {
            // Insert spaces to align to next tab stop
            size_t constexpr tabSize    = 4;
            size_t const spacesToInsert = tabSize - static_cast<size_t>(getCaretColumn() % static_cast<int>(tabSize));
            unsigned long insertPos = 0;
                if (getCaretColumn() > 0) {
                    insertPos = static_cast<unsigned long>(getCaretColumn());
                }
                mText->getRow(getCaretRow())
                    .insert(insertPos, std::string(spacesToInsert, ' '));
            setCaretColumn(getCaretColumn() + static_cast<int>(spacesToInsert));
        }
        // Printable characters are handled by textInput(TextInputEvent&).

        adjustSizeImpl();
        scrollToCaret();

        std::string const & row = getTextRow(getCaretRow());

        assert("text row is valid utf8" && utf8::is_valid(row.begin(), row.end()));
        assert(
            "caret column position is valid" &&
            utf8::is_valid(row.begin(), row.begin() + std::max(0, getCaretColumn())));

        keyEvent.consume();
    }

    void TextBox::textInput(TextInputEvent& event)
    {
        std::string const & text = event.getText();
        if (!text.empty() && mEditable) {
            mText->getRow(getCaretRow()).insert(static_cast<size_t>(getCaretColumn()), text);
            setCaretColumn(getCaretColumn() + static_cast<int>(text.size()));
            adjustSizeImpl();
            scrollToCaret();
        }
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

        // Default to measured content size
        int newWidth  = static_cast<int>(dim.width);
        int newHeight = static_cast<int>(dim.height);

        if (isFixedSize()) {
            return;
        }

        // Handle expand flags:
        // if the widget is set to expand in an axis,
        // keep the current size in that axis so parent layout can
        // control it instead of content-driven resizing.
        if (isHorizontalExpand()) {
            newWidth = getWidth();
        }
        if (isVerticalExpand()) {
            newHeight = getHeight();
        }

        setSize(newWidth, newHeight);
    }

    void TextBox::setCaretPosition(int position)
    {
        assert(
            "position is within bounds" && position >= 0 &&
            static_cast<std::size_t>(position) <= mText->getContent().size());
        mText->setCaretPosition(position);
    }

    int TextBox::getCaretPosition() const
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

    int TextBox::getCaretRow() const
    {
        return mText->getCaretRow();
    }

    void TextBox::setCaretColumn(int column)
    {
        assert("column is non-negative" && column >= 0);
        mText->setCaretColumn(column);
    }

    int TextBox::getCaretColumn() const
    {
        return mText->getCaretColumn();
    }

    std::string TextBox::getTextRow(int row) const
    {
        assert("row is non-negative" && row >= 0);
        assert("row is within bounds" && row < getNumberOfRows());

        return mText->getRow(row);
    }

    void TextBox::setTextRow(int row, std::string const & text)
    {
        assert("row is non-negative" && row >= 0);
        assert("row is within bounds" && row < getNumberOfRows());
        assert("text is valid utf8" && utf8::is_valid(text.begin(), text.end()));

        mText->setRow(static_cast<unsigned int>(row), text);
        adjustSizeImpl();
    }

    int TextBox::getNumberOfRows() const
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
