// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2013 - 2026 Fifengine contributors

// Standard library includes
#include <stdexcept>

// Third-party library includes
#include <catch2/catch_test_macros.hpp>

// Project headers (subdirs before local)
#include "fifechan/genericinput.hpp"
#include "fifechan/keyinput.hpp"
#include "fifechan/mouseinput.hpp"

using fcn::GenericInput;
using fcn::KeyInput;
using fcn::MouseInput;

// ============================================================================
// Key input queue operations
// ============================================================================

TEST_CASE("GenericInput key queue starts empty", "[unit][genericinput]")
{
    GenericInput input;
    REQUIRE(input.isKeyQueueEmpty() == true);
}

TEST_CASE("GenericInput pushKeyPressed adds to queue", "[unit][genericinput]")
{
    GenericInput input;
    input.pushKeyPressed(65); // 'A' key

    REQUIRE(input.isKeyQueueEmpty() == false);
}

TEST_CASE("GenericInput pushKeyReleased adds to queue", "[unit][genericinput]")
{
    GenericInput input;
    input.pushKeyReleased(65); // 'A' key

    REQUIRE(input.isKeyQueueEmpty() == false);
}

TEST_CASE("GenericInput dequeueKeyInput returns correct key", "[unit][genericinput]")
{
    GenericInput input;
    input.pushKeyPressed(65); // 'A' key

    KeyInput keyInput = input.dequeueKeyInput();
    REQUIRE(keyInput.getKey().getValue() == 65);
    REQUIRE(keyInput.getType() == KeyInput::Type::Pressed);
}

TEST_CASE("GenericInput dequeueKeyInput removes from queue", "[unit][genericinput]")
{
    GenericInput input;
    input.pushKeyPressed(65);
    input.pushKeyPressed(66);

    input.dequeueKeyInput();
    // After removing one, queue should still have one item
    REQUIRE(input.isKeyQueueEmpty() == false);

    input.dequeueKeyInput();
    // After removing all, queue should be empty
    REQUIRE(input.isKeyQueueEmpty() == true);
}

TEST_CASE("GenericInput dequeueKeyInput throws when empty", "[unit][genericinput]")
{
    GenericInput input;
    REQUIRE_THROWS_AS(input.dequeueKeyInput(), std::exception);
}

TEST_CASE("GenericInput handles key press and release sequence", "[unit][genericinput]")
{
    GenericInput input;
    input.pushKeyPressed(65);
    input.pushKeyReleased(65);

    KeyInput press = input.dequeueKeyInput();
    REQUIRE(press.getType() == KeyInput::Type::Pressed);
    REQUIRE(press.getKey().getValue() == 65);

    KeyInput release = input.dequeueKeyInput();
    REQUIRE(release.getType() == KeyInput::Type::Released);
    REQUIRE(release.getKey().getValue() == 65);
}

// ============================================================================
// Mouse input queue operations
// ============================================================================

TEST_CASE("GenericInput mouse queue starts empty", "[unit][genericinput]")
{
    GenericInput input;
    REQUIRE(input.isMouseQueueEmpty() == true);
}

TEST_CASE("GenericInput pushMouseButtonPressed adds to queue", "[unit][genericinput]")
{
    GenericInput input;
    input.pushMouseButtonPressed(100, 200, static_cast<int>(MouseInput::Button::Left));

    REQUIRE(input.isMouseQueueEmpty() == false);
}

TEST_CASE("GenericInput pushMouseButtonReleased adds to queue", "[unit][genericinput]")
{
    GenericInput input;
    input.pushMouseButtonReleased(100, 200, static_cast<int>(MouseInput::Button::Left));

    REQUIRE(input.isMouseQueueEmpty() == false);
}

TEST_CASE("GenericInput dequeueMouseInput returns correct data", "[unit][genericinput]")
{
    GenericInput input;
    input.pushMouseButtonPressed(100, 200, static_cast<int>(MouseInput::Button::Left));

    MouseInput mouseInput = input.dequeueMouseInput();
    REQUIRE(mouseInput.getX() == 100);
    REQUIRE(mouseInput.getY() == 200);
    REQUIRE(mouseInput.getButton() == MouseInput::Button::Left);
    REQUIRE(mouseInput.getType() == MouseInput::Type::Pressed);
}

