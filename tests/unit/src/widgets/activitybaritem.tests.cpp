// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2026 Fifengine contributors

// Corresponding header include
#include "fifechan/widgets/activitybaritem.hpp"

// Standard library includes
#include <string>

// Third-party library includes
#include <catch2/catch_test_macros.hpp>

using fcn::ActivityBarItem;

TEST_CASE("ActivityBarItem default construction", "[unit][activitybaritem]")
{
    ActivityBarItem item("");
    REQUIRE(item.getSize() == 40);
    REQUIRE(item.getTooltip().empty());
    REQUIRE(item.getPanel() == nullptr);
    REQUIRE_FALSE(item.isSelected());
}

TEST_CASE("ActivityBarItem construction with icon and tooltip", "[unit][activitybaritem]")
{
    ActivityBarItem item("\xF0\x9F\x94\xA7", "Test tooltip");
    REQUIRE(item.getSize() == 40);
    REQUIRE(item.getTooltip() == "Test tooltip");
    REQUIRE(item.getPanel() == nullptr);
}

TEST_CASE("ActivityBarItem setPanel and getPanel", "[unit][activitybaritem]")
{
    ActivityBarItem item("");
    REQUIRE(item.getPanel() == nullptr);
    item.setPanel(nullptr);
    REQUIRE(item.getPanel() == nullptr);
}

TEST_CASE("ActivityBarItem setSize and getSize", "[unit][activitybaritem]")
{
    ActivityBarItem item("");
    item.setSize(32);
    REQUIRE(item.getSize() == 32);
    REQUIRE(item.getWidth() == 32);
    REQUIRE(item.getHeight() == 32);
    item.setSize(48);
    REQUIRE(item.getSize() == 48);
}

TEST_CASE("ActivityBarItem setTooltip and getTooltip", "[unit][activitybaritem]")
{
    ActivityBarItem item("");
    REQUIRE(item.getTooltip().empty());
    item.setTooltip("New tooltip");
    REQUIRE(item.getTooltip() == "New tooltip");
    item.setTooltip("");
    REQUIRE(item.getTooltip().empty());
}

TEST_CASE("ActivityBarItem setSelected", "[unit][activitybaritem]")
{
    ActivityBarItem item("");
    REQUIRE_FALSE(item.isSelected());
    item.setSelected(true);
    REQUIRE(item.isSelected());
    item.setSelected(false);
    REQUIRE_FALSE(item.isSelected());
}

TEST_CASE("ActivityBarItem toggleSelected", "[unit][activitybaritem]")
{
    ActivityBarItem item("");
    REQUIRE_FALSE(item.isSelected());
    item.toggleSelected();
    REQUIRE(item.isSelected());
    item.toggleSelected();
    REQUIRE_FALSE(item.isSelected());
}

TEST_CASE("ActivityBarItem setPanelVisible with null panel", "[unit][activitybaritem]")
{
    ActivityBarItem item("");
    REQUIRE_FALSE(item.isSelected());
    item.setPanelVisible(true);
    REQUIRE(item.isSelected());
    item.setPanelVisible(false);
    REQUIRE_FALSE(item.isSelected());
}

TEST_CASE("ActivityBarItem getTooltip returns const reference to internal data", "[unit][activitybaritem]")
{
    ActivityBarItem item("", "persistent");
    std::string const & ref1 = item.getTooltip();
    std::string const & ref2 = item.getTooltip();
    // cppcheck-suppress knownConditionTrueFalse
    REQUIRE(&ref1 == &ref2);
}
