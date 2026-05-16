// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2026 Fifengine contributors

// Standard library includes
#include <string>

// Third-party library includes
#include <catch2/catch_test_macros.hpp>

// Project headers
#include "fifechan/widgets/panel.hpp"

using fcn::Panel;
using fcn::VisibilityState;

TEST_CASE("Panel default constructor", "[unit][panel]")
{
    Panel panel;
    REQUIRE(panel.getTitle().empty());
    REQUIRE(panel.isClosable());
    REQUIRE(panel.getVisibilityState() == VisibilityState::Visible);
    REQUIRE(panel.getCollapsedWidth() == 10);
    REQUIRE(panel.isVisible());
    REQUIRE(panel.getChildrenCount() == 0);
}

TEST_CASE("Panel setTitle and getTitle", "[unit][panel]")
{
    Panel panel;
    panel.setTitle("My Panel");
    REQUIRE(panel.getTitle() == "My Panel");

    panel.setTitle("");
    REQUIRE(panel.getTitle().empty());

    panel.setTitle("Another Title");
    REQUIRE(panel.getTitle() == "Another Title");
}

TEST_CASE("Panel setClosable and isClosable", "[unit][panel]")
{
    Panel panel;
    REQUIRE(panel.isClosable());

    panel.setClosable(false);
    REQUIRE(!panel.isClosable());

    panel.setClosable(true);
    REQUIRE(panel.isClosable());
}

TEST_CASE("Panel setVisibilityState and getVisibilityState", "[unit][panel]")
{
    Panel panel;
    REQUIRE(panel.getVisibilityState() == VisibilityState::Visible);

    panel.setVisibilityState(VisibilityState::Hidden);
    REQUIRE(panel.getVisibilityState() == VisibilityState::Hidden);

    panel.setVisibilityState(VisibilityState::Collapsed);
    REQUIRE(panel.getVisibilityState() == VisibilityState::Collapsed);

    panel.setVisibilityState(VisibilityState::Visible);
    REQUIRE(panel.getVisibilityState() == VisibilityState::Visible);
}

TEST_CASE("Panel setCollapsedWidth and getCollapsedWidth", "[unit][panel]")
{
    Panel panel;
    REQUIRE(panel.getCollapsedWidth() == 10);

    panel.setCollapsedWidth(50);
    REQUIRE(panel.getCollapsedWidth() == 50);

    panel.setCollapsedWidth(0);
    REQUIRE(panel.getCollapsedWidth() == 0);
}

TEST_CASE("Panel setVisible maps to VisibilityState", "[unit][panel]")
{
    Panel panel;
    REQUIRE(panel.isVisible());

    panel.setVisible(false);
    REQUIRE(!panel.isVisible());
    REQUIRE(panel.getVisibilityState() == VisibilityState::Hidden);

    panel.setVisible(true);
    REQUIRE(panel.isVisible());
    REQUIRE(panel.getVisibilityState() == VisibilityState::Visible);
}

TEST_CASE("Panel adjustSize does not crash", "[unit][panel]")
{
    Panel panel;
    panel.adjustSize();
    REQUIRE(true);
}
