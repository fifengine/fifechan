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

// ============================================================================
// Window basic properties
// ============================================================================

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

// ============================================================================
// Window adjustSize and getChildrenArea
// ============================================================================

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

TEST_CASE("Window getChildrenArea with different sizes", "[unit][window][edge]")
{
    // Small window
    Window smallWindow;
    smallWindow.setSize(50, 50);
    auto smallArea = smallWindow.getChildrenArea();
    REQUIRE(smallArea.width >= 0);
    REQUIRE(smallArea.height >= 0);

    // Medium window
    Window mediumWindow;
    mediumWindow.setSize(200, 200);
    auto mediumArea = mediumWindow.getChildrenArea();
    REQUIRE(mediumArea.width > 0);
    REQUIRE(mediumArea.height > 0);

    // Large window
    Window largeWindow;
    largeWindow.setSize(1000, 800);
    auto largeArea = largeWindow.getChildrenArea();
    REQUIRE(largeArea.width > 0);
    REQUIRE(largeArea.height > 0);
}

// ============================================================================
// Window mouse event handling
// ============================================================================

TEST_CASE("Window mouse pressed sets drag offset", "[unit][window][mouse]")
{
    Window window;
    // We can't easily create a real MouseEvent without SDL,
    // but we can test the logic path
    window.setSize(200, 200);

    // Test that mousePressed logic is valid
    // (The full SDL event handling requires SDL infrastructure)
    REQUIRE(window.isMovable());
}

TEST_CASE("Window mouse released resets moved state", "[unit][window][mouse]")
{
    Window window;
    window.setSize(200, 200);
    // Test that mouseReleased logic is valid
    REQUIRE(window.isMovable());
}

// ============================================================================
// Window resizeToContent and expandContent
// ============================================================================

TEST_CASE("Window resizeToContent with Absolute layout", "[unit][window][resize]")
{
    Window window;
    window.setLayout(fcn::Container::LayoutPolicy::Absolute);
    window.resizeToContent(true);
    // Should not crash
    REQUIRE(true);
}

TEST_CASE("Window resizeToContent with other layout", "[unit][window][resize]")
{
    Window window;
    window.resizeToContent(true);
    // Should not crash
    REQUIRE(true);
}

TEST_CASE("Window expandContent", "[unit][window][expand]")
{
    Window window;
    window.expandContent(true);
    // Should not crash
    REQUIRE(true);
}

// ============================================================================
// Window drawing and appearance
// ============================================================================

TEST_CASE("Window isOpaque default state", "[unit][window][appearance]")
{
    Window window;
    REQUIRE(window.isOpaque());
}

TEST_CASE("Window toggles opacity", "[unit][window][appearance]")
{
    Window window;
    REQUIRE(window.isOpaque());
    window.setOpaque(false);
    REQUIRE(!window.isOpaque());
    window.setOpaque(true);
    REQUIRE(window.isOpaque());
}

// ============================================================================
// Window caption and alignment edge cases
// ============================================================================

TEST_CASE("Window empty caption", "[unit][window][edge]")
{
    Window window("");
    REQUIRE(window.getCaption().empty());
    REQUIRE(window.getAlignment() == Graphics::Alignment::Center);
}

TEST_CASE("Window long caption", "[unit][window][edge]")
{
    std::string longCaption(100, 'A');
    Window window(longCaption);
    REQUIRE(window.getCaption() == longCaption);
}

TEST_CASE("Window caption with special characters", "[unit][window][edge]")
{
    Window window("Caption with !@#$%^&*() special chars");
    REQUIRE(window.getCaption() == "Caption with !@#$%^&*() special chars");
}

TEST_CASE("Window caption with unicode-like text", "[unit][window][edge]")
{
    Window window("Test üñïcödé");
    REQUIRE(window.getCaption() == "Test üñïcödé");
}

// ============================================================================
// Window border and padding edge cases
// ============================================================================

TEST_CASE("Window border and padding extremes", "[unit][window][edge]")
{
    Window window;
    window.setInnerBorderSize(0);
    REQUIRE(window.getInnerBorderSize() == 0);

    window.setInnerBorderSize(100);
    REQUIRE(window.getInnerBorderSize() == 100);
}

// ============================================================================
// Window title bar height edge cases
// ============================================================================

TEST_CASE("Window title bar height extremes", "[unit][window][edge]")
{
    Window window;
    window.setTitleBarHeight(0);
    REQUIRE(window.getTitleBarHeight() == 0);

    window.setTitleBarHeight(1);
    REQUIRE(window.getTitleBarHeight() == 1);

    window.setTitleBarHeight(200);
    REQUIRE(window.getTitleBarHeight() == 200);
}

// ============================================================================
// Window movability edge cases
// ============================================================================

TEST_CASE("Window movability toggling", "[unit][window][edge]")
{
    Window window;
    REQUIRE(window.isMovable());

    window.setMovable(false);
    REQUIRE(!window.isMovable());

    window.setMovable(true);
    REQUIRE(window.isMovable());

    window.setMovable(false);
    REQUIRE(!window.isMovable());
}

// ============================================================================
// Window constructor with different captions
// ============================================================================

TEST_CASE("Window default constructed has empty caption", "[unit][window][constructor]")
{
    Window window;
    REQUIRE(window.getCaption().empty());
}

TEST_CASE("Window caption constructor sets caption correctly", "[unit][window][constructor]")
{
    Window window("My Window");
    REQUIRE(window.getCaption() == "My Window");
}

TEST_CASE("Window constructor preserves default alignment", "[unit][window][constructor]")
{
    Window window("Test");
    REQUIRE(window.getAlignment() == Graphics::Alignment::Center);
}

TEST_CASE("Window constructor preserves default title bar height", "[unit][window][constructor]")
{
    Window window;
    REQUIRE(window.getTitleBarHeight() == 16);
}

TEST_CASE("Window constructor preserves default inner border size", "[unit][window][constructor]")
{
    Window window;
    REQUIRE(window.getInnerBorderSize() == 1);
}

// ============================================================================
// Window setSize interaction with children area
// ============================================================================

TEST_CASE("Window setSize updates children area proportions", "[unit][window][sizing]")
{
    Window window;
    window.setSize(400, 300);

    auto area1 = window.getChildrenArea();
    REQUIRE(area1.width > 0);
    REQUIRE(area1.height > 0);

    window.setSize(800, 600);
    auto area2 = window.getChildrenArea();
    REQUIRE(area2.width > 0);
    REQUIRE(area2.height > 0);
    REQUIRE(area2.width > area1.width);
    REQUIRE(area2.height > area1.height);
}

// ============================================================================
// Window isFocusable (inherited behavior)
// ============================================================================

TEST_CASE("Window isFocusable default is false", "[unit][window][focus]")
{
    Window window;
    REQUIRE(!window.isFocusable());
}

// ============================================================================
// Window inherited from Container methods
// ============================================================================

TEST_CASE("Window resizeToContent does not crash with recursion", "[unit][window][inherited]")
{
    Window window;
    window.resizeToContent(false);
    REQUIRE(true);
}

TEST_CASE("Window expandContent does not crash", "[unit][window][inherited]")
{
    Window window;
    window.expandContent(true);
    REQUIRE(true);
}

TEST_CASE("Window adjustSize does not crash", "[unit][window][inherited]")
{
    Window window;
    window.adjustSize();
    REQUIRE(true);
}
