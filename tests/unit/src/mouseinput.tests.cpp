// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2013 - 2026 Fifengine contributors

// Third-party library includes
#include <catch2/catch_test_macros.hpp>

// Project headers (subdirs before local)
#include <fifechan/mouseinput.hpp>

TEST_CASE("MouseInput constructors initialize properly", "[unit][mouseinput]")
{
    SECTION("default constructor initializes to defaults")
    {
        fcn::MouseInput mouseInput;
        REQUIRE(mouseInput.getType() == fcn::MouseInput::Type::Moved);
        REQUIRE(mouseInput.getButton() == fcn::MouseInput::Button::Empty);
        REQUIRE(mouseInput.getX() == 0);
        REQUIRE(mouseInput.getY() == 0);
        REQUIRE(mouseInput.getTimeStamp() == 0);
    }

    SECTION("parameter constructor initializes all members")
    {
        fcn::MouseInput mouseInput(fcn::MouseInput::Button::Left, fcn::MouseInput::Type::Pressed, 100, 200, 1234);

        REQUIRE(mouseInput.getButton() == fcn::MouseInput::Button::Left);
        REQUIRE(mouseInput.getType() == fcn::MouseInput::Type::Pressed);
        REQUIRE(mouseInput.getX() == 100);
        REQUIRE(mouseInput.getY() == 200);
        REQUIRE(mouseInput.getTimeStamp() == 1234);
    }

    SECTION("all button types")
    {
        REQUIRE(
            fcn::MouseInput(fcn::MouseInput::Button::Left, fcn::MouseInput::Type::Moved, 0, 0, 0).getButton() ==
            fcn::MouseInput::Button::Left);
        REQUIRE(
            fcn::MouseInput(fcn::MouseInput::Button::Right, fcn::MouseInput::Type::Moved, 0, 0, 0).getButton() ==
            fcn::MouseInput::Button::Right);
        REQUIRE(
            fcn::MouseInput(fcn::MouseInput::Button::Middle, fcn::MouseInput::Type::Moved, 0, 0, 0).getButton() ==
            fcn::MouseInput::Button::Middle);
        REQUIRE(
            fcn::MouseInput(fcn::MouseInput::Button::X1, fcn::MouseInput::Type::Moved, 0, 0, 0).getButton() ==
            fcn::MouseInput::Button::X1);
        REQUIRE(
            fcn::MouseInput(fcn::MouseInput::Button::X2, fcn::MouseInput::Type::Moved, 0, 0, 0).getButton() ==
            fcn::MouseInput::Button::X2);
    }

    SECTION("all type values")
    {
        REQUIRE(
            fcn::MouseInput(fcn::MouseInput::Button::Empty, fcn::MouseInput::Type::Moved, 0, 0, 0).getType() ==
            fcn::MouseInput::Type::Moved);
        REQUIRE(
            fcn::MouseInput(fcn::MouseInput::Button::Empty, fcn::MouseInput::Type::Pressed, 0, 0, 0).getType() ==
            fcn::MouseInput::Type::Pressed);
        REQUIRE(
            fcn::MouseInput(fcn::MouseInput::Button::Empty, fcn::MouseInput::Type::Released, 0, 0, 0).getType() ==
            fcn::MouseInput::Type::Released);
        REQUIRE(
            fcn::MouseInput(fcn::MouseInput::Button::Empty, fcn::MouseInput::Type::WheelMovedDown, 0, 0, 0).getType() ==
            fcn::MouseInput::Type::WheelMovedDown);
        REQUIRE(
            fcn::MouseInput(fcn::MouseInput::Button::Empty, fcn::MouseInput::Type::WheelMovedUp, 0, 0, 0).getType() ==
            fcn::MouseInput::Type::WheelMovedUp);
        REQUIRE(
            fcn::MouseInput(fcn::MouseInput::Button::Empty, fcn::MouseInput::Type::WheelMovedRight, 0, 0, 0)
                .getType() == fcn::MouseInput::Type::WheelMovedRight);
        REQUIRE(
            fcn::MouseInput(fcn::MouseInput::Button::Empty, fcn::MouseInput::Type::WheelMovedLeft, 0, 0, 0).getType() ==
            fcn::MouseInput::Type::WheelMovedLeft);
    }
}

