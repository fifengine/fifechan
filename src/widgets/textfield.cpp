// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#include "fifechan/widgets/textfield.hpp"

#include <algorithm>
#include <string>

#include "fifechan/font.hpp"
#include "fifechan/graphics.hpp"
#include "fifechan/key.hpp"
#include "fifechan/mouseinput.hpp"
#include "fifechan/text.hpp"
#include "fifechan/utf8stringeditor.hpp"

namespace fcn
{
    TextField::TextField() : mEditable(true), mText(new Text()), mXScroll(0), mStringEditor(new UTF8StringEditor)
    {
        mText->addRow("");

        setFocusable(true);

        addMouseListener(this);
        addKeyListener(this);
    }

    TextField::TextField(std::string const & text) :
        mEditable(true), mXScroll(0), mText(new Text(text)), mStringEditor(new UTF8StringEditor)
    {
        adjustSize();

        setFocusable(true);

        addMouseListener(this);
        addKeyListener(this);
    }

    TextField::~TextField()
    {
        delete mText;
        delete mStringEditor;
    }

    void TextField::setText(std::string const & text)
    {
        mText->setContent(text);
    }

    void TextField::draw(Graphics* graphics)
    {
        Color const faceColor = getBaseColor();
        Color highlightColor;
        Color shadowColor;
        int const alpha  = getBaseColor().a;
        highlightColor   = faceColor + 0x303030;
        highlightColor.a = alpha;
        shadowColor      = faceColor - 0x303030;
        shadowColor.a    = alpha;

        // Draw a border.
        graphics->setColor(shadowColor);
        graphics->drawLine(0, 0, getWidth() - 1, 0);
        graphics->drawLine(0, 1, 0, getHeight() - 2);
        graphics->setColor(highlightColor);
        graphics->drawLine(getWidth() - 1, 1, getWidth() - 1, getHeight() - 1);
        graphics->drawLine(0, getHeight() - 1, getWidth() - 1, getHeight() - 1);

        // Push a clip area so the other drawings don't need to worry
        // about the border.
        graphics->pushClipArea(Rectangle(1, 1, getWidth() - 2, getHeight() - 2));

        graphics->setColor(getBackgroundColor());
        graphics->fillRectangle(0, 0, getWidth(), getHeight());

        if (isFocused()) {
            graphics->setColor(getSelectionColor());
            graphics->drawRectangle(0, 0, getWidth() - 2, getHeight() - 2);
            graphics->drawRectangle(1, 1, getWidth() - 4, getHeight() - 4);
        }

        if (isFocused() && isEditable()) {
            drawCaret(graphics, mText->getCaretX(getFont()) - mXScroll);
        }

        graphics->setColor(getForegroundColor());
        graphics->setFont(getFont());

        Rectangle const & dim = mText->getCaretDimension(getFont());
        if (mText->getNumberOfRows() != 0) {
            graphics->drawText(mText->getRow(0), 1 - mXScroll, 1);
        }

        graphics->popClipArea();
    }

    void TextField::drawCaret(Graphics* graphics, int x)
    {
        // Check the current clip area as a clip area with a different
        // size than the widget might have been pushed (which is the
        // case in the draw method when we push a clip area after we have
        // drawn a border).
        // Warning: not using a ref will result in truncation.
        ClipRectangle const & clipArea = graphics->getCurrentClipArea();

        graphics->setColor(getForegroundColor());
        graphics->drawLine(x, clipArea.height - 2, x, 1);
    }

    void TextField::mousePressed(MouseEvent& mouseEvent)
    {
        if (mouseEvent.getButton() == MouseEvent::Button::Left) {
            mText->setCaretPosition(mouseEvent.getX() + mXScroll, mouseEvent.getY(), getFont());
            fixScroll();
        }
    }

    void TextField::mouseDragged(MouseEvent& mouseEvent)
    {
        mouseEvent.consume();
    }

    void TextField::keyPressed(KeyEvent& keyEvent)
    {

        Key const key = keyEvent.getKey();

        if (key.getValue() == Key::Left && getCaretPosition() > 0) {
            setCaretPosition(fcn::UTF8StringEditor::prevChar(getText(), static_cast<int>(getCaretPosition())));
        } else if (key.getValue() == Key::Right && getCaretPosition() < getText().size()) {
            setCaretPosition(fcn::UTF8StringEditor::nextChar(getText(), static_cast<int>(getCaretPosition())));
        } else if (
            key.getValue() == Key::Delete && getCaretPosition() < getText().size() && mText->getNumberOfRows() > 0) {
            setCaretPosition(fcn::UTF8StringEditor::eraseChar(mText->getRow(0), static_cast<int>(getCaretPosition())));
        } else if (key.getValue() == Key::Backspace && getCaretPosition() > 0 && mText->getNumberOfRows() > 0) {
            setCaretPosition(fcn::UTF8StringEditor::prevChar(mText->getRow(0), static_cast<int>(getCaretPosition())));
            setCaretPosition(fcn::UTF8StringEditor::eraseChar(mText->getRow(0), static_cast<int>(getCaretPosition())));
        } else if (key.getValue() == Key::Enter) {
            distributeActionEvent();
        } else if (key.getValue() == Key::Home) {
            setCaretPosition(0);
        } else if (key.getValue() == Key::End) {
            setCaretPosition(getText().size());
        } else if (
            // Add character to text, if key is really an ASCII character
            // or is greater than 8bits long and the character is not
            // the tab key.
            (key.isCharacter() || (key.getValue() > 255 && mText->getNumberOfRows() > 0)) &&
            key.getValue() != Key::Tab) {
            setCaretPosition(fcn::UTF8StringEditor::insertChar(mText->getRow(0), getCaretPosition(), key.getValue()));
        }

        if (key.getValue() != Key::Tab) {
            // consume all characters except TAB which is needed
            // for traversing through widgets in a container.
            keyEvent.consume();
        }

        fixScroll();
    }

    void TextField::resizeToContent(bool recursion)
    {
        static_cast<void>(recursion);
        adjustSize();
    }

    void TextField::adjustSize()
    {
        Rectangle const & dim = mText->getDimension(getFont());
        setWidth(dim.width + 8);
        adjustHeight();

        fixScroll();
    }

    void TextField::adjustHeight()
    {
        setHeight(getFont()->getHeight() + 4);
    }

    void TextField::fixScroll()
    {
        if (isFocused()) {
            int const caretX = mText->getCaretDimension(getFont()).x;

            if (caretX - mXScroll >= getWidth() - 4) {
                mXScroll = caretX - getWidth() + 4;
            } else if (caretX - mXScroll <= 0) {
                mXScroll = caretX - getWidth() / 2;

                mXScroll = std::max(mXScroll, 0);
            }
        }
    }

    void TextField::setCaretPosition(unsigned int position)
    {
        mText->setCaretPosition(position);
    }

    unsigned int TextField::getCaretPosition() const
    {
        return mText->getCaretPosition();
    }

    std::string TextField::getText() const
    {
        return mText->getContent();
    }

    bool TextField::isEditable() const
    {
        return mEditable;
    }

    void TextField::setEditable(bool editable)
    {
        mEditable = editable;
    }
} // namespace fcn
