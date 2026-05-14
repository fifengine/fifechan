// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2026 Fifengine contributors

// Third-party library includes
#include <string>

#include <catch2/catch_test_macros.hpp>

// Project headers
#include "fifechan/widgets/secondarypanel.hpp"

using fcn::SecondaryPanel;

// ============================================================================
// SecondaryPanel constructor
// ============================================================================

TEST_CASE("SecondaryPanel constructor sets title and default width", "[unit][secondarypanel]")
{
    SecondaryPanel panel("Test Title");

    REQUIRE(panel.getTitle() == "Test Title");
    REQUIRE(panel.getDefaultWidth() == 250); // Default width
}

TEST_CASE("SecondaryPanel constructor with empty title", "[unit][secondarypanel]")
{
    SecondaryPanel panel("");

    REQUIRE(panel.getTitle().empty());
    REQUIRE(panel.getDefaultWidth() == 250);
}

// ============================================================================
// SecondaryPanel default width getter/setter
// ============================================================================

TEST_CASE("SecondaryPanel setDefaultWidth updates width", "[unit][secondarypanel]")
{
    SecondaryPanel panel("Test");

    panel.setDefaultWidth(300);
    REQUIRE(panel.getDefaultWidth() == 300);
}

TEST_CASE("SecondaryPanel setDefaultWidth to zero", "[unit][secondarypanel]")
{
    SecondaryPanel panel("Test");

    panel.setDefaultWidth(0);
    REQUIRE(panel.getDefaultWidth() == 0);
}

TEST_CASE("SecondaryPanel setDefaultWidth to negative", "[unit][secondarypanel]")
{
    SecondaryPanel panel("Test");

    panel.setDefaultWidth(-100);
    REQUIRE(panel.getDefaultWidth() == -100);
}

TEST_CASE("SecondaryPanel getDefaultWidth returns set value", "[unit][secondarypanel]")
{
    SecondaryPanel panel("Test");

    panel.setDefaultWidth(500);
    REQUIRE(panel.getDefaultWidth() == 500);

    panel.setDefaultWidth(100);
    REQUIRE(panel.getDefaultWidth() == 100);
}
