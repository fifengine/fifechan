// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2013 - 2026 Fifengine contributors

// Third-party library includes
#include <catch2/catch_test_macros.hpp>

// Project headers (subdirs before local)
#include <fifechan/events/mouseevent.hpp>
#include <fifechan/widget.hpp>

TEST_CASE("MouseEvent constructors initialize properly", "[unit][mouseevent]")
{
    SECTION("basic constructor")
    {
        fcn::MouseEvent event(
            nullptr,
            nullptr,
            false,
            false,
            false,
            false,
            fcn::MouseEvent::Type::Moved,
            fcn::MouseEvent::Button::Empty,
            0,
            0,
            0);

        REQUIRE(event.getType() == fcn::MouseEvent::Type::Moved);
        REQUIRE(event.getButton() == fcn::MouseEvent::Button::Empty);
        REQUIRE(event.getX() == 0);
        REQUIRE(event.getY() == 0);
        REQUIRE(event.getClickCount() == 0);
    }

    SECTION("full constructor")
    {
        fcn::MouseEvent event(
            nullptr,
            nullptr,
            true,
            false,
            true,
            false,
            fcn::MouseEvent::Type::Pressed,
            fcn::MouseEvent::Button::Left,
            100,
            200,
            3);

        REQUIRE(event.getType() == fcn::MouseEvent::Type::Pressed);
        REQUIRE(event.getButton() == fcn::MouseEvent::Button::Left);
        REQUIRE(event.getX() == 100);
        REQUIRE(event.getY() == 200);
        REQUIRE(event.getClickCount() == 3);
    }

    SECTION("with source and distributor")
    {
        fcn::Widget* source      = reinterpret_cast<fcn::Widget*>(0x1000);
        fcn::Widget* distributor = reinterpret_cast<fcn::Widget*>(0x2000);

        fcn::MouseEvent event(
            source,
            distributor,
            false,
            false,
            false,
            false,
            fcn::MouseEvent::Type::Moved,
            fcn::MouseEvent::Button::Empty,
            50,
            60,
            0);

        REQUIRE(event.getSource() == source);
        REQUIRE(event.getDistributor() == distributor);
    }

    SECTION("with modifiers pressed")
    {
        fcn::MouseEvent event(
            nullptr,
            nullptr,
            true,
            true,
            true,
            true,
            fcn::MouseEvent::Type::Moved,
            fcn::MouseEvent::Button::Empty,
            0,
            0,
            0);

        REQUIRE(event.isShiftPressed() == true);
        REQUIRE(event.isControlPressed() == true);
        REQUIRE(event.isAltPressed() == true);
        REQUIRE(event.isMetaPressed() == true);
    }
}

