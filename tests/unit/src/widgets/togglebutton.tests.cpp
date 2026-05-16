// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2026 Fifengine contributors

// Standard library includes
#include <string>

// Third-party library includes
#include <catch2/catch_test_macros.hpp>

// Project headers
#include "fifechan/events/actionevent.hpp"
#include "fifechan/events/keyevent.hpp"
#include "fifechan/events/mouseevent.hpp"
#include "fifechan/key.hpp"
#include "fifechan/listeners/actionlistener.hpp"
#include "fifechan/widgets/togglebutton.hpp"

using fcn::ActionEvent;
using fcn::ActionListener;
using fcn::Key;
using fcn::KeyEvent;
using fcn::MouseEvent;
using fcn::ToggleButton;

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

TEST_CASE("ToggleButton default constructor", "[unit][togglebutton]")
{
    ToggleButton btn;
    REQUIRE(!btn.isSelected());
    REQUIRE(btn.getGroup().empty());
    REQUIRE(btn.getCaption().empty());
}

TEST_CASE("ToggleButton parameterized constructor", "[unit][togglebutton]")
{
    ToggleButton btn("Option1", "group1", true);
    REQUIRE(btn.getCaption() == "Option1");
    REQUIRE(btn.getGroup() == "group1");
    REQUIRE(btn.isSelected());
}

TEST_CASE("ToggleButton parameterized constructor default not selected", "[unit][togglebutton]")
{
    ToggleButton btn("Option2", "group2");
    REQUIRE(!btn.isSelected());
}

TEST_CASE("ToggleButton setSelected", "[unit][togglebutton]")
{
    ToggleButton btn;
    btn.setSelected(true);
    REQUIRE(btn.isSelected());

    btn.setSelected(false);
    REQUIRE(!btn.isSelected());
}

TEST_CASE("ToggleButton toggleSelected", "[unit][togglebutton]")
{
    ToggleButton btn;
    REQUIRE(!btn.isSelected());

    btn.toggleSelected();
    REQUIRE(btn.isSelected());

    btn.toggleSelected();
    REQUIRE(!btn.isSelected());
}

TEST_CASE("ToggleButton toggleSelected fires action event", "[unit][togglebutton]")
{
    ToggleButton btn;
    MockActionListener listener;
    btn.addActionListener(&listener);
    btn.setActionEventId("toggle");

    btn.toggleSelected();
    REQUIRE(listener.actionCalled);
}

TEST_CASE("ToggleButton setGroup and getGroup", "[unit][togglebutton]")
{
    ToggleButton btn;
    btn.setGroup("mygroup");
    REQUIRE(btn.getGroup() == "mygroup");

    btn.setGroup("");
    REQUIRE(btn.getGroup().empty());
}

TEST_CASE("ToggleButton group selection mutual exclusion", "[unit][togglebutton]")
{
    ToggleButton btn1("A", "groupX");
    ToggleButton btn2("B", "groupX");
    ToggleButton btn3("C", "groupX");

    REQUIRE(!btn1.isSelected());
    REQUIRE(!btn2.isSelected());
    REQUIRE(!btn3.isSelected());

    btn1.setSelected(true);
    REQUIRE(btn1.isSelected());
    REQUIRE(!btn2.isSelected());
    REQUIRE(!btn3.isSelected());

    btn2.setSelected(true);
    REQUIRE(!btn1.isSelected());
    REQUIRE(btn2.isSelected());
    REQUIRE(!btn3.isSelected());
}

TEST_CASE("ToggleButton non-grouped select does not affect others", "[unit][togglebutton]")
{
    ToggleButton btn1("A", "groupA");
    ToggleButton btn2("B", "groupB");

    btn1.setSelected(true);
    REQUIRE(btn1.isSelected());

    btn2.setSelected(true);
    REQUIRE(btn2.isSelected());
    REQUIRE(btn1.isSelected());
}

TEST_CASE("ToggleButton keyReleased triggers toggle", "[unit][togglebutton]")
{
    ToggleButton btn("Test", "groupK", false);
    MockActionListener listener;
    btn.addActionListener(&listener);
    btn.setActionEventId("key_toggle");

    // Simulate key press to set mKeyPressed
    Key const enterKey(Key::KEY_RETURN);
    KeyEvent pressEvent(&btn, &btn, false, false, false, false, KeyEvent::Type::Pressed, false, enterKey);
    btn.keyPressed(pressEvent);

    // Now release
    KeyEvent releaseEvent(&btn, &btn, false, false, false, false, KeyEvent::Type::Released, false, enterKey);
    btn.keyReleased(releaseEvent);
    REQUIRE(btn.isSelected());
    REQUIRE(listener.actionCalled);
}

TEST_CASE("ToggleButton keyReleased space key", "[unit][togglebutton]")
{
    ToggleButton btn;
    btn.setSelected(true);

    Key const spaceKey(Key::SPACE);
    KeyEvent pressEvent(&btn, &btn, false, false, false, false, KeyEvent::Type::Pressed, false, spaceKey);
    btn.keyPressed(pressEvent);

    KeyEvent releaseEvent(&btn, &btn, false, false, false, false, KeyEvent::Type::Released, false, spaceKey);
    btn.keyReleased(releaseEvent);
    REQUIRE(!btn.isSelected());
}

TEST_CASE("ToggleButton mouseReleased toggles selection", "[unit][togglebutton]")
{
    ToggleButton btn;
    btn.setSize(100, 50);
    MockActionListener listener;
    btn.addActionListener(&listener);

    MouseEvent enterEvent(
        &btn, &btn, false, false, false, false, MouseEvent::Type::Entered, MouseEvent::Button::Empty, 50, 25, 0);
    btn.mouseEntered(enterEvent);

    MouseEvent pressEvent(
        &btn, &btn, false, false, false, false, MouseEvent::Type::Pressed, MouseEvent::Button::Left, 50, 25, 0);
    btn.mousePressed(pressEvent);

    MouseEvent releaseEvent(
        &btn, &btn, false, false, false, false, MouseEvent::Type::Released, MouseEvent::Button::Left, 50, 25, 0);
    btn.mouseReleased(releaseEvent);
    REQUIRE(btn.isSelected());
    REQUIRE(listener.actionCalled);
}

TEST_CASE("ToggleButton mouseReleased without mouse does not toggle", "[unit][togglebutton]")
{
    ToggleButton btn;
    REQUIRE(!btn.isSelected());

    MouseEvent pressEvent(
        &btn, &btn, false, false, false, false, MouseEvent::Type::Pressed, MouseEvent::Button::Left, 0, 0, 0);
    btn.mousePressed(pressEvent);

    MouseEvent releaseEvent(
        &btn, &btn, false, false, false, false, MouseEvent::Type::Released, MouseEvent::Button::Left, 0, 0, 0);
    btn.mouseReleased(releaseEvent);
    REQUIRE(!btn.isSelected());
}

TEST_CASE("ToggleButton destructor removes from group", "[unit][togglebutton]")
{
    auto btn1 = std::make_unique<ToggleButton>("A", "groupDestruct");
    {
        ToggleButton btn2("B", "groupDestruct", true);
        REQUIRE(btn2.isSelected());
        REQUIRE(!btn1->isSelected());

        btn1->setSelected(true);
        REQUIRE(btn1->isSelected());
        REQUIRE(!btn2.isSelected());
    }
    // btn2 destroyed, btn1 still has its selection state
    REQUIRE(btn1->isSelected());
    btn1->setSelected(false);
    REQUIRE(!btn1->isSelected());
}