TEST_CASE("GenericInput dequeueMouseInput removes from queue", "[unit][genericinput]")
{
    GenericInput input;
    input.pushMouseButtonPressed(100, 200, static_cast<int>(MouseInput::Button::Left));
    input.pushMouseButtonPressed(300, 400, static_cast<int>(MouseInput::Button::Right));

    input.dequeueMouseInput();
    REQUIRE(input.isMouseQueueEmpty() == false);

    input.dequeueMouseInput();
    REQUIRE(input.isMouseQueueEmpty() == true);
}

TEST_CASE("GenericInput dequeueMouseInput throws when empty", "[unit][genericinput]")
{
    GenericInput input;
    REQUIRE_THROWS_AS(input.dequeueMouseInput(), std::exception);
}

// ============================================================================
// Mouse wheel events
// ============================================================================

TEST_CASE("GenericInput pushMouseWheelMovedUp", "[unit][genericinput]")
{
    GenericInput input;
    input.pushMouseWheelMovedUp(100, 200);

    MouseInput mouseInput = input.dequeueMouseInput();
    REQUIRE(mouseInput.getX() == 100);
    REQUIRE(mouseInput.getY() == 200);
    REQUIRE(mouseInput.getType() == MouseInput::Type::WheelMovedUp);
}

TEST_CASE("GenericInput pushMouseWheelMovedDown", "[unit][genericinput]")
{
    GenericInput input;
    input.pushMouseWheelMovedDown(100, 200);

    MouseInput mouseInput = input.dequeueMouseInput();
    REQUIRE(mouseInput.getType() == MouseInput::Type::WheelMovedDown);
}

TEST_CASE("GenericInput pushMouseWheelMovedRight", "[unit][genericinput]")
{
    GenericInput input;
    input.pushMouseWheelMovedRight(100, 200);

    MouseInput mouseInput = input.dequeueMouseInput();
    REQUIRE(mouseInput.getType() == MouseInput::Type::WheelMovedRight);
}

TEST_CASE("GenericInput pushMouseWheelMovedLeft", "[unit][genericinput]")
{
    GenericInput input;
    input.pushMouseWheelMovedLeft(100, 200);

    MouseInput mouseInput = input.dequeueMouseInput();
    REQUIRE(mouseInput.getType() == MouseInput::Type::WheelMovedLeft);
}

// ============================================================================
// Mouse move events
// ============================================================================

TEST_CASE("GenericInput pushMouseMoved", "[unit][genericinput]")
{
    GenericInput input;
    input.pushMouseMoved(150, 250);

    MouseInput mouseInput = input.dequeueMouseInput();
    REQUIRE(mouseInput.getX() == 150);
    REQUIRE(mouseInput.getY() == 250);
    REQUIRE(mouseInput.getType() == MouseInput::Type::Moved);
}

// ============================================================================
// Combined operations
// ============================================================================

TEST_CASE("GenericInput handles mixed key and mouse events", "[unit][genericinput]")
{
    GenericInput input;
    input.pushKeyPressed(65);
    input.pushMouseButtonPressed(100, 200, static_cast<int>(MouseInput::Button::Left));

    // Key queue should have one item
    REQUIRE(input.isKeyQueueEmpty() == false);
    // Mouse queue should have one item
    REQUIRE(input.isMouseQueueEmpty() == false);

    // Dequeue both
    KeyInput key     = input.dequeueKeyInput();
    MouseInput mouse = input.dequeueMouseInput();

    REQUIRE(key.getKey().getValue() == 65);
    REQUIRE(mouse.getX() == 100);
    REQUIRE(mouse.getY() == 200);
}

TEST_CASE("GenericInput pollInput does nothing", "[unit][genericinput]")
{
    GenericInput input;
    // Should not throw or crash
    input._pollInput();
    REQUIRE(input.isKeyQueueEmpty() == true);
    REQUIRE(input.isMouseQueueEmpty() == true);
}
