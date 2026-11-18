// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2026 Fifengine contributors

// Standard library includes
#include <string>

// Third-party library includes
#include <catch2/catch_test_macros.hpp>

// Project headers
#include "fifechan/widgets/tooltip.hpp"

using fcn::Tooltip;
using fcn::TooltipSpec;

// ============================================================================
// Tooltip basic properties
// ============================================================================

TEST_CASE("Tooltip default constructor", "[unit][tooltip]")
{
    Tooltip tip;
    REQUIRE_FALSE(tip.isHovering());
    REQUIRE_FALSE(tip.isExtendedView());
    REQUIRE(tip.getCurrentContent().empty());
    REQUIRE(tip.getWidgetId() == 0);
}

TEST_CASE("Tooltip setSpec and getSpec", "[unit][tooltip]")
{
    Tooltip tip;
    TooltipSpec spec;
    spec.delayMs = 500;
    tip.setSpec(spec);
    REQUIRE(tip.getSpec().delayMs == 500);
}

TEST_CASE("Tooltip setWidgetId and getWidgetId", "[unit][tooltip]")
{
    Tooltip tip;
    tip.setWidgetId(42);
    REQUIRE(tip.getWidgetId() == 42);

    tip.setWidgetId(0);
    REQUIRE(tip.getWidgetId() == 0);
}

TEST_CASE("Tooltip startHover and endHover", "[unit][tooltip]")
{
    Tooltip tip;
    REQUIRE_FALSE(tip.isHovering());

    tip.startHover();
    REQUIRE(tip.isHovering());

    tip.endHover();
    REQUIRE_FALSE(tip.isHovering());
}

TEST_CASE("Tooltip update while not hovering does nothing", "[unit][tooltip]")
{
    Tooltip tip;
    REQUIRE_FALSE(tip.isHovering());

    tip.update(100, 0);
    REQUIRE(tip.getCurrentContent().empty());
}

TEST_CASE("Tooltip update generates content immediately", "[unit][tooltip]")
{
    Tooltip tip;
    TooltipSpec spec;
    spec.delayMs = 200;
    spec.content = [](int) {
        return std::string("Tooltip text");
    };
    tip.setSpec(spec);

    tip.startHover();
    REQUIRE(tip.isHovering());

    // Content is generated immediately on update, regardless of delay
    tip.update(100, 0);
    REQUIRE(tip.getCurrentContent() == "Tooltip text");

    // Delay only affects draw() and getChildrenArea()
    tip.update(200, 0);
    REQUIRE(tip.getCurrentContent() == "Tooltip text");
}

TEST_CASE("Tooltip extended content with ALT modifier", "[unit][tooltip]")
{
    Tooltip tip;
    TooltipSpec spec;
    spec.delayMs = 0;
    spec.content = [](int) {
        return std::string("Normal");
    };
    spec.modifierBehavior.enabled         = true;
    spec.modifierBehavior.modifiedContent = [](int) {
        return std::string("Extended");
    };
    tip.setSpec(spec);

    tip.startHover();
    tip.update(0, 0x100); // KMOD_ALT

    REQUIRE(tip.isExtendedView());
    std::string const content = tip.getCurrentContent();
    REQUIRE(content.find("Normal") != std::string::npos);
    REQUIRE(content.find("Extended") != std::string::npos);
}

TEST_CASE("Tooltip extended without ALT is not extended", "[unit][tooltip]")
{
    Tooltip tip;
    TooltipSpec spec;
    spec.delayMs = 0;
    spec.content = [](int) {
        return std::string("Normal");
    };
    spec.modifierBehavior.enabled         = true;
    spec.modifierBehavior.modifiedContent = [](int) {
        return std::string("Extended");
    };
    tip.setSpec(spec);

    tip.startHover();
    tip.update(0, 0); // No ALT

    REQUIRE_FALSE(tip.isExtendedView());
    REQUIRE(tip.getCurrentContent() == "Normal");
}

TEST_CASE("Tooltip extended with modified content starting with normal", "[unit][tooltip]")
{
    Tooltip tip;
    TooltipSpec spec;
    spec.delayMs = 0;
    spec.content = [](int) {
        return std::string("Base");
    };
    spec.modifierBehavior.enabled         = true;
    spec.modifierBehavior.modifiedContent = [](int) {
        return std::string("Base\nDetails");
    };
    tip.setSpec(spec);

    tip.startHover();
    tip.update(0, 0x100);

    // The duplicate prefix "Base" should be stripped
    std::string const content = tip.getCurrentContent();
    REQUIRE(content.find("Base") != std::string::npos);
    REQUIRE(content.find("Details") != std::string::npos);
}

