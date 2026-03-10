// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#include <fifechan/widgets/textbox.hpp>

#include <fifechan/font.hpp>
#include <fifechan/graphics.hpp>
#include <fifechan/key.hpp>
#include <fifechan/mouseinput.hpp>
#include <fifechan/text.hpp>
#include <fifechan/utf8stringeditor.hpp>
#include <fifechan/util/utf8/utf8.hpp>

#include <cassert>
#include <string>

namespace fcn
{
    TextBox::TextBox(std::string const & text) :
        mEditable(true), mOpaque(true), mText(new Text(text)), mStringEditor(new UTF8StringEditor)
    {
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
        mText->setContent(text);
        adjustSizeImpl();
    }

    void TextBox::draw(Graphics* graphics)
    {
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
        graphics->setColor(getForegroundColor());
        graphics->drawLine(x, y, x, y + getFont()->getHeight());
    }

    void TextBox::mousePressed(MouseEvent& mouseEvent)
    {
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
            if (getCaretColumn() == 0) {
                if (getCaretRow() > 0) {
                    setCaretRow(getCaretRow() - 1);
                    setCaretColumn(getTextRow(getCaretRow()).size());
                }
            } else {
                setCaretColumn(fcn::UTF8StringEditor::prevChar(getTextRow(getCaretRow()), getCaretColumn()));
            }
        } else if (key.getValue() == Key::Right) {
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
            mText->insertRow(
                getTextRow(getCaretRow()).substr(getCaretColumn(), getTextRow(getCaretRow()).size() - getCaretColumn()),
                getCaretRow() + 1);

            mText->getRow(getCaretRow()).resize(getCaretColumn());
            setCaretRow(getCaretRow() + 1);
            setCaretColumn(0);
        } else if (key.getValue() == Key::Backspace && getCaretColumn() != 0 && mEditable) {
            std::string& currRow = mText->getRow(getCaretRow());
            setCaretColumn(fcn::UTF8StringEditor::prevChar(currRow, static_cast<int>(getCaretColumn())));
            setCaretColumn(fcn::UTF8StringEditor::eraseChar(currRow, static_cast<int>(getCaretColumn())));
        } else if (key.getValue() == Key::Backspace && getCaretColumn() == 0 && getCaretRow() != 0 && mEditable) {
            /*setCaretColumn(mText->getRow(getCaretRow() - 1).size());
            mText->getRow(getCaretRow() - 1) += getTextRow(getCaretRow());
            mText->eraseRow(getCaretRow());
            setCaretRow(getCaretRow() - 1);*/
            unsigned const newCaretColumn = getTextRow(getCaretRow() - 1).size();
            // setCaretColumn(getTextRow(getCaretRow() - 1).size());
            mText->getRow(getCaretRow() - 1) += getTextRow(getCaretRow());
            mText->eraseRow(getCaretRow());
            setCaretRow(getCaretRow() - 1);
            setCaretColumn(newCaretColumn);
        } else if (
            key.getValue() == Key::Delete && getCaretColumn() < static_cast<int>(getTextRow(getCaretRow()).size()) &&
            mEditable) {
            setCaretColumn(fcn::UTF8StringEditor::eraseChar(mText->getRow(getCaretRow()), getCaretColumn()));
        } else if (
            key.getValue() == Key::Delete && getCaretColumn() == static_cast<int>(getTextRow(getCaretRow()).size()) &&
            getCaretRow() < (static_cast<int>(getNumberOfRows()) - 1) && mEditable) {
            mText->getRow(getCaretRow()) += getTextRow((getCaretRow() + 1));
            mText->eraseRow(getCaretRow() + 1);
        } else if (key.getValue() == Key::PageUp) {
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
            // FIXME: jump X spaces, so getCaretColumn() % TAB_SIZE = 0 and X <= TAB_SIZE
            mText->getRow(getCaretRow()).insert(getCaretColumn(), std::string("    "));
            setCaretColumn(getCaretColumn() + 4);
        } else if ((key.isCharacter() || key.getValue() > 255) && mEditable) {
            setCaretColumn(
                fcn::UTF8StringEditor::insertChar(mText->getRow(getCaretRow()), getCaretColumn(), key.getValue()));
        }

        adjustSizeImpl();
        scrollToCaret();
        assert(
            utf8::is_valid(getTextRow(getCaretRow()).begin(), getTextRow(getCaretRow()).end()) ==
            utf8::internal::UTF8_OK);
        assert(
            utf8::is_valid(getTextRow(getCaretRow()).begin(), getTextRow(getCaretRow()).begin() + getCaretColumn()) ==
            utf8::internal::UTF8_OK);
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
        Rectangle const & dim = mText->getDimension(getFont());
        setSize(dim.width, dim.height);
    }

    void TextBox::setCaretPosition(unsigned int position)
    {
        mText->setCaretPosition(position);
    }

    unsigned int TextBox::getCaretPosition() const
    {
        return mText->getCaretPosition();
    }

    void TextBox::setCaretRowColumn(int row, int column)
    {
        mText->setCaretRow(row);
        mText->setCaretColumn(column);
    }

    void TextBox::setCaretRow(int row)
    {
        mText->setCaretRow(row);
    }

    unsigned int TextBox::getCaretRow() const
    {
        return mText->getCaretRow();
    }

    void TextBox::setCaretColumn(int column)
    {
        mText->setCaretColumn(column);
    }

    unsigned int TextBox::getCaretColumn() const
    {
        return mText->getCaretColumn();
    }

    std::string TextBox::getTextRow(int row) const
    {
        return mText->getRow(row);
    }

    void TextBox::setTextRow(int row, std::string const & text)
    {
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
        setCaretRowUTF8(row);
        setCaretColumnUTF8(column);
    }
} // namespace fcn
