// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2026 Fifengine contributors

// Corresponding header include
#include <fifechan/widgets/passwordfield.hpp>

// Standard library includes
#include <cassert>
#include <string>

// Third-party library includes
#include <utf8cpp/utf8.h>

// Project headers (subdirs before local)
#include "fifechan/events/textinputevent.hpp"
#include "fifechan/key.hpp"
#include "fifechan/text.hpp"
#include "fifechan/utf8stringeditor.hpp"

namespace fcn
{
    PasswordField::PasswordField(std::string const & text) : TextField(text), mActualText(new Text)
    {
        assert("mActualText must not be null after allocation" && mActualText != nullptr);
        setText(text);
    }

    PasswordField::~PasswordField()
    {
        delete mActualText;
    }

    void PasswordField::keyPressed(KeyEvent& keyEvent)
    {
        Key const key = keyEvent.getKey();

        if (key.getValue() == fcn::Key::LEFT && getCaretPosition() > 0) {
            mText->setCaretPosition(mText->getCaretPosition() - 1);
            setActualTextCaretPosition(fcn::UTF8StringEditor::prevChar(getText(), getActualTextCaretPosition()));
        } else if (key.getValue() == fcn::Key::RIGHT && getCaretPosition() < static_cast<int>(getText().size())) {
            mText->setCaretPosition(mText->getCaretPosition() + 1);
            setActualTextCaretPosition(fcn::UTF8StringEditor::nextChar(getText(), getActualTextCaretPosition()));
        } else if (
            mEditable && key.getValue() == fcn::Key::KEY_DELETE &&
            getCaretPosition() < static_cast<int>(getText().size()) && mText->getNumberOfRows() > 0) {
            mText->remove(1);
            setActualTextCaretPosition(
                fcn::UTF8StringEditor::eraseChar(mActualText->getRow(0), getActualTextCaretPosition()));
        } else if (
            mEditable && key.getValue() == fcn::Key::BACKSPACE && getCaretPosition() > 0 &&
            mText->getNumberOfRows() > 0) {
            mText->remove(-1);
            setActualTextCaretPosition(
                fcn::UTF8StringEditor::prevChar(mActualText->getRow(0), getActualTextCaretPosition()));
            setActualTextCaretPosition(
                fcn::UTF8StringEditor::eraseChar(mActualText->getRow(0), getActualTextCaretPosition()));
        } else if (key.getValue() == fcn::Key::KEY_RETURN) {
            distributeActionEvent();
        } else if (key.getValue() == fcn::Key::HOME) {
            mText->setCaretColumn(0);
            setActualTextCaretPosition(0);
        } else if (key.getValue() == fcn::Key::END) {
            mText->setCaretColumn(mText->getNumberOfCharacters(0));
            setActualTextCaretPosition(static_cast<int>(getText().size()));
        }
        // Printable characters are handled by textInput(TextInputEvent&).

        if (key.getValue() != fcn::Key::TAB) {
            // consume all characters except TAB which is needed
            // for traversing through widgets in a container.
            keyEvent.consume();
        }

        fixScroll();
    }

    void PasswordField::textInput(TextInputEvent& event)
    {
        std::string const & text = event.getText();

        if (!mEditable || text.empty() || mText->getNumberOfRows() == 0) {
            return;
        }

        assert("text is valid utf8" && utf8::is_valid(text.begin(), text.end()));

        int const codePoints = static_cast<int>(utf8::distance(text.begin(), text.end()));

        for (int i = 0; i < codePoints; ++i) {
            mText->insert('*');
        }

        int const actualCaret = getActualTextCaretPosition();
        mActualText->getRow(0).insert(static_cast<std::string::size_type>(actualCaret), text);
        setActualTextCaretPosition(actualCaret + static_cast<int>(text.size()));

        fixScroll();
    }

    void PasswordField::setText(std::string const & text)
    {
        assert("mText must not be null" && mText != nullptr);
        assert("mActualText must not be null" && mActualText != nullptr);

        std::string asterisks;
        auto const characters = static_cast<std::string::size_type>(utf8::distance(text.begin(), text.end()));
        asterisks.assign(characters, '*');

        mText->setContent(asterisks);
        mActualText->setContent(text);
    }

    std::string PasswordField::getText() const
    {
        return mActualText->getContent();
    }

    void PasswordField::setActualTextCaretPosition(int position)
    {
        mActualText->setCaretPosition(position);
    }

    int PasswordField::getActualTextCaretPosition() const
    {
        return mActualText->getCaretPosition();
    }
}; // namespace fcn