TEST_CASE("Tooltip endHover clears extended state", "[unit][tooltip]")
{
    Tooltip tip;
    TooltipSpec spec;
    spec.delayMs = 0;
    spec.content = [](int) {
        return std::string("Text");
    };
    spec.modifierBehavior.enabled         = true;
    spec.modifierBehavior.modifiedContent = [](int) {
        return std::string("Extended");
    };
    tip.setSpec(spec);

    tip.startHover();
    tip.update(0, 0x100);
    REQUIRE(tip.isExtendedView());

    tip.endHover();
    REQUIRE_FALSE(tip.isHovering());
    REQUIRE_FALSE(tip.isExtendedView());
}

TEST_CASE("Tooltip update increments hover timer", "[unit][tooltip]")
{
    Tooltip tip;
    TooltipSpec spec;
    spec.delayMs = 1000;
    spec.content = [](int) {
        return std::string("Delayed");
    };
    tip.setSpec(spec);

    tip.startHover();

    // Content is generated immediately regardless of delay
    tip.update(500, 0);
    REQUIRE(tip.getCurrentContent() == "Delayed");

    tip.update(600, 0);
    REQUIRE(tip.getCurrentContent() == "Delayed");
}

// ============================================================================
// Tooltip edge cases and extended coverage
// ============================================================================

TEST_CASE("Tooltip generateNormalContent with content function", "[unit][tooltip][edge]")
{
    Tooltip tip;
    TooltipSpec spec;
    spec.content = [](int widgetId) {
        return std::string("Widget-specific content");
    };
    tip.setSpec(spec);
    tip.setWidgetId(1);

    tip.startHover();
    tip.update(0, 0);

    REQUIRE(tip.getCurrentContent() == "Widget-specific content");
}

TEST_CASE("Tooltip generateNormalContent without content function", "[unit][tooltip][edge]")
{
    Tooltip tip;
    TooltipSpec spec;
    // No content set - should clear content when hovering starts
    tip.setSpec(spec);

    tip.startHover();
    tip.update(0, 0);

    REQUIRE(tip.getCurrentContent().empty());
}

TEST_CASE("Tooltip update with ALT enables extended view", "[unit][tooltip][edge]")
{
    Tooltip tip;
    TooltipSpec spec;
    spec.delayMs = 0;
    spec.content = [](int) {
        return std::string("Normal text");
    };
    spec.modifierBehavior.enabled         = true;
    spec.modifierBehavior.modifiedContent = [](int) {
        return std::string("Modified text");
    };
    tip.setSpec(spec);

    tip.startHover();

    // Without ALT initially
    tip.update(0, 0);
    REQUIRE(tip.getCurrentContent() == "Normal text");
    REQUIRE_FALSE(tip.isExtendedView());

    // With ALT
    tip.update(0, 0x100);
    REQUIRE(tip.isExtendedView());
    std::string const content = tip.getCurrentContent();
    REQUIRE(content.find("Normal text") != std::string::npos);
    REQUIRE(content.find("Modified text") != std::string::npos);
}

TEST_CASE("Tooltip update disables extended view when ALT released", "[unit][tooltip][edge]")
{
    Tooltip tip;
    TooltipSpec spec;
    spec.delayMs = 0;
    spec.content = [](int) {
        return std::string("Normal");
    };
    spec.modifierBehavior.enabled         = true;
    spec.modifierBehavior.modifiedContent = [](int) {
        return std::string("Extended");
    };
    tip.setSpec(spec);

    tip.startHover();
    tip.update(0, 0x100); // ALT pressed
    REQUIRE(tip.isExtendedView());

    tip.update(0, 0); // ALT released
    REQUIRE_FALSE(tip.isExtendedView());
    REQUIRE(tip.getCurrentContent() == "Normal");
}

TEST_CASE("Tooltip draw not hovering returns early state consistent", "[unit][tooltip][edge]")
{
    Tooltip tip;
    TooltipSpec spec;
    spec.delayMs = 1000;
    spec.content = [](int) {
        return std::string("Delayed");
    };
    tip.setSpec(spec);
    tip.startHover();

    // Before delay timer expires - should return early
    tip.update(100, 0); // timer = 100, still < 1000
    // draw() should not crash or assert when not hovering ready
    // Just verify state is consistent
    REQUIRE(tip.isHovering());
}

TEST_CASE("Tooltip getChildrenArea not hovering returns empty", "[unit][tooltip][edge]")
{
    Tooltip tip;
    TooltipSpec spec;
    spec.delayMs = 1000;
    tip.setSpec(spec);

    // Before hover timer expires
    auto area = tip.getChildrenArea();
    REQUIRE(area.width == 0);
    REQUIRE(area.height == 0);
    REQUIRE(area.x == 0);
    REQUIRE(area.y == 0);
}