TEST_CASE("MouseEvent getType returns correct type", "[unit][mouseevent]")
{
    SECTION("Moved type")
    {
        fcn::MouseEvent event(
            nullptr,
            nullptr,
            false,
            false,
            false,
            false,
            fcn::MouseEvent::Type::Moved,
            fcn::MouseEvent::Button::Empty,
            0,
            0,
            0);

        REQUIRE(event.getType() == fcn::MouseEvent::Type::Moved);
    }

    SECTION("Pressed type")
    {
        fcn::MouseEvent event(
            nullptr,
            nullptr,
            false,
            false,
            false,
            false,
            fcn::MouseEvent::Type::Pressed,
            fcn::MouseEvent::Button::Left,
            0,
            0,
            0);

        REQUIRE(event.getType() == fcn::MouseEvent::Type::Pressed);
    }

    SECTION("Released type")
    {
        fcn::MouseEvent event(
            nullptr,
            nullptr,
            false,
            false,
            false,
            false,
            fcn::MouseEvent::Type::Released,
            fcn::MouseEvent::Button::Left,
            0,
            0,
            0);

        REQUIRE(event.getType() == fcn::MouseEvent::Type::Released);
    }

    SECTION("WheelMovedUp type")
    {
        fcn::MouseEvent event(
            nullptr,
            nullptr,
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

    SECTION("Clicked type")
    {
        fcn::MouseEvent event(
            nullptr,
            nullptr,
            false,
            false,
            false,
            false,
            fcn::MouseEvent::Type::Clicked,
            fcn::MouseEvent::Button::Left,
            10,
            20,
            1);

        REQUIRE(event.getType() == fcn::MouseEvent::Type::Clicked);
    }

    SECTION("Entered type")
    {
        fcn::MouseEvent event(
            nullptr,
            nullptr,
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

    SECTION("Exited type")
    {
        fcn::MouseEvent event(
            nullptr,
            nullptr,
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

    SECTION("Dragged type")
    {
        fcn::MouseEvent event(
            nullptr,
            nullptr,
            false,
            false,
            false,
            false,
            fcn::MouseEvent::Type::Dragged,
            fcn::MouseEvent::Button::Left,
            50,
            60,
            0);

        REQUIRE(event.getType() == fcn::MouseEvent::Type::Dragged);
    }
}

TEST_CASE("MouseEvent getButton returns correct button", "[unit][mouseevent]")
{
    SECTION("Empty button")
    {
        fcn::MouseEvent event(
            nullptr,
            nullptr,
            false,
            false,
            false,
            false,
            fcn::MouseEvent::Type::Moved,
            fcn::MouseEvent::Button::Empty,
            0,
            0,
            0);

        REQUIRE(event.getButton() == fcn::MouseEvent::Button::Empty);
    }

    SECTION("Left button")
    {
        fcn::MouseEvent event(
            nullptr,
            nullptr,
            false,
            false,
            false,
            false,
            fcn::MouseEvent::Type::Pressed,
            fcn::MouseEvent::Button::Left,
            0,
            0,
            0);

        REQUIRE(event.getButton() == fcn::MouseEvent::Button::Left);
    }

    SECTION("Middle button")
    {
        fcn::MouseEvent event(
            nullptr,
            nullptr,
            false,
            false,
            false,
            false,
            fcn::MouseEvent::Type::Pressed,
            fcn::MouseEvent::Button::Middle,
            0,
            0,
            0);

        REQUIRE(event.getButton() == fcn::MouseEvent::Button::Middle);
    }

    SECTION("Right button")
    {
        fcn::MouseEvent event(
            nullptr,
            nullptr,
            false,
            false,
            false,
            false,
            fcn::MouseEvent::Type::Pressed,
            fcn::MouseEvent::Button::Right,
            0,
            0,
            0);

        REQUIRE(event.getButton() == fcn::MouseEvent::Button::Right);
    }

    SECTION("X1 button")
    {
        fcn::MouseEvent event(
            nullptr,
            nullptr,
            false,
            false,
            false,
            false,
            fcn::MouseEvent::Type::Pressed,
            fcn::MouseEvent::Button::X1,
            0,
            0,
            0);

        REQUIRE(event.getButton() == fcn::MouseEvent::Button::X1);
    }

    SECTION("X2 button")
    {
        fcn::MouseEvent event(
            nullptr,
            nullptr,
            false,
            false,
            false,
            false,
            fcn::MouseEvent::Type::Pressed,
            fcn::MouseEvent::Button::X2,
            0,
            0,
            0);

        REQUIRE(event.getButton() == fcn::MouseEvent::Button::X2);
    }
}

TEST_CASE("MouseEvent coordinates", "[unit][mouseevent]")
{
    SECTION("zero coordinates")
    {
        fcn::MouseEvent event(
            nullptr,
            nullptr,
            false,
            false,
            false,
            false,
            fcn::MouseEvent::Type::Moved,
            fcn::MouseEvent::Button::Empty,
            0,
            0,
            0);

        REQUIRE(event.getX() == 0);
        REQUIRE(event.getY() == 0);
    }

    SECTION("positive coordinates")
    {
        fcn::MouseEvent event(
            nullptr,
            nullptr,
            false,
            false,
            false,
            false,
            fcn::MouseEvent::Type::Moved,
            fcn::MouseEvent::Button::Empty,
            100,
            200,
            0);

        REQUIRE(event.getX() == 100);
        REQUIRE(event.getY() == 200);
    }

    SECTION("negative coordinates")
    {
        fcn::MouseEvent event(
            nullptr,
            nullptr,
            false,
            false,
            false,
            false,
            fcn::MouseEvent::Type::Moved,
            fcn::MouseEvent::Button::Empty,
            -50,
            -100,
            0);

        REQUIRE(event.getX() == -50);
        REQUIRE(event.getY() == -100);
    }

    SECTION("large coordinates")
    {
        fcn::MouseEvent event(
            nullptr,
            nullptr,
            false,
            false,
            false,
            false,
            fcn::MouseEvent::Type::Moved,
            fcn::MouseEvent::Button::Empty,
            10000,
            20000,
            0);

        REQUIRE(event.getX() == 10000);
        REQUIRE(event.getY() == 20000);
    }
}

TEST_CASE("MouseEvent getClickCount", "[unit][mouseevent]")
{
    SECTION("zero click count")
    {
        fcn::MouseEvent event(
            nullptr,
            nullptr,
            false,
            false,
            false,
            false,
            fcn::MouseEvent::Type::Moved,
            fcn::MouseEvent::Button::Empty,
            0,
            0,
            0);

        REQUIRE(event.getClickCount() == 0);
    }

    SECTION("single click")
    {
        fcn::MouseEvent event(
            nullptr,
            nullptr,
            false,
            false,
            false,
            false,
            fcn::MouseEvent::Type::Clicked,
            fcn::MouseEvent::Button::Left,
            0,
            0,
            1);

        REQUIRE(event.getClickCount() == 1);
    }

    SECTION("double click")
    {
        fcn::MouseEvent event(
            nullptr,
            nullptr,
            false,
            false,
            false,
            false,
            fcn::MouseEvent::Type::Clicked,
            fcn::MouseEvent::Button::Left,
            0,
            0,
            2);

        REQUIRE(event.getClickCount() == 2);
    }

    SECTION("triple click")
    {
        fcn::MouseEvent event(
            nullptr,
            nullptr,
            false,
            false,
            false,
            false,
            fcn::MouseEvent::Type::Clicked,
            fcn::MouseEvent::Button::Left,
            0,
            0,
            3);

        REQUIRE(event.getClickCount() == 3);
    }
}

TEST_CASE("MouseEvent inherits from InputEvent", "[unit][mouseevent]")
{
    SECTION("consume works")
    {
        fcn::MouseEvent event(
            nullptr,
            nullptr,
            false,
            false,
            false,
            false,
            fcn::MouseEvent::Type::Moved,
            fcn::MouseEvent::Button::Empty,
            0,
            0,
            0);

        REQUIRE(event.isConsumed() == false);
        event.consume();
        REQUIRE(event.isConsumed() == true);
    }
}

TEST_CASE("MouseEvent Type enum values", "[unit][mouseevent]")
{
    SECTION("enum ordering")
    {
        // cppcheck-suppress knownConditionTrueFalse
        REQUIRE(static_cast<int>(fcn::MouseEvent::Type::Moved) == 0);
        // cppcheck-suppress knownConditionTrueFalse
        REQUIRE(static_cast<int>(fcn::MouseEvent::Type::Pressed) == 1);
        // cppcheck-suppress knownConditionTrueFalse
        REQUIRE(static_cast<int>(fcn::MouseEvent::Type::Released) == 2);
        // cppcheck-suppress knownConditionTrueFalse
        REQUIRE(static_cast<int>(fcn::MouseEvent::Type::WheelMovedDown) == 3);
    }
}

TEST_CASE("MouseEvent Button enum values", "[unit][mouseevent]")
{
    SECTION("enum ordering")
    {
        // cppcheck-suppress knownConditionTrueFalse
        REQUIRE(static_cast<int>(fcn::MouseEvent::Button::Empty) == 0);
        // cppcheck-suppress knownConditionTrueFalse
        REQUIRE(static_cast<int>(fcn::MouseEvent::Button::Left) == 1);
        // cppcheck-suppress knownConditionTrueFalse
        REQUIRE(static_cast<int>(fcn::MouseEvent::Button::Middle) == 2);
        // cppcheck-suppress knownConditionTrueFalse
        REQUIRE(static_cast<int>(fcn::MouseEvent::Button::Right) == 3);
        // cppcheck-suppress knownConditionTrueFalse
        REQUIRE(static_cast<int>(fcn::MouseEvent::Button::X1) == 4);
        // cppcheck-suppress knownConditionTrueFalse
        REQUIRE(static_cast<int>(fcn::MouseEvent::Button::X2) == 5);
    }
}