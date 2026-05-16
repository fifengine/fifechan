// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2026 Fifengine contributors

// Standard library includes
#include <string>

// Third-party library includes
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>

// Project headers
#include "fifechan/color.hpp"
#include "fifechan/defaultfont.hpp"
#include "fifechan/events/actionevent.hpp"
#include "fifechan/events/keyevent.hpp"
#include "fifechan/events/mouseevent.hpp"
#include "fifechan/graphics.hpp"
#include "fifechan/key.hpp"
#include "fifechan/listeners/actionlistener.hpp"
#include "fifechan/mouseinput.hpp"
#include "fifechan/widgets/button.hpp"

using fcn::ActionEvent;
using fcn::ActionListener;
using fcn::Button;
using fcn::Color;
using fcn::DefaultFont;
using fcn::Graphics;
using fcn::Key;
using fcn::KeyEvent;
using fcn::MouseEvent;
using fcn::MouseInput;

namespace
{
    class MockActionListener : public ActionListener
    {
        public:
            void action(ActionEvent const & /*actionEvent*/) override
            {
                actionCalled = true;
            }
            bool actionCalled = false;
    };
} // namespace

TEST_CASE("Button default constructor", "[unit][button]")
{
    Button btn;
    REQUIRE(btn.getCaption().empty());
    REQUIRE(btn.getAlignment() == Graphics::Alignment::Center);
    REQUIRE(btn.isActive());
    REQUIRE(btn.getDownXOffset() == 1);
    REQUIRE(btn.getDownYOffset() == 1);
    REQUIRE(btn.isFocusable());
}

TEST_CASE("Button caption constructor", "[unit][button]")
{
    Button btn("Click Me");
    REQUIRE(btn.getCaption() == "Click Me");
    REQUIRE(btn.getAlignment() == Graphics::Alignment::Center);
    REQUIRE(btn.getWidth() > 0);
    REQUIRE(btn.getHeight() > 0);
}

TEST_CASE("Button setCaption and getCaption", "[unit][button]")
{
    Button btn;
    btn.setCaption("Hello");
    REQUIRE(btn.getCaption() == "Hello");

    btn.setCaption("");
    REQUIRE(btn.getCaption().empty());

    btn.setCaption("Multi Word Caption");
    REQUIRE(btn.getCaption() == "Multi Word Caption");
}

TEST_CASE("Button setCaption updates width", "[unit][button]")
{
    Button btn;
    int const initialWidth = btn.getWidth();
    btn.setCaption("A longer caption that should increase width");
    REQUIRE(btn.getWidth() > initialWidth);
}

TEST_CASE("Button setActive and isActive", "[unit][button]")
{
    Button btn;
    REQUIRE(btn.isActive());

    btn.setActive(false);
    REQUIRE(!btn.isActive());

    btn.setActive(true);
    REQUIRE(btn.isActive());
}

TEST_CASE("Button setAlignment and getAlignment", "[unit][button]")
{
    Button btn;
    btn.setAlignment(Graphics::Alignment::Left);
    REQUIRE(btn.getAlignment() == Graphics::Alignment::Left);

    btn.setAlignment(Graphics::Alignment::Center);
    REQUIRE(btn.getAlignment() == Graphics::Alignment::Center);

    btn.setAlignment(Graphics::Alignment::Right);
    REQUIRE(btn.getAlignment() == Graphics::Alignment::Right);
}

TEST_CASE("Button setDownXOffset and getDownXOffset", "[unit][button]")
{
    Button btn;
    btn.setDownXOffset(5);
    REQUIRE(btn.getDownXOffset() == 5);

    btn.setDownXOffset(-3);
    REQUIRE(btn.getDownXOffset() == -3);
}

TEST_CASE("Button setDownYOffset and getDownYOffset", "[unit][button]")
{
    Button btn;
    btn.setDownYOffset(7);
    REQUIRE(btn.getDownYOffset() == 7);

    btn.setDownYOffset(0);
    REQUIRE(btn.getDownYOffset() == 0);
}

TEST_CASE("Button setDownOffset sets both x and y", "[unit][button]")
{
    Button btn;
    btn.setDownOffset(3, 4);
    REQUIRE(btn.getDownXOffset() == 3);
    REQUIRE(btn.getDownYOffset() == 4);
}

