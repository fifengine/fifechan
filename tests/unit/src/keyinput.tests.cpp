// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2026 Fifengine contributors

// Third-party library includes
#include <catch2/catch_test_macros.hpp>

// Project headers (subdirs before local)
#include <fifechan/key.hpp>
#include <fifechan/keyinput.hpp>

TEST_CASE("KeyInput constructors initialize properly", "[unit][keyinput]")
{
    SECTION("default constructor initializes members")
    {
        fcn::KeyInput const keyInput;
        REQUIRE(keyInput.getKey().getValue() == 0);
        REQUIRE(keyInput.getType() == fcn::KeyInput::Type::Pressed);
    }

    SECTION("key and type constructor")
    {
        fcn::Key key(65);
        fcn::KeyInput const keyInput(key, fcn::KeyInput::Type::Released);
        REQUIRE(keyInput.getKey() == key);
        REQUIRE(keyInput.getType() == fcn::KeyInput::Type::Released);
    }
}

TEST_CASE("KeyInput getters and setters", "[unit][keyinput]")
{
    SECTION("setKey updates the key")
    {
        fcn::KeyInput keyInput;
        fcn::Key key(65);
        keyInput.setKey(key);
        REQUIRE(keyInput.getKey() == key);
    }

    SECTION("setType updates the type")
    {
        fcn::KeyInput keyInput;
        keyInput.setType(fcn::KeyInput::Type::Pressed);
        REQUIRE(keyInput.getType() == fcn::KeyInput::Type::Pressed);

        keyInput.setType(fcn::KeyInput::Type::Released);
        REQUIRE(keyInput.getType() == fcn::KeyInput::Type::Released);
    }

    SECTION("all types are settable")
    {
        fcn::KeyInput keyInput;
        keyInput.setType(fcn::KeyInput::Type::Pressed);
        REQUIRE(keyInput.getType() == fcn::KeyInput::Type::Pressed);

        keyInput.setType(fcn::KeyInput::Type::Released);
        REQUIRE(keyInput.getType() == fcn::KeyInput::Type::Released);
    }
}

TEST_CASE("KeyInput modifier keys state", "[unit][keyinput]")
{
    SECTION("Shift key state")
    {
        fcn::KeyInput keyInput;
        REQUIRE(keyInput.isShiftPressed() == false);

        keyInput.setShiftPressed(true);
        REQUIRE(keyInput.isShiftPressed() == true);

        keyInput.setShiftPressed(false);
        REQUIRE(keyInput.isShiftPressed() == false);
    }

    SECTION("Control key state")
    {
        fcn::KeyInput keyInput;
        REQUIRE(keyInput.isControlPressed() == false);

        keyInput.setControlPressed(true);
        REQUIRE(keyInput.isControlPressed() == true);

        keyInput.setControlPressed(false);
        REQUIRE(keyInput.isControlPressed() == false);
    }

    SECTION("Alt key state")
    {
        fcn::KeyInput keyInput;
        REQUIRE(keyInput.isAltPressed() == false);

        keyInput.setAltPressed(true);
        REQUIRE(keyInput.isAltPressed() == true);

        keyInput.setAltPressed(false);
        REQUIRE(keyInput.isAltPressed() == false);
    }

    SECTION("Meta key state")
    {
        fcn::KeyInput keyInput;
        REQUIRE(keyInput.isMetaPressed() == false);

        keyInput.setMetaPressed(true);
        REQUIRE(keyInput.isMetaPressed() == true);

        keyInput.setMetaPressed(false);
        REQUIRE(keyInput.isMetaPressed() == false);
    }
}

TEST_CASE("KeyInput numeric pad state", "[unit][keyinput]")
{
    SECTION("default is false")
    {
        fcn::KeyInput const keyInput;
        REQUIRE(keyInput.isNumericPad() == false);
    }

    SECTION("setNumericPad sets state")
    {
        fcn::KeyInput keyInput;
        keyInput.setNumericPad(true);
        REQUIRE(keyInput.isNumericPad() == true);

        keyInput.setNumericPad(false);
        REQUIRE(keyInput.isNumericPad() == false);
    }
}

TEST_CASE("KeyInput complex scenarios", "[unit][keyinput]")
{
    SECTION("all modifier keys can be set simultaneously")
    {
        fcn::KeyInput keyInput;
        keyInput.setShiftPressed(true);
        keyInput.setControlPressed(true);
        keyInput.setAltPressed(true);
        keyInput.setMetaPressed(true);

        REQUIRE(keyInput.isShiftPressed() == true);
        REQUIRE(keyInput.isControlPressed() == true);
        REQUIRE(keyInput.isAltPressed() == true);
        REQUIRE(keyInput.isMetaPressed() == true);
    }

    SECTION("changing key updates all states")
    {
        fcn::KeyInput keyInput;
        keyInput.setShiftPressed(true);
        keyInput.setControlPressed(true);

        fcn::Key key(65);
        keyInput.setKey(key);
        keyInput.setType(fcn::KeyInput::Type::Released);

        REQUIRE(keyInput.getKey() == key);
        REQUIRE(keyInput.getType() == fcn::KeyInput::Type::Released);
        REQUIRE(keyInput.isShiftPressed() == true);
        REQUIRE(keyInput.isControlPressed() == true);
    }
}
