// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2026 Fifengine contributors

// Standard library includes
#include <string>

// Third-party library includes
#include <catch2/catch_test_macros.hpp>

// Project headers
#include "fifechan/defaultfont.hpp"
#include "fifechan/events/keyevent.hpp"
#include "fifechan/key.hpp"
#include "fifechan/widgets/passwordfield.hpp"

using fcn::DefaultFont;
using fcn::Key;
using fcn::KeyEvent;
using fcn::PasswordField;

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
    REQUIRE(pf.getCaretPosition() == static_cast<unsigned int>(pf.getText().size()));
}
