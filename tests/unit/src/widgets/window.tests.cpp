// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2026 Fifengine contributors

// Standard library includes
#include <string>

// Third-party library includes
#include <catch2/catch_test_macros.hpp>

// Project headers
#include "fifechan/graphics.hpp"
#include "fifechan/widgets/window.hpp"

using fcn::Graphics;
using fcn::Window;

TEST_CASE("Window default constructor", "[unit][window]")
{
    Window window;
    REQUIRE(window.getCaption().empty());
    REQUIRE(window.getAlignment() == Graphics::Alignment::Center);
    REQUIRE(window.getTitleBarHeight() == 16);
    REQUIRE(window.getInnerBorderSize() == 1);
    REQUIRE(window.isMovable());
    REQUIRE(window.isOpaque());
    REQUIRE(!window.isFocusable());
}

TEST_CASE("Window caption constructor", "[unit][window]")
{
    Window window("Test Window");
    REQUIRE(window.getCaption() == "Test Window");
    REQUIRE(window.getAlignment() == Graphics::Alignment::Center);
}

TEST_CASE("Window setCaption and getCaption", "[unit][window]")
{
    Window window;
    window.setCaption("New Caption");
    REQUIRE(window.getCaption() == "New Caption");

    window.setCaption("");
    REQUIRE(window.getCaption().empty());
}

TEST_CASE("Window setAlignment and getAlignment", "[unit][window]")
{
    Window window;
    window.setAlignment(Graphics::Alignment::Left);
    REQUIRE(window.getAlignment() == Graphics::Alignment::Left);

    window.setAlignment(Graphics::Alignment::Right);
    REQUIRE(window.getAlignment() == Graphics::Alignment::Right);
}

TEST_CASE("Window setTitleBarHeight and getTitleBarHeight", "[unit][window]")
{
    Window window;
    window.setTitleBarHeight(24);
    REQUIRE(window.getTitleBarHeight() == 24);

    window.setTitleBarHeight(0);
    REQUIRE(window.getTitleBarHeight() == 0);
}

TEST_CASE("Window setInnerBorderSize and getInnerBorderSize", "[unit][window]")
{
    Window window;
    window.setInnerBorderSize(5);
    REQUIRE(window.getInnerBorderSize() == 5);

    window.setInnerBorderSize(0);
    REQUIRE(window.getInnerBorderSize() == 0);
}

TEST_CASE("Window setMovable and isMovable", "[unit][window]")
{
    Window window;
    REQUIRE(window.isMovable());

    window.setMovable(false);
    REQUIRE(!window.isMovable());

    window.setMovable(true);
    REQUIRE(window.isMovable());
}

TEST_CASE("Window setOpaque and isOpaque", "[unit][window]")
{
    Window window;
    REQUIRE(window.isOpaque());

    window.setOpaque(false);
    REQUIRE(!window.isOpaque());

    window.setOpaque(true);
    REQUIRE(window.isOpaque());
}

TEST_CASE("Window adjustSize does not crash", "[unit][window]")
{
    Window window;
    window.adjustSize();
    REQUIRE(true);
}

TEST_CASE("Window getChildrenArea is not zero", "[unit][window]")
{
    Window window;
    window.setSize(200, 200);
    auto area = window.getChildrenArea();
    REQUIRE(area.width > 0);
    REQUIRE(area.height > 0);
}
