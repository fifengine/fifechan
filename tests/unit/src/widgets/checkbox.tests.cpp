// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2026 Fifengine contributors

// Standard library includes
#include <string>

// Third-party library includes
#include <catch2/catch_test_macros.hpp>

// Project headers
#include "fifechan/defaultfont.hpp"
#include "fifechan/events/actionevent.hpp"
#include "fifechan/events/keyevent.hpp"
#include "fifechan/events/mouseevent.hpp"
#include "fifechan/key.hpp"
#include "fifechan/listeners/actionlistener.hpp"
#include "fifechan/widgets/checkbox.hpp"

using fcn::ActionEvent;
using fcn::ActionListener;
using fcn::CheckBox;
using fcn::DefaultFont;
using fcn::Key;
using fcn::KeyEvent;
using fcn::MouseEvent;

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

TEST_CASE("CheckBox default constructor", "[unit][checkbox]")
{
    CheckBox cb;
    REQUIRE(!cb.isSelected());
    REQUIRE(cb.getCaption().empty());
    REQUIRE(cb.getMarkerStyle() == CheckBox::MarkerStyle::Checkmark);
}

TEST_CASE("CheckBox caption constructor", "[unit][checkbox]")
{
    CheckBox cb("Check Me", true);
    REQUIRE(cb.getCaption() == "Check Me");
    REQUIRE(cb.isSelected());

    CheckBox cb2("Unchecked", false);
    REQUIRE(!cb2.isSelected());
}

TEST_CASE("CheckBox setSelected and isSelected", "[unit][checkbox]")
{
    CheckBox cb;
    cb.setSelected(true);
    REQUIRE(cb.isSelected());

    cb.setSelected(false);
    REQUIRE(!cb.isSelected());
}

TEST_CASE("CheckBox toggleSelected", "[unit][checkbox]")
{
    CheckBox cb;
    REQUIRE(!cb.isSelected());

    cb.toggleSelected();
    REQUIRE(cb.isSelected());

    cb.toggleSelected();
    REQUIRE(!cb.isSelected());
}

TEST_CASE("CheckBox setMarkerStyle and getMarkerStyle", "[unit][checkbox]")
{
    CheckBox cb;
    REQUIRE(cb.getMarkerStyle() == CheckBox::MarkerStyle::Checkmark);

    cb.setMarkerStyle(CheckBox::MarkerStyle::Cross);
    REQUIRE(cb.getMarkerStyle() == CheckBox::MarkerStyle::Cross);

    cb.setMarkerStyle(CheckBox::MarkerStyle::Dot);
    REQUIRE(cb.getMarkerStyle() == CheckBox::MarkerStyle::Dot);

    cb.setMarkerStyle(CheckBox::MarkerStyle::Rhombus);
    REQUIRE(cb.getMarkerStyle() == CheckBox::MarkerStyle::Rhombus);

    cb.setMarkerStyle(CheckBox::MarkerStyle::Image);
    REQUIRE(cb.getMarkerStyle() == CheckBox::MarkerStyle::Image);
}

TEST_CASE("CheckBox keyPressed Enter toggles", "[unit][checkbox]")
{
    CheckBox cb("Test");
    MockActionListener listener;
    cb.addActionListener(&listener);
    cb.setActionEventId("key_cb");

    Key const enterKey(Key::KEY_RETURN);
    KeyEvent event(&cb, &cb, false, false, false, false, KeyEvent::Type::Pressed, false, enterKey);
    cb.keyPressed(event);
    REQUIRE(cb.isSelected());
    REQUIRE(event.isConsumed());
    REQUIRE(listener.actionCalled);
}

TEST_CASE("CheckBox keyPressed Space toggles", "[unit][checkbox]")
{
    CheckBox cb("Test");
    cb.setSelected(true);

    Key const spaceKey(Key::SPACE);
    KeyEvent event(&cb, &cb, false, false, false, false, KeyEvent::Type::Pressed, false, spaceKey);
    cb.keyPressed(event);
    REQUIRE(!cb.isSelected());
}

TEST_CASE("CheckBox keyReleased consumes event", "[unit][checkbox]")
{
    CheckBox cb;
    Key const enterKey(Key::KEY_RETURN);
    KeyEvent event(&cb, &cb, false, false, false, false, KeyEvent::Type::Released, false, enterKey);
    cb.keyReleased(event);
    REQUIRE(event.isConsumed());
}

TEST_CASE("CheckBox mouseClick toggles", "[unit][checkbox]")
{
    CheckBox cb("Click");
    MockActionListener listener;
    cb.addActionListener(&listener);
    cb.setActionEventId("click_cb");

    MouseEvent clickEvent(
        &cb, &cb, false, false, false, false, MouseEvent::Type::Clicked, MouseEvent::Button::Left, 0, 0, 0);
    cb.mouseClicked(clickEvent);
    REQUIRE(cb.isSelected());
    REQUIRE(clickEvent.isConsumed());
    REQUIRE(listener.actionCalled);
}

TEST_CASE("CheckBox mousePressed/MouseReleased with hasMouse", "[unit][checkbox]")
{
    CheckBox cb;
    cb.setSize(100, 50);

    MouseEvent enterEvent(
        &cb, &cb, false, false, false, false, MouseEvent::Type::Entered, MouseEvent::Button::Empty, 50, 25, 0);
    cb.mouseEntered(enterEvent);

    MouseEvent pressEvent(
        &cb, &cb, false, false, false, false, MouseEvent::Type::Pressed, MouseEvent::Button::Left, 50, 25, 0);
    cb.mousePressed(pressEvent);
    REQUIRE(pressEvent.isConsumed());

    MouseEvent releaseEvent(
        &cb, &cb, false, false, false, false, MouseEvent::Type::Released, MouseEvent::Button::Left, 50, 25, 0);
    cb.mouseReleased(releaseEvent);
    REQUIRE(releaseEvent.isConsumed());
}

TEST_CASE("CheckBox adjustSize with caption", "[unit][checkbox]")
{
    CheckBox cb("Sample");
    DefaultFont font;
    cb.setFont(&font);
    cb.adjustSize();
    REQUIRE(cb.getWidth() > 0);
    REQUIRE(cb.getHeight() > 0);
}

TEST_CASE("CheckBox adjustSize no caption no image", "[unit][checkbox]")
{
    CheckBox cb;
    cb.adjustSize();
    REQUIRE(cb.getWidth() >= 0);
}

TEST_CASE("CheckBox background image getter returns nullptr by default", "[unit][checkbox]")
{
    CheckBox cb;
    REQUIRE(cb.getBackgroundImage() == nullptr);
}
