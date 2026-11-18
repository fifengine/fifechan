// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2026 Fifengine contributors

// Corresponding header include
#include "fifechan/widgets/textfield.hpp"

// Standard library includes
#include <string>
#include <utility>

// Third-party library includes
#include <catch2/catch_test_macros.hpp>

// Project headers
#include "fifechan/defaultfont.hpp"
#include "fifechan/events/actionevent.hpp"
#include "fifechan/events/keyevent.hpp"
#include "fifechan/events/textinputevent.hpp"
#include "fifechan/key.hpp"
#include "fifechan/listeners/actionlistener.hpp"

using fcn::DefaultFont;
using fcn::Key;
using fcn::KeyEvent;
using fcn::TextField;
using fcn::TextInputEvent;

namespace
{
    class CountingActionListener : public fcn::ActionListener
    {
        public:
            void action(fcn::ActionEvent const & /*actionEvent*/) override
            {
                ++mCount;
            }

            int count() const
            {
                return mCount;
            }

        private:
            int mCount{0};
    };

    KeyEvent makeKeyEvent(TextField& field, int keyValue)
    {
        return {&field, &field, false, false, false, false, KeyEvent::Type::Pressed, false, Key(keyValue)};
    }

    TextInputEvent makeTextInput(TextField& field, std::string text)
    {
        return {&field, &field, false, false, false, false, std::move(text)};
    }
} // namespace

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
    REQUIRE(static_cast<size_t>(tf.getCaretPosition()) <= tf.getText().size());
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
    REQUIRE(static_cast<size_t>(tf.getCaretPosition()) == tf.getText().size());
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

TEST_CASE("TextField wantsTextInput follows editability", "[unit][textfield]")
{
    TextField tf;
    REQUIRE(tf.wantsTextInput() == true);

    tf.setEditable(false);
    REQUIRE(tf.wantsTextInput() == false);

    tf.setEditable(true);
    REQUIRE(tf.wantsTextInput() == true);
}

TEST_CASE("TextField textInput inserts UTF-8 text at the caret", "[unit][textfield]")
{
    TextField tf("ab");
    tf.setCaretPosition(1);

    auto event = makeTextInput(
        tf,
        "\xC3\xA4"
        "c"); // 'ä' (2 bytes) + 'c'
    tf.textInput(event);

    REQUIRE(
        tf.getText() ==
        "a\xC3\xA4"
        "cb");
    REQUIRE(tf.getCaretPosition() == 4); // byte offset after 2-byte 'ä' and 'c'
}

TEST_CASE("TextField textInput is ignored when not editable", "[unit][textfield]")
{
    TextField tf("abc");
    tf.setCaretPosition(3);
    tf.setEditable(false);

    auto event = makeTextInput(tf, "XYZ");
    tf.textInput(event);

    REQUIRE(tf.getText() == "abc");
    REQUIRE(tf.getCaretPosition() == 3);
}

TEST_CASE("TextField keyPressed does not insert printable characters", "[unit][textfield]")
{
    TextField tf("ab");
    tf.setCaretPosition(2);

    auto event = makeKeyEvent(tf, 'x');
    tf.keyPressed(event);

    REQUIRE(tf.getText() == "ab");
    REQUIRE(tf.getCaretPosition() == 2);
}

TEST_CASE("TextField keyPressed edits are ignored when not editable", "[unit][textfield]")
{
    TextField tf("abc");
    tf.setCaretPosition(3);
    tf.setEditable(false);

    auto backspace = makeKeyEvent(tf, Key::BACKSPACE);
    tf.keyPressed(backspace);
    REQUIRE(tf.getText() == "abc");

    tf.setCaretPosition(0);
    auto del = makeKeyEvent(tf, Key::KEY_DELETE);
    tf.keyPressed(del);
    REQUIRE(tf.getText() == "abc");

    auto printable = makeKeyEvent(tf, 'z');
    tf.keyPressed(printable);
    REQUIRE(tf.getText() == "abc");
}

TEST_CASE("TextField keyPressed navigation works when not editable", "[unit][textfield]")
{
    TextField tf("abc");
    tf.setEditable(false);

    auto end = makeKeyEvent(tf, Key::END);
    tf.keyPressed(end);
    REQUIRE(tf.getCaretPosition() == 3);

    auto home = makeKeyEvent(tf, Key::HOME);
    tf.keyPressed(home);
    REQUIRE(tf.getCaretPosition() == 0);
}

TEST_CASE("TextField keyPressed RETURN distributes action when not editable", "[unit][textfield]")
{
    TextField tf("abc");
    tf.setEditable(false);

    CountingActionListener listener;
    tf.addActionListener(&listener);

    auto enter = makeKeyEvent(tf, Key::KEY_RETURN);
    tf.keyPressed(enter);

    REQUIRE(listener.count() == 1);
}