TEST_CASE("Button adjustSize resizes to caption", "[unit][button]")
{
    Button btn("Text");
    int const widthWithText = btn.getWidth();

    btn.setCaption("Much longer text for button");
    REQUIRE(btn.getWidth() > widthWithText);

    btn.setCaption("");
    btn.adjustSize();
    REQUIRE(btn.getWidth() < widthWithText);
}

TEST_CASE("Button resizeToContent works", "[unit][button]")
{
    Button btn("Test");
    btn.resizeToContent();
    REQUIRE(btn.getWidth() > 0);
}

TEST_CASE("Button fontChanged triggers adjustSize", "[unit][button]")
{
    Button btn("Sample");
    DefaultFont font;
    btn.setFont(&font);
    btn.fontChanged();
    REQUIRE(btn.getCaption() == "Sample");
}

TEST_CASE("Button distributeActionEvent", "[unit][button]")
{
    Button btn("test");
    MockActionListener listener;
    btn.setActionEventId("btn_action");
    btn.addActionListener(&listener);

    // Simulate pressing Enter to trigger action
    Key const enterKey(Key::KEY_RETURN);
    KeyEvent pressEvent(&btn, &btn, false, false, false, false, KeyEvent::Type::Pressed, false, enterKey);
    btn.keyPressed(pressEvent);
    REQUIRE(pressEvent.isConsumed());

    KeyEvent releaseEvent(&btn, &btn, false, false, false, false, KeyEvent::Type::Released, false, enterKey);
    btn.keyReleased(releaseEvent);
    REQUIRE(listener.actionCalled);
}

TEST_CASE("Button keyPressed SPACE triggers action on release", "[unit][button]")
{
    Button btn;
    MockActionListener listener;
    btn.addActionListener(&listener);
    btn.setActionEventId("space_btn");

    Key const spaceKey(Key::SPACE);
    KeyEvent pressEvent(&btn, &btn, false, false, false, false, KeyEvent::Type::Pressed, false, spaceKey);
    btn.keyPressed(pressEvent);
    REQUIRE(pressEvent.isConsumed());

    KeyEvent releaseEvent(&btn, &btn, false, false, false, false, KeyEvent::Type::Released, false, spaceKey);
    btn.keyReleased(releaseEvent);
    REQUIRE(listener.actionCalled);
}

TEST_CASE("Button keyPressed non-activation key is ignored", "[unit][button]")
{
    Button btn;
    MockActionListener listener;
    btn.addActionListener(&listener);

    Key const aKey('A');
    KeyEvent pressEvent(&btn, &btn, false, false, false, false, KeyEvent::Type::Pressed, false, aKey);
    btn.keyPressed(pressEvent);
    REQUIRE(!pressEvent.isConsumed());
}

TEST_CASE("Button mouseEntered and mouseExited", "[unit][button]")
{
    Button btn;
    MouseEvent enterEvent(
        &btn, &btn, false, false, false, false, MouseEvent::Type::Entered, MouseEvent::Button::Empty, 0, 0, 0);
    btn.mouseEntered(enterEvent);
    // just verify no crash

    MouseEvent exitEvent(
        &btn, &btn, false, false, false, false, MouseEvent::Type::Exited, MouseEvent::Button::Empty, 0, 0, 0);
    btn.mouseExited(exitEvent);
}

TEST_CASE("Button mousePressed left button", "[unit][button]")
{
    Button btn;
    btn.setSize(100, 50);

    MouseEvent pressEvent(
        &btn, &btn, false, false, false, false, MouseEvent::Type::Pressed, MouseEvent::Button::Left, 50, 25, 0);
    btn.mouseEntered(pressEvent); // So mHasMouse is true

    // Now pressing mouse inside
    MouseEvent enterForPress(
        &btn, &btn, false, false, false, false, MouseEvent::Type::Entered, MouseEvent::Button::Empty, 50, 25, 0);
    btn.mouseEntered(enterForPress);

    btn.mousePressed(pressEvent);
    REQUIRE(pressEvent.isConsumed());
}

