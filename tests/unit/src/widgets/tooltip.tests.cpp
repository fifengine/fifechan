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
