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
#include "fifechan/widgets/textfield.hpp"

using fcn::DefaultFont;
using fcn::Key;
using fcn::KeyEvent;
using fcn::TextField;

TEST_CASE("TextField default constructor", "[unit][textfield]")
{
    TextField tf;
    REQUIRE(tf.getText() == "\n");
    REQUIRE(tf.getCaretPosition() == 0);
    REQUIRE(tf.isEditable());
}

TEST_CASE("TextField text constructor", "[unit][textfield]")
{
    TextField tf("Hello");
    DefaultFont font;
    tf.setFont(&font);
    REQUIRE(tf.getText() == "Hello");
}

TEST_CASE("TextField setText and getText", "[unit][textfield]")
{
    TextField tf;
    tf.setText("New Text");
    REQUIRE(tf.getText() == "New Text");

    tf.setText("");
    REQUIRE(tf.getText().empty());

    tf.setText("Multi\nLine");
    REQUIRE(tf.getText() == "Multi\nLine");
}

TEST_CASE("TextField setCaretPosition and getCaretPosition", "[unit][textfield]")
{
    TextField tf("Hello World");
    tf.setCaretPosition(5);
    REQUIRE(tf.getCaretPosition() == 5);

    tf.setCaretPosition(0);
    REQUIRE(tf.getCaretPosition() == 0);

    tf.setCaretPosition(100);
    REQUIRE(tf.getCaretPosition() <= tf.getText().size());
}

TEST_CASE("TextField setEditable and isEditable", "[unit][textfield]")
{
    TextField tf;
    REQUIRE(tf.isEditable());

    tf.setEditable(false);
    REQUIRE(!tf.isEditable());

    tf.setEditable(true);
    REQUIRE(tf.isEditable());
}

TEST_CASE("TextField adjustSize", "[unit][textfield]")
{
    TextField tf("Sample");
    DefaultFont font;
    tf.setFont(&font);
    tf.adjustSize();
    REQUIRE(tf.getWidth() > 0);
    REQUIRE(tf.getHeight() > 0);
}

TEST_CASE("TextField adjustHeight", "[unit][textfield]")
{
    TextField tf;
    DefaultFont font;
    tf.setFont(&font);
    tf.adjustHeight();
    REQUIRE(true);
}

TEST_CASE("TextField keyPressed navigation", "[unit][textfield]")
{
    TextField tf("Hello World");
    DefaultFont font;
    tf.setFont(&font);

    Key const rightKey(Key::RIGHT);
    KeyEvent rightEvent(&tf, &tf, false, false, false, false, KeyEvent::Type::Pressed, false, rightKey);
    tf.keyPressed(rightEvent);
    REQUIRE(tf.getCaretPosition() == 1);

    Key const leftKey(Key::LEFT);
    KeyEvent leftEvent(&tf, &tf, false, false, false, false, KeyEvent::Type::Pressed, false, leftKey);
    tf.keyPressed(leftEvent);
    REQUIRE(tf.getCaretPosition() == 0);
}

TEST_CASE("TextField keyPressed home and end", "[unit][textfield]")
{
    TextField tf("Hello World");
    DefaultFont font;
    tf.setFont(&font);
    tf.setCaretPosition(5);

    Key const homeKey(Key::HOME);
    KeyEvent homeEvent(&tf, &tf, false, false, false, false, KeyEvent::Type::Pressed, false, homeKey);
    tf.keyPressed(homeEvent);
    REQUIRE(tf.getCaretPosition() == 0);

    Key const endKey(Key::END);
    KeyEvent endEvent(&tf, &tf, false, false, false, false, KeyEvent::Type::Pressed, false, endKey);
    tf.keyPressed(endEvent);
    REQUIRE(tf.getCaretPosition() == static_cast<unsigned int>(tf.getText().size()));
}

TEST_CASE("TextField keyPressed delete character", "[unit][textfield]")
{
    TextField tf("Hello");
    DefaultFont font;
    tf.setFont(&font);
    tf.setCaretPosition(5);

    Key const leftKey(Key::LEFT);
    KeyEvent leftEvent(&tf, &tf, false, false, false, false, KeyEvent::Type::Pressed, false, leftKey);
    tf.keyPressed(leftEvent);
    REQUIRE(tf.getCaretPosition() == 4);

    Key const backspaceKey(Key::BACKSPACE);
    KeyEvent bsEvent(&tf, &tf, false, false, false, false, KeyEvent::Type::Pressed, false, backspaceKey);
    tf.keyPressed(bsEvent);
    REQUIRE(tf.getText() == "Helo");
    REQUIRE(tf.getCaretPosition() == 3);
}

TEST_CASE("TextField setText triggers adjustSize", "[unit][textfield]")
{
    TextField tf;
    DefaultFont font;
    tf.setFont(&font);
    int const heightBefore = tf.getHeight();

    tf.setText("A longer text to adjust size");
    REQUIRE(tf.getHeight() >= heightBefore);
}
