// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2026 Fifengine contributors

// Standard library includes
#include <cmath>

// Third-party library includes
#include <catch2/catch_test_macros.hpp>

// Project headers
#include "fifechan/color.hpp"
#include "fifechan/events/actionevent.hpp"
#include "fifechan/events/keyevent.hpp"
#include "fifechan/events/mouseevent.hpp"
#include "fifechan/key.hpp"
#include "fifechan/listeners/actionlistener.hpp"
#include "fifechan/widgets/slider.hpp"

using fcn::ActionEvent;
using fcn::ActionListener;
using fcn::Color;
using fcn::Key;
using fcn::KeyEvent;
using fcn::MouseEvent;
using fcn::Slider;

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

TEST_CASE("Slider default constructor", "[unit][slider]")
{
    Slider slider;
    REQUIRE(slider.getScaleStart() == 0.0);
    REQUIRE(slider.getScaleEnd() == 1.0);
    REQUIRE(slider.getValue() == 0.0);
    REQUIRE(slider.getOrientation() == Slider::Orientation::Horizontal);
    REQUIRE(slider.getMarkerLength() == 10);
    REQUIRE(std::abs(slider.getStepLength() - 0.1) < 1e-9);
    REQUIRE(slider.isFocusable());
    REQUIRE(slider.getBorderSize() == 1);
}

TEST_CASE("Slider scale-end constructor", "[unit][slider]")
{
    Slider slider(5.0);
    REQUIRE(slider.getScaleStart() == 0.0);
    REQUIRE(slider.getScaleEnd() == 5.0);
    REQUIRE(slider.getValue() == 0.0);
    REQUIRE(std::abs(slider.getStepLength() - 0.5) < 1e-9);
}

TEST_CASE("Slider scale-range constructor", "[unit][slider]")
{
    Slider slider(10.0, 100.0);
    REQUIRE(slider.getScaleStart() == 10.0);
    REQUIRE(slider.getScaleEnd() == 100.0);
    REQUIRE(slider.getValue() == 10.0);
    REQUIRE(std::abs(slider.getStepLength() - 9.0) < 1e-9);
}

TEST_CASE("Slider setScale", "[unit][slider]")
{
    Slider slider(0.0, 100.0);
    slider.setValue(50.0);
    REQUIRE(slider.getValue() == 50.0);

    slider.setScale(20.0, 80.0);
    REQUIRE(slider.getValue() == 50.0);

    slider.setScale(60.0, 200.0);
    REQUIRE(slider.getValue() == 60.0);
}

TEST_CASE("Slider setScaleStart", "[unit][slider]")
{
    Slider slider(0.0, 100.0);
    slider.setValue(50.0);
    slider.setScaleStart(60.0);
    REQUIRE(slider.getValue() == 60.0);
    REQUIRE(slider.getScaleStart() == 60.0);
}

TEST_CASE("Slider setScaleEnd", "[unit][slider]")
{
    Slider slider(0.0, 100.0);
    slider.setValue(80.0);
    slider.setScaleEnd(50.0);
    REQUIRE(slider.getValue() == 50.0);
    REQUIRE(slider.getScaleEnd() == 50.0);
}

TEST_CASE("Slider getScaleStart and getScaleEnd", "[unit][slider]")
{
    Slider slider;
    REQUIRE(slider.getScaleStart() == 0.0);
    REQUIRE(slider.getScaleEnd() == 1.0);

    slider.setScaleStart(-5.0);
    REQUIRE(slider.getScaleStart() == -5.0);

    slider.setScaleEnd(20.0);
    REQUIRE(slider.getScaleEnd() == 20.0);
}

TEST_CASE("Slider setValue", "[unit][slider]")
{
    Slider slider(0.0, 10.0);

    slider.setValue(5.0);
    REQUIRE(slider.getValue() == 5.0);

    slider.setValue(-1.0);
    REQUIRE(slider.getValue() == 0.0);

    slider.setValue(15.0);
    REQUIRE(slider.getValue() == 10.0);

    slider.setValue(7.5);
    REQUIRE(slider.getValue() == 7.5);
}

TEST_CASE("Slider setMarkerLength and getMarkerLength", "[unit][slider]")
{
    Slider slider;
    slider.setMarkerLength(20);
    REQUIRE(slider.getMarkerLength() == 20);

    slider.setMarkerLength(0);
    REQUIRE(slider.getMarkerLength() == 0);

    slider.setMarkerLength(-5);
    REQUIRE(slider.getMarkerLength() == -5);
}