TEST_CASE("MouseInput setters update values", "[unit][mouseinput]")
{
    SECTION("setType updates type")
    {
        fcn::MouseInput mouseInput;
        mouseInput.setType(fcn::MouseInput::Type::Pressed);
        REQUIRE(mouseInput.getType() == fcn::MouseInput::Type::Pressed);

        mouseInput.setType(fcn::MouseInput::Type::Released);
        REQUIRE(mouseInput.getType() == fcn::MouseInput::Type::Released);
    }

    SECTION("setButton updates button")
    {
        fcn::MouseInput mouseInput;
        mouseInput.setButton(fcn::MouseInput::Button::Left);
        REQUIRE(mouseInput.getButton() == fcn::MouseInput::Button::Left);

        mouseInput.setButton(fcn::MouseInput::Button::Right);
        REQUIRE(mouseInput.getButton() == fcn::MouseInput::Button::Right);
    }

    SECTION("setTimeStamp updates timestamp")
    {
        fcn::MouseInput mouseInput;
        mouseInput.setTimeStamp(9999);
        REQUIRE(mouseInput.getTimeStamp() == 9999);
    }

    SECTION("setX updates x coordinate")
    {
        fcn::MouseInput mouseInput;
        mouseInput.setX(42);
        REQUIRE(mouseInput.getX() == 42);
    }

    SECTION("setY updates y coordinate")
    {
        fcn::MouseInput mouseInput;
        mouseInput.setY(42);
        REQUIRE(mouseInput.getY() == 42);
    }

    SECTION("negative coordinates")
    {
        fcn::MouseInput mouseInput;
        mouseInput.setX(-100);
        mouseInput.setY(-200);
        REQUIRE(mouseInput.getX() == -100);
        REQUIRE(mouseInput.getY() == -200);
    }

    SECTION("zero coordinates")
    {
        fcn::MouseInput mouseInput;
        mouseInput.setX(0);
        mouseInput.setY(0);
        REQUIRE(mouseInput.getX() == 0);
        REQUIRE(mouseInput.getY() == 0);
    }

    SECTION("large coordinates")
    {
        fcn::MouseInput mouseInput;
        mouseInput.setX(10000);
        mouseInput.setY(10000);
        REQUIRE(mouseInput.getX() == 10000);
        REQUIRE(mouseInput.getY() == 10000);
    }
}

TEST_CASE("MouseInput edge cases", "[unit][mouseinput]")
{
    SECTION("default button is Empty")
    {
        fcn::MouseInput mouseInput;
        REQUIRE(mouseInput.getButton() == fcn::MouseInput::Button::Empty);
    }

    SECTION("default type is Moved")
    {
        fcn::MouseInput mouseInput;
        REQUIRE(mouseInput.getType() == fcn::MouseInput::Type::Moved);
    }

    SECTION("zero timestamp")
    {
        fcn::MouseInput mouseInput;
        REQUIRE(mouseInput.getTimeStamp() == 0);
    }

    SECTION("negative timestamp")
    {
        fcn::MouseInput mouseInput;
        mouseInput.setTimeStamp(-1);
        REQUIRE(mouseInput.getTimeStamp() == -1);
    }

    SECTION("setButton to Empty works")
    {
        fcn::MouseInput mouseInput;
        mouseInput.setButton(fcn::MouseInput::Button::Left);
        mouseInput.setButton(fcn::MouseInput::Button::Empty);
        REQUIRE(mouseInput.getButton() == fcn::MouseInput::Button::Empty);
    }
}

TEST_CASE("MouseInput button enum values", "[unit][mouseinput]")
{
    SECTION("Button enum ordering")
    {
        REQUIRE(static_cast<int>(fcn::MouseInput::Button::Empty) == 0);
        REQUIRE(static_cast<int>(fcn::MouseInput::Button::Left) == 1);
        REQUIRE(static_cast<int>(fcn::MouseInput::Button::Right) == 2);
        REQUIRE(static_cast<int>(fcn::MouseInput::Button::Middle) == 3);
        REQUIRE(static_cast<int>(fcn::MouseInput::Button::X1) == 4);
        REQUIRE(static_cast<int>(fcn::MouseInput::Button::X2) == 5);
    }

    SECTION("Type enum ordering")
    {
        REQUIRE(static_cast<int>(fcn::MouseInput::Type::Moved) == 0);
        REQUIRE(static_cast<int>(fcn::MouseInput::Type::Pressed) == 1);
        REQUIRE(static_cast<int>(fcn::MouseInput::Type::Released) == 2);
        REQUIRE(static_cast<int>(fcn::MouseInput::Type::WheelMovedDown) == 3);
        REQUIRE(static_cast<int>(fcn::MouseInput::Type::WheelMovedUp) == 4);
    }
}