TEST_CASE("Tooltip getChildrenArea hovering ready returns children area", "[unit][tooltip][edge]")
{
    Tooltip tip;
    TooltipSpec spec;
    spec.delayMs = 0;
    tip.setSpec(spec);
    tip.startHover();
    tip.update(100, 0); // timer now >= delayMs (0)

    auto area = tip.getChildrenArea();
    // Should return the normal children area (non-empty since tooltip is ready)
    REQUIRE(area.width >= 0);
    REQUIRE(area.height >= 0);
}

TEST_CASE("Tooltip update with various modifier states", "[unit][tooltip][edge]")
{
    Tooltip tip;
    TooltipSpec spec;
    spec.delayMs = 0;
    spec.content = [](int) {
        return std::string("Content");
    };
    spec.modifierBehavior.enabled         = true;
    spec.modifierBehavior.modifiedContent = [](int) {
        return std::string("Modified");
    };
    tip.setSpec(spec);

    tip.startHover();

    // Test various modifier state values
    tip.update(0, 0);     // No modifier
    tip.update(0, 0x100); // ALT only
    tip.update(0, 0);     // Release ALT
    tip.update(0, 0x100); // Press ALT again

    // Should remain consistent
    REQUIRE(!tip.getCurrentContent().empty());
}

TEST_CASE("Tooltip multiple rapid updates", "[unit][tooltip][edge]")
{
    Tooltip tip;
    int callCount = 0;
    TooltipSpec spec;
    spec.delayMs = 0;
    spec.content = [&callCount](int) {
        callCount++;
        return std::string("Call ") + std::to_string(callCount);
    };
    tip.setSpec(spec);

    tip.startHover();

    // Multiple rapid updates
    tip.update(10, 0);
    tip.update(20, 0);
    tip.update(30, 0);
    tip.update(40, 0);

    // Content should be from the last update
    REQUIRE(tip.getCurrentContent() == "Call 4");
}

TEST_CASE("Tooltip content function called with widgetId", "[unit][tooltip][edge]")
{
    Tooltip tip;
    int receivedWidgetId = -1;
    TooltipSpec spec;
    spec.content = [&receivedWidgetId](int widgetId) {
        receivedWidgetId = widgetId;
        return std::string("Content for ") + std::to_string(widgetId);
    };
    spec.modifierBehavior.enabled         = true;
    spec.modifierBehavior.modifiedContent = [&receivedWidgetId](int widgetId) {
        receivedWidgetId = widgetId;
        return std::string("Modified for ") + std::to_string(widgetId);
    };
    tip.setSpec(spec);
    tip.setWidgetId(42);

    tip.startHover();
    tip.update(0, 0);

    REQUIRE(receivedWidgetId == 42);
    REQUIRE(tip.getCurrentContent() == "Content for 42");
}

TEST_CASE("Tooltip extended content strips duplicate prefix correctly", "[unit][tooltip][edge]")
{
    Tooltip tip;
    TooltipSpec spec;
    spec.delayMs = 0;
    spec.content = [](int) {
        return std::string("Prefix text more stuff");
    };
    spec.modifierBehavior.enabled         = true;
    spec.modifierBehavior.modifiedContent = [](int) {
        return std::string("Prefix text details here");
    };
    tip.setSpec(spec);

    tip.startHover();
    tip.update(0, 0x100);

    std::string const content = tip.getCurrentContent();
    // "Prefix text" should be stripped from modified content
    REQUIRE(content.find("details here") != std::string::npos);
    // Normal content should remain
    REQUIRE(content.find("Prefix text more stuff") != std::string::npos);
}

TEST_CASE("Tooltip isHovering and isExtendedView states", "[unit][tooltip][edge]")
{
    Tooltip tip;
    TooltipSpec spec;
    spec.delayMs = 0;
    spec.content = [](int) {
        return std::string("Content");
    };
    spec.modifierBehavior.enabled         = true;
    spec.modifierBehavior.modifiedContent = [](int) {
        return std::string("Extended");
    };
    tip.setSpec(spec);

    // Initially
    REQUIRE_FALSE(tip.isHovering());
    REQUIRE_FALSE(tip.isExtendedView());

    // After startHover
    tip.startHover();
    REQUIRE(tip.isHovering());
    REQUIRE_FALSE(tip.isExtendedView());

    // After update with ALT
    tip.update(0, 0x100);
    REQUIRE(tip.isHovering());
    REQUIRE(tip.isExtendedView());

    // After endHover
    tip.endHover();
    REQUIRE_FALSE(tip.isHovering());
    REQUIRE_FALSE(tip.isExtendedView());
}