TEST_CASE("Slider setOrientation and getOrientation", "[unit][slider]")
{
    Slider slider;
    REQUIRE(slider.getOrientation() == Slider::Orientation::Horizontal);

    slider.setOrientation(Slider::Orientation::Vertical);
    REQUIRE(slider.getOrientation() == Slider::Orientation::Vertical);

    slider.setOrientation(Slider::Orientation::Horizontal);
    REQUIRE(slider.getOrientation() == Slider::Orientation::Horizontal);
}

TEST_CASE("Slider setStepLength and getStepLength", "[unit][slider]")
{
    Slider slider;
    slider.setStepLength(2.5);
    REQUIRE(slider.getStepLength() == 2.5);

    slider.setStepLength(0.0);
    REQUIRE(slider.getStepLength() == 0.0);

    slider.setStepLength(-1.0);
    REQUIRE(slider.getStepLength() == -1.0);
}

TEST_CASE("Slider keyPressed LEFT decrements value", "[unit][slider]")
{
    Slider slider(0.0, 100.0);
    slider.setValue(50.0);
    MockActionListener listener;
    slider.addActionListener(&listener);
    slider.setActionEventId("slider");

    Key const leftKey(Key::LEFT);
    KeyEvent event(&slider, &slider, false, false, false, false, KeyEvent::Type::Pressed, false, leftKey);
    slider.keyPressed(event);
    REQUIRE(slider.getValue() < 50.0);
    REQUIRE(event.isConsumed());
    REQUIRE(listener.actionCalled);
}

TEST_CASE("Slider keyPressed RIGHT increments value", "[unit][slider]")
{
    Slider slider(0.0, 100.0);
    slider.setValue(50.0);
    MockActionListener listener;
    slider.addActionListener(&listener);

    Key const rightKey(Key::RIGHT);
    KeyEvent event(&slider, &slider, false, false, false, false, KeyEvent::Type::Pressed, false, rightKey);
    slider.keyPressed(event);
    REQUIRE(slider.getValue() > 50.0);
}

TEST_CASE("Slider keyPressed UP/DOWN in vertical orientation", "[unit][slider]")
{
    Slider slider(0.0, 100.0);
    slider.setOrientation(Slider::Orientation::Vertical);
    slider.setValue(50.0);

    Key const upKey(Key::UP);
    KeyEvent upEvent(&slider, &slider, false, false, false, false, KeyEvent::Type::Pressed, false, upKey);
    slider.keyPressed(upEvent);
    REQUIRE(slider.getValue() > 50.0);

    slider.setValue(50.0);

    Key const downKey(Key::DOWN);
    KeyEvent downEvent(&slider, &slider, false, false, false, false, KeyEvent::Type::Pressed, false, downKey);
    slider.keyPressed(downEvent);
    REQUIRE(slider.getValue() < 50.0);
}

TEST_CASE("Slider keyPressed UP/DOWN in horizontal ignored", "[unit][slider]")
{
    Slider slider(0.0, 100.0);
    slider.setValue(50.0);

    Key const upKey(Key::UP);
    KeyEvent upEvent(&slider, &slider, false, false, false, false, KeyEvent::Type::Pressed, false, upKey);
    slider.keyPressed(upEvent);
    REQUIRE(slider.getValue() == 50.0);

    Key const downKey(Key::DOWN);
    KeyEvent downEvent(&slider, &slider, false, false, false, false, KeyEvent::Type::Pressed, false, downKey);
    slider.keyPressed(downEvent);
    REQUIRE(slider.getValue() == 50.0);
}

TEST_CASE("Slider keyPressed LEFT/RIGHT in vertical ignored", "[unit][slider]")
{
    Slider slider(0.0, 100.0);
    slider.setOrientation(Slider::Orientation::Vertical);
    slider.setValue(50.0);

    Key const leftKey(Key::LEFT);
    KeyEvent leftEvent(&slider, &slider, false, false, false, false, KeyEvent::Type::Pressed, false, leftKey);
    slider.keyPressed(leftEvent);
    REQUIRE(slider.getValue() == 50.0);

    Key const rightKey(Key::RIGHT);
    KeyEvent rightEvent(&slider, &slider, false, false, false, false, KeyEvent::Type::Pressed, false, rightKey);
    slider.keyPressed(rightEvent);
    REQUIRE(slider.getValue() == 50.0);
}

TEST_CASE("Slider mousePressed sets value horizontally", "[unit][slider]")
{
    Slider slider(0.0, 100.0);
    slider.setSize(200, 20);
    slider.setMarkerLength(20);

    MouseEvent pressEvent(
        &slider, &slider, false, false, false, false, MouseEvent::Type::Pressed, MouseEvent::Button::Left, 100, 10, 0);
    slider.mousePressed(pressEvent);
    REQUIRE(slider.getValue() > 0.0);
}

