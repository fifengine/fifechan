// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2013 - 2026 Fifengine contributors

// Corresponding header include
#include "fifechan/events/mouseevent.hpp"

// Third-party library includes
#include <catch2/catch_test_macros.hpp>

// Project headers
#include "fifechan/widgets/label.hpp"

TEST_CASE("MouseEvent constructor and getters work correctly", "[mouseevent]")
{
    fcn::Label source;
    fcn::Label distributor;

    SECTION("Pressed event with Left button")
    {
        fcn::MouseEvent const event(
            &source,
            &distributor,
            false,
            false,
            false,
            false,
            fcn::MouseEvent::Type::Pressed,
            fcn::MouseEvent::Button::Left,
            100,
            200,
            1);

        REQUIRE(event.getButton() == fcn::MouseEvent::Button::Left);
        REQUIRE(event.getX() == 100);
        REQUIRE(event.getY() == 200);
        REQUIRE(event.getClickCount() == 1);
        REQUIRE(event.getType() == fcn::MouseEvent::Type::Pressed);
    }

    SECTION("Released event with Right button")
    {
        fcn::MouseEvent const event(
            &source,
            &distributor,
            true,
            false,
            false,
            false,
            fcn::MouseEvent::Type::Released,
            fcn::MouseEvent::Button::Right,
            50,
            75,
            0);

        REQUIRE(event.getButton() == fcn::MouseEvent::Button::Right);
        REQUIRE(event.getX() == 50);
        REQUIRE(event.getY() == 75);
        REQUIRE(event.getClickCount() == 0);
        REQUIRE(event.getType() == fcn::MouseEvent::Type::Released);
    }

    SECTION("Moved event")
    {
        fcn::MouseEvent const event(
            &source,
            &distributor,
            false,
            false,
            false,
            false,
            fcn::MouseEvent::Type::Moved,
            fcn::MouseEvent::Button::Empty,
            300,
            400,
            0);

        REQUIRE(event.getButton() == fcn::MouseEvent::Button::Empty);
        REQUIRE(event.getX() == 300);
        REQUIRE(event.getY() == 400);
        REQUIRE(event.getClickCount() == 0);
        REQUIRE(event.getType() == fcn::MouseEvent::Type::Moved);
    }

    SECTION("WheelMovedUp event")
    {
        fcn::MouseEvent const event(
            &source,
            &distributor,
            false,
            false,
            false,
            false,
            fcn::MouseEvent::Type::WheelMovedUp,
            fcn::MouseEvent::Button::Empty,
            0,
            0,
            0);

        REQUIRE(event.getType() == fcn::MouseEvent::Type::WheelMovedUp);
    }

    SECTION("WheelMovedDown event")
    {
        fcn::MouseEvent const event(
            &source,
            &distributor,
            false,
            false,
            false,
            false,
            fcn::MouseEvent::Type::WheelMovedDown,
            fcn::MouseEvent::Button::Empty,
            0,
            0,
            0);

        REQUIRE(event.getType() == fcn::MouseEvent::Type::WheelMovedDown);
    }

    SECTION("Clicked event with double click")
    {
        fcn::MouseEvent const event(
            &source,
            &distributor,
            false,
            false,
            false,
            false,
            fcn::MouseEvent::Type::Clicked,
            fcn::MouseEvent::Button::Left,
            100,
            100,
            2);

        REQUIRE(event.getType() == fcn::MouseEvent::Type::Clicked);
        REQUIRE(event.getClickCount() == 2);
    }

    SECTION("Entered event")
    {
        fcn::MouseEvent const event(
            &source,
            &distributor,
            false,
            false,
            false,
            false,
            fcn::MouseEvent::Type::Entered,
            fcn::MouseEvent::Button::Empty,
            0,
            0,
            0);

        REQUIRE(event.getType() == fcn::MouseEvent::Type::Entered);
    }

    SECTION("Exited event")
    {
        fcn::MouseEvent const event(
            &source,
            &distributor,
            false,
            false,
            false,
            false,
            fcn::MouseEvent::Type::Exited,
            fcn::MouseEvent::Button::Empty,
            0,
            0,
            0);

        REQUIRE(event.getType() == fcn::MouseEvent::Type::Exited);
    }

    SECTION("Dragged event")
    {
        fcn::MouseEvent const event(
            &source,
            &distributor,
            false,
            false,
            false,
            false,
            fcn::MouseEvent::Type::Dragged,
            fcn::MouseEvent::Button::Left,
            150,
            250,
            0);

        REQUIRE(event.getType() == fcn::MouseEvent::Type::Dragged);
    }
}

TEST_CASE("MouseEvent with modifier keys", "[mouseevent]")
{
    fcn::Label source;
    fcn::Label distributor;

    SECTION("Shift pressed")
    {
        fcn::MouseEvent const event(
            &source,
            &distributor,
            true,
            false,
            false,
            false,
            fcn::MouseEvent::Type::Pressed,
            fcn::MouseEvent::Button::Left,
            10,
            20,
            1);

        REQUIRE(event.isShiftPressed() == true);
        REQUIRE(event.isControlPressed() == false);
        REQUIRE(event.isAltPressed() == false);
        REQUIRE(event.isMetaPressed() == false);
    }

    SECTION("Control and Alt pressed")
    {
        fcn::MouseEvent const event(
            &source,
            &distributor,
            false,
            true,
            true,
            false,
            fcn::MouseEvent::Type::Released,
            fcn::MouseEvent::Button::Right,
            30,
            40,
            0);

        REQUIRE(event.isShiftPressed() == false);
        REQUIRE(event.isControlPressed() == true);
        REQUIRE(event.isAltPressed() == true);
        REQUIRE(event.isMetaPressed() == false);
    }

    SECTION("Meta pressed")
    {
        fcn::MouseEvent const event(
            &source,
            &distributor,
            false,
            false,
            false,
            true,
            fcn::MouseEvent::Type::Moved,
            fcn::MouseEvent::Button::Empty,
            0,
            0,
            0);

        REQUIRE(event.isMetaPressed() == true);
    }
}

TEST_CASE("MouseEvent with Middle and X1 buttons", "[mouseevent]")
{
    fcn::Label source;
    fcn::Label distributor;

    SECTION("Middle button")
    {
        fcn::MouseEvent const event(
            &source,
            &distributor,
            false,
            false,
            false,
            false,
            fcn::MouseEvent::Type::Pressed,
            fcn::MouseEvent::Button::Middle,
            100,
            100,
            1);

        REQUIRE(event.getButton() == fcn::MouseEvent::Button::Middle);
    }

    SECTION("X1 button")
    {
        fcn::MouseEvent const event(
            &source,
            &distributor,
            false,
            false,
            false,
            false,
            fcn::MouseEvent::Type::Pressed,
            fcn::MouseEvent::Button::X1,
            100,
            100,
            1);

        REQUIRE(event.getButton() == fcn::MouseEvent::Button::X1);
    }

    SECTION("X2 button")
    {
        fcn::MouseEvent const event(
            &source,
            &distributor,
            false,
            false,
            false,
            false,
            fcn::MouseEvent::Type::Pressed,
            fcn::MouseEvent::Button::X2,
            100,
            100,
            1);

        REQUIRE(event.getButton() == fcn::MouseEvent::Button::X2);
    }
}