TEST_CASE("Button mouseReleased fires action when pressed and mouse inside", "[unit][button]")
{
    Button btn;
    btn.setSize(100, 50);
    MockActionListener listener;
    btn.addActionListener(&listener);
    btn.setActionEventId("release_btn");

    MouseEvent enterEvent(
        &btn, &btn, false, false, false, false, MouseEvent::Type::Entered, MouseEvent::Button::Empty, 50, 25, 0);
    btn.mouseEntered(enterEvent);

    MouseEvent pressEvent(
        &btn, &btn, false, false, false, false, MouseEvent::Type::Pressed, MouseEvent::Button::Left, 50, 25, 0);
    btn.mousePressed(pressEvent);

    MouseEvent releaseEvent(
        &btn, &btn, false, false, false, false, MouseEvent::Type::Released, MouseEvent::Button::Left, 50, 25, 0);
    btn.mouseReleased(releaseEvent);
    REQUIRE(listener.actionCalled);
}

TEST_CASE("Button mouseReleased without mouse press does not fire action", "[unit][button]")
{
    Button btn;
    MockActionListener listener;
    btn.addActionListener(&listener);

    MouseEvent releaseEvent(
        &btn, &btn, false, false, false, false, MouseEvent::Type::Released, MouseEvent::Button::Left, 0, 0, 0);
    btn.mouseReleased(releaseEvent);
    REQUIRE(!listener.actionCalled);
}

TEST_CASE("Button mouseDragged consumes event", "[unit][button]")
{
    Button btn;
    MouseEvent dragEvent(
        &btn, &btn, false, false, false, false, MouseEvent::Type::Dragged, MouseEvent::Button::Left, 0, 0, 0);
    btn.mouseDragged(dragEvent);
    REQUIRE(dragEvent.isConsumed());
}

TEST_CASE("Button focusLost resets state", "[unit][button]")
{
    Button btn;
    // First press a key
    Key const enterKey(Key::KEY_RETURN);
    KeyEvent pressEvent(&btn, &btn, false, false, false, false, KeyEvent::Type::Pressed, false, enterKey);
    btn.keyPressed(pressEvent);
    REQUIRE(pressEvent.isConsumed());

    // Focus lost resets the key pressed state
    fcn::Event focusEvent(&btn);
    btn.focusLost(focusEvent);

    // Now release should NOT fire action since state was reset
    MockActionListener listener;
    btn.addActionListener(&listener);
    KeyEvent releaseEvent(&btn, &btn, false, false, false, false, KeyEvent::Type::Released, false, enterKey);
    btn.keyReleased(releaseEvent);
    REQUIRE(!listener.actionCalled);
}

TEST_CASE("Button ancestorHidden resets state", "[unit][button]")
{
    Button btn;
    // Press key
    Key const spaceKey(Key::SPACE);
    KeyEvent pressEvent(&btn, &btn, false, false, false, false, KeyEvent::Type::Pressed, false, spaceKey);
    btn.keyPressed(pressEvent);

    fcn::Event hiddenEvent(&btn);
    btn.ancestorHidden(hiddenEvent);

    MockActionListener listener;
    btn.addActionListener(&listener);
    KeyEvent releaseEvent(&btn, &btn, false, false, false, false, KeyEvent::Type::Released, false, spaceKey);
    btn.keyReleased(releaseEvent);
    REQUIRE(!listener.actionCalled);
}

TEST_CASE("Button non-left mouse buttons are ignored", "[unit][button]")
{
    Button btn;
    btn.setSize(100, 50);
    MockActionListener listener;
    btn.addActionListener(&listener);

    MouseEvent pressEvent(
        &btn, &btn, false, false, false, false, MouseEvent::Type::Pressed, MouseEvent::Button::Right, 50, 25, 0);
    btn.mousePressed(pressEvent);
    REQUIRE(!pressEvent.isConsumed());

    MouseEvent releaseEvent(
        &btn, &btn, false, false, false, false, MouseEvent::Type::Released, MouseEvent::Button::Right, 50, 25, 0);
    btn.mouseReleased(releaseEvent);
    REQUIRE(!listener.actionCalled);
}

TEST_CASE("Button setCaption triggers adjustSize", "[unit][button]")
{
    Button btn;
    btn.setCaption("AdjustMe");
    REQUIRE(btn.getWidth() > 0);
    REQUIRE(btn.getHeight() > 0);
}
