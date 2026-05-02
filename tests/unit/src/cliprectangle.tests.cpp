// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2026 Fifengine contributors

#include "fifechan/cliprectangle.hpp"

// Third-party library includes
#include <catch2/catch_test_macros.hpp>

using fcn::ClipRectangle;
using fcn::Rectangle;

TEST_CASE("ClipRectangle default constructor creates valid rectangle", "[cliprectangle]")
{
    ClipRectangle rect;
    REQUIRE(rect.x == 0);
    REQUIRE(rect.y == 0);
    REQUIRE(rect.width == 0);
    REQUIRE(rect.height == 0);
    REQUIRE(rect.xOffset == 0);
    REQUIRE(rect.yOffset == 0);
}

TEST_CASE("ClipRectangle parameterized constructor sets values correctly", "[cliprectangle]")
{
    ClipRectangle rect(10, 20, 100, 200, 5, 15);
    REQUIRE(rect.x == 10);
    REQUIRE(rect.y == 20);
    REQUIRE(rect.width == 100);
    REQUIRE(rect.height == 200);
    REQUIRE(rect.xOffset == 5);
    REQUIRE(rect.yOffset == 15);
}

TEST_CASE("ClipRectangle copy assignment from Rectangle works correctly", "[cliprectangle]")
{
    Rectangle baseRect(1, 2, 3, 4);
    ClipRectangle clipRect;

    clipRect = baseRect;

    REQUIRE(clipRect.x == 1);
    REQUIRE(clipRect.y == 2);
    REQUIRE(clipRect.width == 3);
    REQUIRE(clipRect.height == 4);
    // xOffset and yOffset should remain unchanged (not part of Rectangle)
    REQUIRE(clipRect.xOffset == 0);
    REQUIRE(clipRect.yOffset == 0);
}

TEST_CASE("ClipRectangle copy assignment returns reference to self", "[cliprectangle]")
{
    Rectangle baseRect(10, 20, 30, 40);
    ClipRectangle clipRect(1, 2, 3, 4, 5, 6);

    ClipRectangle& result = (clipRect = baseRect);

    REQUIRE(&result == &clipRect);
}

TEST_CASE("ClipRectangle can be used with different offset values", "[cliprectangle]")
{
    ClipRectangle rect;
    rect.xOffset = 100;
    rect.yOffset = 200;

    REQUIRE(rect.xOffset == 100);
    REQUIRE(rect.yOffset == 200);
}

TEST_CASE("ClipRectangle is subclass of Rectangle", "[cliprectangle]")
{
    ClipRectangle clipRect(5, 10, 15, 20, 25, 30);
    Rectangle* rectPtr = &clipRect;

    REQUIRE(rectPtr->x == 5);
    REQUIRE(rectPtr->y == 10);
    REQUIRE(rectPtr->width == 15);
    REQUIRE(rectPtr->height == 20);
}