TEST_CASE("Slider mousePressed sets value vertically", "[unit][slider]")
{
    Slider slider(0.0, 100.0);
    slider.setOrientation(Slider::Orientation::Vertical);
    slider.setSize(20, 200);
    slider.setMarkerLength(20);

    MouseEvent pressEvent(
        &slider, &slider, false, false, false, false, MouseEvent::Type::Pressed, MouseEvent::Button::Left, 10, 100, 0);
    slider.mousePressed(pressEvent);
    REQUIRE(slider.getValue() > 0.0);
}

TEST_CASE("Slider mouseDragged changes value", "[unit][slider]")
{
    Slider slider(0.0, 100.0);
    slider.setSize(200, 20);
    slider.setMarkerLength(20);

    MouseEvent dragEvent(
        &slider, &slider, false, false, false, false, MouseEvent::Type::Dragged, MouseEvent::Button::Left, 150, 10, 0);
    slider.mouseDragged(dragEvent);
    REQUIRE(slider.getValue() > 0.0);
    REQUIRE(dragEvent.isConsumed());
}

TEST_CASE("Slider mouseWheelMovedUp/MovedDown for vertical", "[unit][slider]")
{
    Slider slider(0.0, 100.0);
    slider.setOrientation(Slider::Orientation::Vertical);
    slider.setValue(50.0);

    MouseEvent wheelUpEvent(
        &slider,
        &slider,
        false,
        false,
        false,
        false,
        MouseEvent::Type::WheelMovedUp,
        MouseEvent::Button::Empty,
        0,
        0,
        0);
    slider.mouseWheelMovedUp(wheelUpEvent);
    REQUIRE(slider.getValue() > 50.0);
    REQUIRE(wheelUpEvent.isConsumed());

    slider.setValue(50.0);

    MouseEvent wheelDownEvent(
        &slider,
        &slider,
        false,
        false,
        false,
        false,
        MouseEvent::Type::WheelMovedDown,
        MouseEvent::Button::Empty,
        0,
        0,
        0);
    slider.mouseWheelMovedDown(wheelDownEvent);
    REQUIRE(slider.getValue() < 50.0);
    REQUIRE(wheelDownEvent.isConsumed());
}

TEST_CASE("Slider mouseWheelMovedUp/MovedDown for horizontal ignored", "[unit][slider]")
{
    Slider slider(0.0, 100.0);
    slider.setValue(50.0);
    REQUIRE(slider.getOrientation() == Slider::Orientation::Horizontal);

    MouseEvent wheelUpEvent(
        &slider,
        &slider,
        false,
        false,
        false,
        false,
        MouseEvent::Type::WheelMovedUp,
        MouseEvent::Button::Empty,
        0,
        0,
        0);
    slider.mouseWheelMovedUp(wheelUpEvent);
    REQUIRE(slider.getValue() == 50.0);

    MouseEvent wheelDownEvent(
        &slider,
        &slider,
        false,
        false,
        false,
        false,
        MouseEvent::Type::WheelMovedDown,
        MouseEvent::Button::Empty,
        0,
        0,
        0);
    slider.mouseWheelMovedDown(wheelDownEvent);
    REQUIRE(slider.getValue() == 50.0);
}

TEST_CASE("Slider mouseWheelMovedRight/MovedLeft for horizontal", "[unit][slider]")
{
    Slider slider(0.0, 100.0);
    slider.setValue(50.0);

    MouseEvent wheelRightEvent(
        &slider,
        &slider,
        false,
        false,
        false,
        false,
        MouseEvent::Type::WheelMovedRight,
        MouseEvent::Button::Empty,
        0,
        0,
        0);
    slider.mouseWheelMovedRight(wheelRightEvent);
    REQUIRE(slider.getValue() > 50.0);
    REQUIRE(wheelRightEvent.isConsumed());

    slider.setValue(50.0);

    MouseEvent wheelLeftEvent(
        &slider,
        &slider,
        false,
        false,
        false,
        false,
        MouseEvent::Type::WheelMovedLeft,
        MouseEvent::Button::Empty,
        0,
        0,
        0);
    slider.mouseWheelMovedLeft(wheelLeftEvent);
    REQUIRE(slider.getValue() < 50.0);
    REQUIRE(wheelLeftEvent.isConsumed());
}

TEST_CASE("Slider clamping at scale boundaries", "[unit][slider]")
{
    Slider slider(0.0, 10.0);
    slider.setValue(-5.0);
    REQUIRE(slider.getValue() == 0.0);

    slider.setValue(20.0);
    REQUIRE(slider.getValue() == 10.0);

    slider.setScaleStart(3.0);
    slider.setScaleEnd(7.0);
    REQUIRE(slider.getValue() == 7.0);
}
