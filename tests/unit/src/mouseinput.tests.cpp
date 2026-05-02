// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2026 Fifengine contributors

#include "fifechan/mouseinput.hpp"

#include <catch2/catch_test_macros.hpp>

TEST_CASE("MouseInput can be constructed with parameters", "[mouseinput]")
{
    fcn::MouseInput input(fcn::MouseInput::Button::Left, fcn::MouseInput::Type::Pressed, 100, 200, 12345);

    REQUIRE(input.getButton() == fcn::MouseInput::Button::Left);
    REQUIRE(input.getType() == fcn::MouseInput::Type::Pressed);
    REQUIRE(input.getX() == 100);
    REQUIRE(input.getY() == 200);
    REQUIRE(input.getTimeStamp() == 12345);
}

TEST_CASE("MouseInput default constructor initializes to defaults", "[mouseinput]")
{
    fcn::MouseInput input;

    REQUIRE(input.getButton() == fcn::MouseInput::Button::Empty);
    REQUIRE(input.getType() == fcn::MouseInput::Type::Moved);
    REQUIRE(input.getX() == 0);
    REQUIRE(input.getY() == 0);
    REQUIRE(input.getTimeStamp() == 0);
}

TEST_CASE("MouseInput setType and getType work correctly", "[mouseinput]")
{
    fcn::MouseInput input;

    input.setType(fcn::MouseInput::Type::Pressed);
    REQUIRE(input.getType() == fcn::MouseInput::Type::Pressed);

    input.setType(fcn::MouseInput::Type::Released);
    REQUIRE(input.getType() == fcn::MouseInput::Type::Released);

    input.setType(fcn::MouseInput::Type::Moved);
    REQUIRE(input.getType() == fcn::MouseInput::Type::Moved);

    input.setType(fcn::MouseInput::Type::WheelMovedDown);
    REQUIRE(input.getType() == fcn::MouseInput::Type::WheelMovedDown);

    input.setType(fcn::MouseInput::Type::WheelMovedUp);
    REQUIRE(input.getType() == fcn::MouseInput::Type::WheelMovedUp);

    input.setType(fcn::MouseInput::Type::WheelMovedRight);
    REQUIRE(input.getType() == fcn::MouseInput::Type::WheelMovedRight);

    input.setType(fcn::MouseInput::Type::WheelMovedLeft);
    REQUIRE(input.getType() == fcn::MouseInput::Type::WheelMovedLeft);
}

TEST_CASE("MouseInput setButton and getButton work correctly", "[mouseinput]")
{
    fcn::MouseInput input;

    input.setButton(fcn::MouseInput::Button::Left);
    REQUIRE(input.getButton() == fcn::MouseInput::Button::Left);

    input.setButton(fcn::MouseInput::Button::Right);
    REQUIRE(input.getButton() == fcn::MouseInput::Button::Right);

    input.setButton(fcn::MouseInput::Button::Middle);
    REQUIRE(input.getButton() == fcn::MouseInput::Button::Middle);

    input.setButton(fcn::MouseInput::Button::X1);
    REQUIRE(input.getButton() == fcn::MouseInput::Button::X1);

    input.setButton(fcn::MouseInput::Button::X2);
    REQUIRE(input.getButton() == fcn::MouseInput::Button::X2);

    input.setButton(fcn::MouseInput::Button::Empty);
    REQUIRE(input.getButton() == fcn::MouseInput::Button::Empty);
}

TEST_CASE("MouseInput setTimeStamp and getTimeStamp work correctly", "[mouseinput]")
{
    fcn::MouseInput input;

    input.setTimeStamp(1000);
    REQUIRE(input.getTimeStamp() == 1000);

    input.setTimeStamp(0);
    REQUIRE(input.getTimeStamp() == 0);

    input.setTimeStamp(9999999);
    REQUIRE(input.getTimeStamp() == 9999999);
}

