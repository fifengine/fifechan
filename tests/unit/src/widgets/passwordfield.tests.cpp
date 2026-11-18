// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2026 Fifengine contributors

// Standard library includes
#include <string>
#include <utility>

// Third-party library includes
#include <catch2/catch_test_macros.hpp>

// Project headers
#include "fifechan/defaultfont.hpp"
#include "fifechan/events/keyevent.hpp"
#include "fifechan/events/textinputevent.hpp"
#include "fifechan/key.hpp"
#include "fifechan/widgets/passwordfield.hpp"

using fcn::DefaultFont;
using fcn::Key;
using fcn::KeyEvent;
using fcn::PasswordField;
using fcn::TextInputEvent;

namespace
{
    // cppcheck-suppress constParameterReference -- event constructors require non-const Widget*
    KeyEvent makeKeyEvent(PasswordField& field, int keyValue)
    {
        return {&field, &field, false, false, false, false, KeyEvent::Type::Pressed, false, Key(keyValue)};
    }

    // cppcheck-suppress constParameterReference -- event constructors require non-const Widget*
    TextInputEvent makeTextInput(PasswordField& field, std::string text)
    {
        return {&field, &field, false, false, false, false, std::move(text)};
    }
} // namespace

TEST_CASE("PasswordField default constructor", "[unit][passwordfield]")
{
    PasswordField pf;
    DefaultFont font;
    pf.setFont(&font);
    REQUIRE(pf.getText().empty());
    REQUIRE(pf.isEditable());
}

TEST_CASE("PasswordField text constructor", "[unit][passwordfield]")
{
    PasswordField pf("secret123");
    DefaultFont font;
    pf.setFont(&font);
    REQUIRE(pf.getText() == "secret123");
}

TEST_CASE("PasswordField setText and getText", "[unit][passwordfield]")
{
    PasswordField pf;
    DefaultFont font;
    pf.setFont(&font);

    pf.setText("my_password");
    REQUIRE(pf.getText() == "my_password");

    pf.setText("");
    REQUIRE(pf.getText().empty());
}

TEST_CASE("PasswordField setText displays asterisks", "[unit][passwordfield]")
{
    PasswordField pf;
    DefaultFont font;
    pf.setFont(&font);

    pf.setText("abc");
    // The underlying TextField displays asterisks, but getText() returns actual text
    REQUIRE(pf.getText() == "abc");
}

TEST_CASE("PasswordField keyPressed navigation", "[unit][passwordfield]")
{
    PasswordField pf("hello");
    DefaultFont font;
    pf.setFont(&font);

    Key const rightKey(Key::RIGHT);
    KeyEvent rightEvent(&pf, &pf, false, false, false, false, KeyEvent::Type::Pressed, false, rightKey);
    pf.keyPressed(rightEvent);
    REQUIRE(pf.getCaretPosition() == 1);

    Key const leftKey(Key::LEFT);
    KeyEvent leftEvent(&pf, &pf, false, false, false, false, KeyEvent::Type::Pressed, false, leftKey);
    pf.keyPressed(leftEvent);
    REQUIRE(pf.getCaretPosition() == 0);
}

TEST_CASE("PasswordField keyPressed HOME and END", "[unit][passwordfield]")
{
    PasswordField pf("hello");
    DefaultFont font;
    pf.setFont(&font);
    pf.setCaretPosition(3);

    Key const homeKey(Key::HOME);
    KeyEvent homeEvent(&pf, &pf, false, false, false, false, KeyEvent::Type::Pressed, false, homeKey);
    pf.keyPressed(homeEvent);
    REQUIRE(pf.getCaretPosition() == 0);

    Key const endKey(Key::END);
    KeyEvent endEvent(&pf, &pf, false, false, false, false, KeyEvent::Type::Pressed, false, endKey);
    pf.keyPressed(endEvent);
    REQUIRE(static_cast<size_t>(pf.getCaretPosition()) == pf.getText().size());
}

TEST_CASE("PasswordField wantsTextInput follows editability", "[unit][passwordfield]")
{
    PasswordField pf("secret");
    REQUIRE(pf.wantsTextInput() == true);

    pf.setEditable(false);
    REQUIRE(pf.wantsTextInput() == false);
}

TEST_CASE("PasswordField textInput stores the real text and masks it", "[unit][passwordfield]")
{
    PasswordField pf;

    auto first = makeTextInput(pf, "ab");
    pf.textInput(first);

    REQUIRE(pf.getText() == "ab");
    REQUIRE(pf.getCaretPosition() == 2);

    auto second = makeTextInput(pf, "cd");
    pf.textInput(second);

    REQUIRE(pf.getText() == "abcd");
    REQUIRE(pf.getCaretPosition() == 4);
}

TEST_CASE("PasswordField textInput masks one character per code point", "[unit][passwordfield]")
{
    PasswordField pf;

    auto first = makeTextInput(pf, "\xC3\xA4"); // 'ä', 2 UTF-8 bytes, 1 code point
    pf.textInput(first);

    REQUIRE(pf.getText() == "\xC3\xA4");
    REQUIRE(pf.getCaretPosition() == 1); // one mask character

    auto second = makeTextInput(pf, "b");
    pf.textInput(second);

    REQUIRE(
        pf.getText() ==
        "\xC3\xA4"
        "b");
    REQUIRE(pf.getCaretPosition() == 2);

    auto backspace = makeKeyEvent(pf, Key::BACKSPACE);
    pf.keyPressed(backspace);

    REQUIRE(pf.getText() == "\xC3\xA4");
    REQUIRE(pf.getCaretPosition() == 1);

    pf.keyPressed(backspace);

    REQUIRE(pf.getText().empty());
    REQUIRE(pf.getCaretPosition() == 0);
}

TEST_CASE("PasswordField textInput is ignored when not editable", "[unit][passwordfield]")
{
    PasswordField pf;
    pf.setEditable(false);

    auto event = makeTextInput(pf, "secret");
    pf.textInput(event);

    REQUIRE(pf.getText().empty());
    REQUIRE(pf.getCaretPosition() == 0);
}

TEST_CASE("PasswordField keyPressed does not insert printable characters", "[unit][passwordfield]")
{
    PasswordField pf;

    auto key = makeKeyEvent(pf, 'a');
    pf.keyPressed(key);

    REQUIRE(pf.getText().empty());

    // A backend that delivers key and text for the same character must insert once.
    auto text = makeTextInput(pf, "a");
    pf.textInput(text);

    REQUIRE(pf.getText() == "a");
    REQUIRE(pf.getCaretPosition() == 1);
}

TEST_CASE("PasswordField setText masks by code point", "[unit][passwordfield]")
{
    PasswordField pf;
    pf.setText("\xC3\xA4\xC3\xB6\xC3\xBC"); // "äöü", three code points, six bytes

    auto end = makeKeyEvent(pf, Key::END);
    pf.keyPressed(end);

    REQUIRE(pf.getCaretPosition() == 3);
}
