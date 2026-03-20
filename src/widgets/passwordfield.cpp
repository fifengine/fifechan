// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2026 Fifengine contributors

#include <fifechan/widgets/passwordfield.hpp>

#include <fifechan/key.hpp>
#include <fifechan/text.hpp>
#include <fifechan/utf8stringeditor.hpp>

#include <string>

namespace fcn
{
    PasswordField::PasswordField(std::string const & text) : TextField(text), mActualText(new Text)
    {
        setText(text);
    }

    PasswordField::~PasswordField()
    {
        delete mActualText;
    }

    void PasswordField::keyPressed(KeyEvent& keyEvent)
    {
        Key const key = keyEvent.getKey();

        if (key.getValue() == Key::Left && getCaretPosition() > 0) {
            mText->setCaretPosition(mText->getCaretPosition() - 1);
            setActualTextCaretPosition(
                fcn::UTF8StringEditor::prevChar(getText(), static_cast<int>(getActualTextCaretPosition())));
        } else if (key.getValue() == Key::Right && getCaretPosition() < getText().size()) {
            mText->setCaretPosition(mText->getCaretPosition() + 1);
            setActualTextCaretPosition(
                fcn::UTF8StringEditor::nextChar(getText(), static_cast<int>(getActualTextCaretPosition())));
        } else if (
            mEditable && key.getValue() == Key::Delete && getCaretPosition() < getText().size() &&
            mText->getNumberOfRows() > 0) {
            mText->remove(1);
            setActualTextCaretPosition(
                fcn::UTF8StringEditor::eraseChar(
                    mActualText->getRow(0), static_cast<int>(getActualTextCaretPosition())));
        } else if (
            mEditable && key.getValue() == Key::Backspace && getCaretPosition() > 0 && mText->getNumberOfRows() > 0) {
            mText->remove(-1);
            setActualTextCaretPosition(
                fcn::UTF8StringEditor::prevChar(
                    mActualText->getRow(0), static_cast<int>(getActualTextCaretPosition())));
            setActualTextCaretPosition(
                fcn::UTF8StringEditor::eraseChar(
                    mActualText->getRow(0), static_cast<int>(getActualTextCaretPosition())));
        } else if (key.getValue() == Key::Enter) {
            distributeActionEvent();
        } else if (key.getValue() == Key::Home) {
            mText->setCaretColumn(0);
            setActualTextCaretPosition(0);
        } else if (key.getValue() == Key::End) {
            mText->setCaretColumn(mText->getNumberOfCharacters(0));
            setActualTextCaretPosition(getText().size());
        } else if (
            // Add character to text, if key is a ASCII character
            // or is greater than 8bits long and the character is not
            // the tab key.
            (key.isCharacter() || (key.getValue() > 255 && mText->getNumberOfRows() > 0)) &&
            key.getValue() != Key::Tab) {
            mText->insert('*');
            setActualTextCaretPosition(
                fcn::UTF8StringEditor::insertChar(
                    mActualText->getRow(0), getActualTextCaretPosition(), key.getValue()));
        }

        if (key.getValue() != Key::Tab) {
            // consume all characters except TAB which is needed
            // for traversing through widgets in a container.
            keyEvent.consume();
        }

        fixScroll();
    }

    void PasswordField::setText(std::string const & text)
    {
        std::string asterisks;
        asterisks.assign(text.size(), '*');

        mText->setContent(asterisks);
        mActualText->setContent(text);
    }

    std::string PasswordField::getText() const
    {
        return mActualText->getContent();
    }

    void PasswordField::setActualTextCaretPosition(unsigned int position)
    {
        mActualText->setCaretPosition(position);
    }

    unsigned int PasswordField::getActualTextCaretPosition() const
    {
        return mActualText->getCaretPosition();
    }
}; // namespace fcn