TEST_CASE("MouseInput setX and getX work correctly", "[mouseinput]")
{
    fcn::MouseInput input;

    input.setX(50);
    REQUIRE(input.getX() == 50);

    input.setX(-25);
    REQUIRE(input.getX() == -25);

    input.setX(0);
    REQUIRE(input.getX() == 0);

    input.setX(1920);
    REQUIRE(input.getX() == 1920);
}

TEST_CASE("MouseInput setY and getY work correctly", "[mouseinput]")
{
    fcn::MouseInput input;

    input.setY(100);
    REQUIRE(input.getY() == 100);

    input.setY(-50);
    REQUIRE(input.getY() == -50);

    input.setY(0);
    REQUIRE(input.getY() == 0);

    input.setY(1080);
    REQUIRE(input.getY() == 1080);
}

TEST_CASE("MouseInput can be constructed with all button types", "[mouseinput]")
{
    fcn::MouseInput input1(fcn::MouseInput::Button::Left, fcn::MouseInput::Type::Pressed, 0, 0, 0);
    REQUIRE(input1.getButton() == fcn::MouseInput::Button::Left);

    fcn::MouseInput input2(fcn::MouseInput::Button::Right, fcn::MouseInput::Type::Pressed, 0, 0, 0);
    REQUIRE(input2.getButton() == fcn::MouseInput::Button::Right);

    fcn::MouseInput input3(fcn::MouseInput::Button::Middle, fcn::MouseInput::Type::Pressed, 0, 0, 0);
    REQUIRE(input3.getButton() == fcn::MouseInput::Button::Middle);

    fcn::MouseInput input4(fcn::MouseInput::Button::X1, fcn::MouseInput::Type::Pressed, 0, 0, 0);
    REQUIRE(input4.getButton() == fcn::MouseInput::Button::X1);

    fcn::MouseInput input5(fcn::MouseInput::Button::X2, fcn::MouseInput::Type::Pressed, 0, 0, 0);
    REQUIRE(input5.getButton() == fcn::MouseInput::Button::X2);

    fcn::MouseInput input6(fcn::MouseInput::Button::Empty, fcn::MouseInput::Type::Pressed, 0, 0, 0);
    REQUIRE(input6.getButton() == fcn::MouseInput::Button::Empty);
}

TEST_CASE("MouseInput can be constructed with all type types", "[mouseinput]")
{
    fcn::MouseInput input1(fcn::MouseInput::Button::Left, fcn::MouseInput::Type::Moved, 0, 0, 0);
    REQUIRE(input1.getType() == fcn::MouseInput::Type::Moved);

    fcn::MouseInput input2(fcn::MouseInput::Button::Left, fcn::MouseInput::Type::Pressed, 0, 0, 0);
    REQUIRE(input2.getType() == fcn::MouseInput::Type::Pressed);

    fcn::MouseInput input3(fcn::MouseInput::Button::Left, fcn::MouseInput::Type::Released, 0, 0, 0);
    REQUIRE(input3.getType() == fcn::MouseInput::Type::Released);

    fcn::MouseInput input4(fcn::MouseInput::Button::Left, fcn::MouseInput::Type::WheelMovedDown, 0, 0, 0);
    REQUIRE(input4.getType() == fcn::MouseInput::Type::WheelMovedDown);

    fcn::MouseInput input5(fcn::MouseInput::Button::Left, fcn::MouseInput::Type::WheelMovedUp, 0, 0, 0);
    REQUIRE(input5.getType() == fcn::MouseInput::Type::WheelMovedUp);

    fcn::MouseInput input6(fcn::MouseInput::Button::Left, fcn::MouseInput::Type::WheelMovedRight, 0, 0, 0);
    REQUIRE(input6.getType() == fcn::MouseInput::Type::WheelMovedRight);

    fcn::MouseInput input7(fcn::MouseInput::Button::Left, fcn::MouseInput::Type::WheelMovedLeft, 0, 0, 0);
    REQUIRE(input7.getType() == fcn::MouseInput::Type::WheelMovedLeft);
}
