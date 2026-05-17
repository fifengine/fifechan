// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2026 Fifengine contributors

// Standard library includes
#include <string>

// Third-party library includes
#include <catch2/catch_test_macros.hpp>

// Project headers
#include "fifechan/widgets/statusbar.hpp"

using fcn::StatusBar;

TEST_CASE("StatusBar default constructor", "[unit][statusbar]")
{
    StatusBar sb;
    REQUIRE(sb.getText().empty());
    REQUIRE(sb.getHeight() == 24);
}

TEST_CASE("StatusBar setText and getText", "[unit][statusbar]")
{
    StatusBar sb;
    sb.setText("Ready");
    REQUIRE(sb.getText() == "Ready");

    sb.setText("");
    REQUIRE(sb.getText().empty());

    sb.setText("Loading...");
    REQUIRE(sb.getText() == "Loading...");
}

TEST_CASE("StatusBar internal areas exist", "[unit][statusbar]")
{
    StatusBar sb;
    REQUIRE(sb.getIconArea() != nullptr);
    REQUIRE(sb.getLeftArea() != nullptr);
    REQUIRE(sb.getRightArea() != nullptr);
}

TEST_CASE("StatusBar shrink priority", "[unit][statusbar]")
{
    StatusBar sb;
    REQUIRE_FALSE(sb.isShrinkPriorityEnabled());

    sb.setShrinkPriorityEnabled(true);
    REQUIRE(sb.isShrinkPriorityEnabled());

    sb.setShrinkPriorityEnabled(false);
    REQUIRE_FALSE(sb.isShrinkPriorityEnabled());
}

TEST_CASE("StatusBar truncate with ellipsis", "[unit][statusbar]")
{
    StatusBar sb;
    // Default value
    bool const initial = sb.isTruncateWithEllipsis();

    sb.setTruncateWithEllipsis(!initial);
    REQUIRE(sb.isTruncateWithEllipsis() == !initial);

    sb.setTruncateWithEllipsis(initial);
    REQUIRE(sb.isTruncateWithEllipsis() == initial);
}

TEST_CASE("StatusBar setSize", "[unit][statusbar]")
{
    StatusBar sb;
    sb.setSize(500, 30);
    REQUIRE(sb.getWidth() == 500);
    REQUIRE(sb.getHeight() == 30);
}

TEST_CASE("StatusBar adjustSize does not crash", "[unit][statusbar]")
{
    StatusBar sb;
    sb.adjustSize();
    REQUIRE(true);
}

TEST_CASE("StatusBar resizeToContent does not crash", "[unit][statusbar]")
{
    StatusBar sb;
    sb.resizeToContent();
    REQUIRE(true);
}
