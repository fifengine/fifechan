// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2026 Fifengine contributors

// Corresponding header include
#include "fifechan/widgets/menubar.hpp"

// Third-party library includes
#include <catch2/catch_test_macros.hpp>

using fcn::MenuBar;

TEST_CASE("MenuBar default construction", "[unit][menubar]")
{
    MenuBar bar;
    REQUIRE(bar.isOpaque());
    REQUIRE(bar.getFixedHeight() == 24);
    REQUIRE_FALSE(bar.isMenuOpen());
    REQUIRE(bar.getOpenMenu() == nullptr);
}

TEST_CASE("MenuBar closeAll when no menu open", "[unit][menubar]")
{
    MenuBar bar;
    bar.closeAll();
    REQUIRE_FALSE(bar.isMenuOpen());
}

TEST_CASE("MenuBar addMenu with text only", "[unit][menubar]")
{
    MenuBar bar;
    auto const * item = bar.addMenu("File");
    REQUIRE(item != nullptr);
    REQUIRE(bar.isMenuOpen() == false);
    REQUIRE(bar.getOpenMenu() == nullptr);
}

TEST_CASE("MenuBar addMenu with null popup", "[unit][menubar]")
{
    MenuBar bar;
    auto const * item = bar.addMenu("Edit", nullptr);
    REQUIRE(item != nullptr);
}

TEST_CASE("MenuBar multiple menus", "[unit][menubar]")
{
    MenuBar bar;
    bar.addMenu("File");
    bar.addMenu("Edit");
    bar.addMenu("View");
    REQUIRE(bar.getChildrenCount() == 3);
}
