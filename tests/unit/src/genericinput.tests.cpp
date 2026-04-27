// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2013 - 2026 Fifengine contributors

// Standard library includes
#include <stdexcept>

// Third-party library includes
#include <catch2/catch_test_macros.hpp>

// Project headers (subdirs before local)
#include <fifechan/exception.hpp>
#include <fifechan/genericinput.hpp>
#include <fifechan/key.hpp>

TEST_CASE("GenericInput default constructor", "[unit][genericinput]")
{
    SECTION("initializes with empty queues")
    {
        fcn::GenericInput input;
        REQUIRE(input.isKeyQueueEmpty() == true);
        REQUIRE(input.isMouseQueueEmpty() == true);
    }
}

TEST_CASE("GenericInput key press operations", "[unit][genericinput]")
{
    SECTION("pushKeyPressed adds to queue")
    {
        fcn::GenericInput input;
        input.pushKeyPressed(65); // 'A'

        REQUIRE(input.isKeyQueueEmpty() == false);

        fcn::KeyInput keyInput = input.dequeueKeyInput();
        REQUIRE(keyInput.getKey().getValue() == 65);
        REQUIRE(keyInput.getType() == fcn::KeyInput::Type::Pressed);
    }

    SECTION("pushKeyReleased adds to queue")
    {
        fcn::GenericInput input;
        input.pushKeyReleased(65);

        REQUIRE(input.isKeyQueueEmpty() == false);

        fcn::KeyInput keyInput = input.dequeueKeyInput();
        REQUIRE(keyInput.getKey().getValue() == 65);
        REQUIRE(keyInput.getType() == fcn::KeyInput::Type::Released);
    }

    SECTION("multiple key presses are queued")
    {
        fcn::GenericInput input;
        input.pushKeyPressed(65);
        input.pushKeyPressed(66);
        input.pushKeyPressed(67);

        REQUIRE(input.isKeyQueueEmpty() == false);

        fcn::KeyInput key1 = input.dequeueKeyInput();
        fcn::KeyInput key2 = input.dequeueKeyInput();
        fcn::KeyInput key3 = input.dequeueKeyInput();

        REQUIRE(key1.getKey().getValue() == 65);
        REQUIRE(key2.getKey().getValue() == 66);
        REQUIRE(key3.getKey().getValue() == 67);
        REQUIRE(input.isKeyQueueEmpty() == true);
    }

    SECTION("key press and release ordering")
    {
        fcn::GenericInput input;
        input.pushKeyPressed(65);
        input.pushKeyReleased(65);

        fcn::KeyInput key1 = input.dequeueKeyInput();
        fcn::KeyInput key2 = input.dequeueKeyInput();

        REQUIRE(key1.getType() == fcn::KeyInput::Type::Pressed);
        REQUIRE(key2.getType() == fcn::KeyInput::Type::Released);
    }

    SECTION("special key values work")
    {
        fcn::GenericInput input;
        input.pushKeyPressed(fcn::Key::Escape);
        input.pushKeyPressed(fcn::Key::Tab);
        input.pushKeyPressed(fcn::Key::F1);

        fcn::KeyInput key1 = input.dequeueKeyInput();
        fcn::KeyInput key2 = input.dequeueKeyInput();
        fcn::KeyInput key3 = input.dequeueKeyInput();

        REQUIRE(key1.getKey().getValue() == fcn::Key::Escape);
        REQUIRE(key2.getKey().getValue() == fcn::Key::Tab);
        REQUIRE(key3.getKey().getValue() == fcn::Key::F1);
    }

    SECTION("negative key values work")
    {
        fcn::GenericInput input;
        input.pushKeyPressed(-1000); // LeftAlt

        fcn::KeyInput keyInput = input.dequeueKeyInput();
        REQUIRE(keyInput.getKey().getValue() == -1000);
    }

    SECTION("zero key value works")
    {
        fcn::GenericInput input;
        input.pushKeyPressed(0);

        fcn::KeyInput keyInput = input.dequeueKeyInput();
        REQUIRE(keyInput.getKey().getValue() == 0);
    }
}

