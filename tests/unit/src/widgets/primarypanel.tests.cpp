// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2026 Fifengine contributors

// Third-party library includes
#include <string>

#include <catch2/catch_test_macros.hpp>

// Project headers
#include "fifechan/widgets/primarypanel.hpp"

using fcn::PrimaryPanel;

// ============================================================================
// PrimaryPanel constructor
// ============================================================================

TEST_CASE("PrimaryPanel constructor sets title and default width", "[unit][primarypanel]")
{
    PrimaryPanel panel("Test Title");

    REQUIRE(panel.getTitle() == "Test Title");
    REQUIRE(panel.getDefaultWidth() == 250); // Default width
}

TEST_CASE("PrimaryPanel constructor with empty title", "[unit][primarypanel]")
{
    PrimaryPanel panel("");

    REQUIRE(panel.getTitle() == "");
    REQUIRE(panel.getDefaultWidth() == 250);
}

// ============================================================================
// PrimaryPanel default width getter/setter
// ============================================================================

TEST_CASE("PrimaryPanel setDefaultWidth updates width", "[unit][primarypanel]")
{
    PrimaryPanel panel("Test");

    panel.setDefaultWidth(300);
    REQUIRE(panel.getDefaultWidth() == 300);
}

TEST_CASE("PrimaryPanel getDefaultWidth returns set value", "[unit][primarypanel]")
{
    PrimaryPanel panel("Test");

    panel.setDefaultWidth(500);
    REQUIRE(panel.getDefaultWidth() == 500);

    panel.setDefaultWidth(100);
    REQUIRE(panel.getDefaultWidth() == 100);
}