TEST_CASE("GenericInput mouse button operations", "[unit][genericinput]")
{
    SECTION("pushMouseButtonPressed adds to queue")
    {
        fcn::GenericInput input;
        input.pushMouseButtonPressed(100, 200, 1);

        REQUIRE(input.isMouseQueueEmpty() == false);

        fcn::MouseInput mouseInput = input.dequeueMouseInput();
        REQUIRE(mouseInput.getX() == 100);
        REQUIRE(mouseInput.getY() == 200);
        REQUIRE(mouseInput.getButton() == fcn::MouseInput::Button::Left);
        REQUIRE(mouseInput.getType() == fcn::MouseInput::Type::Pressed);
    }

    SECTION("pushMouseButtonReleased adds to queue")
    {
        fcn::GenericInput input;
        input.pushMouseButtonReleased(100, 200, 1);

        REQUIRE(input.isMouseQueueEmpty() == false);

        fcn::MouseInput mouseInput = input.dequeueMouseInput();
        REQUIRE(mouseInput.getType() == fcn::MouseInput::Type::Released);
    }

    SECTION("all button types")
    {
        fcn::GenericInput input;

        input.pushMouseButtonPressed(0, 0, 0);
        REQUIRE(input.dequeueMouseInput().getButton() == fcn::MouseInput::Button::Empty);

        input.pushMouseButtonPressed(0, 0, 1);
        REQUIRE(input.dequeueMouseInput().getButton() == fcn::MouseInput::Button::Left);

        input.pushMouseButtonPressed(0, 0, 2);
        REQUIRE(input.dequeueMouseInput().getButton() == fcn::MouseInput::Button::Right);

        input.pushMouseButtonPressed(0, 0, 3);
        REQUIRE(input.dequeueMouseInput().getButton() == fcn::MouseInput::Button::Middle);

        input.pushMouseButtonPressed(0, 0, 4);
        REQUIRE(input.dequeueMouseInput().getButton() == fcn::MouseInput::Button::X1);

        input.pushMouseButtonPressed(0, 0, 5);
        REQUIRE(input.dequeueMouseInput().getButton() == fcn::MouseInput::Button::X2);
    }

    SECTION("negative coordinates work")
    {
        fcn::GenericInput input;
        input.pushMouseButtonPressed(-100, -200, 1);

        fcn::MouseInput mouseInput = input.dequeueMouseInput();
        REQUIRE(mouseInput.getX() == -100);
        REQUIRE(mouseInput.getY() == -200);
    }

    SECTION("zero coordinates work")
    {
        fcn::GenericInput input;
        input.pushMouseButtonPressed(0, 0, 1);

        fcn::MouseInput mouseInput = input.dequeueMouseInput();
        REQUIRE(mouseInput.getX() == 0);
        REQUIRE(mouseInput.getY() == 0);
    }

    SECTION("large coordinates work")
    {
        fcn::GenericInput input;
        input.pushMouseButtonPressed(10000, 20000, 1);

        fcn::MouseInput mouseInput = input.dequeueMouseInput();
        REQUIRE(mouseInput.getX() == 10000);
        REQUIRE(mouseInput.getY() == 20000);
    }
}

TEST_CASE("GenericInput mouse wheel operations", "[unit][genericinput]")
{
    SECTION("pushMouseWheelMovedUp")
    {
        fcn::GenericInput input;
        input.pushMouseWheelMovedUp(100, 200);

        fcn::MouseInput mouseInput = input.dequeueMouseInput();
        REQUIRE(mouseInput.getType() == fcn::MouseInput::Type::WheelMovedUp);
        REQUIRE(mouseInput.getX() == 100);
        REQUIRE(mouseInput.getY() == 200);
    }

    SECTION("pushMouseWheelMovedDown")
    {
        fcn::GenericInput input;
        input.pushMouseWheelMovedDown(100, 200);

        fcn::MouseInput mouseInput = input.dequeueMouseInput();
        REQUIRE(mouseInput.getType() == fcn::MouseInput::Type::WheelMovedDown);
    }

    SECTION("pushMouseWheelMovedRight")
    {
        fcn::GenericInput input;
        input.pushMouseWheelMovedRight(100, 200);

        fcn::MouseInput mouseInput = input.dequeueMouseInput();
        REQUIRE(mouseInput.getType() == fcn::MouseInput::Type::WheelMovedRight);
    }

    SECTION("pushMouseWheelMovedLeft")
    {
        fcn::GenericInput input;
        input.pushMouseWheelMovedLeft(100, 200);

        fcn::MouseInput mouseInput = input.dequeueMouseInput();
        REQUIRE(mouseInput.getType() == fcn::MouseInput::Type::WheelMovedLeft);
    }

    SECTION("wheel events default to Empty button")
    {
        fcn::GenericInput input;
        input.pushMouseWheelMovedUp(0, 0);

        fcn::MouseInput mouseInput = input.dequeueMouseInput();
        REQUIRE(mouseInput.getButton() == fcn::MouseInput::Button::Empty);
    }
}

TEST_CASE("GenericInput mouse moved operations", "[unit][genericinput]")
{
    SECTION("pushMouseMoved adds to queue")
    {
        fcn::GenericInput input;
        input.pushMouseMoved(150, 250);

        REQUIRE(input.isMouseQueueEmpty() == false);

        fcn::MouseInput mouseInput = input.dequeueMouseInput();
        REQUIRE(mouseInput.getX() == 150);
        REQUIRE(mouseInput.getY() == 250);
        REQUIRE(mouseInput.getType() == fcn::MouseInput::Type::Moved);
        REQUIRE(mouseInput.getButton() == fcn::MouseInput::Button::Empty);
    }

    SECTION("multiple mouse moves are queued")
    {
        fcn::GenericInput input;
        input.pushMouseMoved(10, 20);
        input.pushMouseMoved(30, 40);
        input.pushMouseMoved(50, 60);

        fcn::MouseInput mouse1 = input.dequeueMouseInput();
        fcn::MouseInput mouse2 = input.dequeueMouseInput();
        fcn::MouseInput mouse3 = input.dequeueMouseInput();

        REQUIRE(mouse1.getX() == 10);
        REQUIRE(mouse2.getX() == 30);
        REQUIRE(mouse3.getX() == 50);
        REQUIRE(input.isMouseQueueEmpty() == true);
    }
}

TEST_CASE("GenericInput queue empty checks", "[unit][genericinput]")
{
    SECTION("isKeyQueueEmpty returns true initially")
    {
        fcn::GenericInput input;
        REQUIRE(input.isKeyQueueEmpty() == true);
    }

    SECTION("isMouseQueueEmpty returns true initially")
    {
        fcn::GenericInput input;
        REQUIRE(input.isMouseQueueEmpty() == true);
    }

    SECTION("empty after dequeuing all key inputs")
    {
        fcn::GenericInput input;
        input.pushKeyPressed(65);
        input.dequeueKeyInput();
        REQUIRE(input.isKeyQueueEmpty() == true);
    }

    SECTION("empty after dequeuing all mouse inputs")
    {
        fcn::GenericInput input;
        input.pushMouseButtonPressed(0, 0, 1);
        input.dequeueMouseInput();
        REQUIRE(input.isMouseQueueEmpty() == true);
    }
}

TEST_CASE("GenericInput mixed queue operations", "[unit][genericinput]")
{
    SECTION("key and mouse queues are independent")
    {
        fcn::GenericInput input;
        input.pushKeyPressed(65);
        input.pushMouseButtonPressed(0, 0, 1);

        REQUIRE(input.isKeyQueueEmpty() == false);
        REQUIRE(input.isMouseQueueEmpty() == false);

        input.dequeueKeyInput();
        REQUIRE(input.isKeyQueueEmpty() == true);
        REQUIRE(input.isMouseQueueEmpty() == false);
    }

    SECTION("interleaved key and mouse events")
    {
        fcn::GenericInput input;
        input.pushKeyPressed(65);
        input.pushMouseButtonPressed(10, 20, 1);
        input.pushKeyPressed(66);
        input.pushMouseMoved(30, 40);

        fcn::KeyInput key1     = input.dequeueKeyInput();
        fcn::MouseInput mouse1 = input.dequeueMouseInput();
        fcn::KeyInput key2     = input.dequeueKeyInput();
        fcn::MouseInput mouse2 = input.dequeueMouseInput();

        REQUIRE(key1.getKey().getValue() == 65);
        REQUIRE(mouse1.getX() == 10);
        REQUIRE(key2.getKey().getValue() == 66);
        REQUIRE(mouse2.getX() == 30);
    }
}

TEST_CASE("GenericInput throws on empty queue", "[unit][genericinput]")
{
    SECTION("dequeueKeyInput throws when empty")
    {
        fcn::GenericInput input;
        REQUIRE_THROWS_AS(input.dequeueKeyInput(), fcn::Exception);
    }

    SECTION("dequeueMouseInput throws when empty")
    {
        fcn::GenericInput input;
        REQUIRE_THROWS_AS(input.dequeueMouseInput(), fcn::Exception);
    }
}