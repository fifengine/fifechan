// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2026 Fifengine contributors

#include <catch2/catch_test_macros.hpp>

#include "fifechan/widgets/activitybar.hpp"

using fcn::ActivityBar;

TEST_CASE("ActivityBar default construction", "[unit][activitybar]")
{
    ActivityBar bar;
    REQUIRE(bar.getWidth() == 48);
    REQUIRE(bar.getSpacing() == 4);
    REQUIRE(bar.isOpaque());
}

TEST_CASE("ActivityBar custom width construction", "[unit][activitybar]")
{
    ActivityBar bar(64);
    REQUIRE(bar.getWidth() == 64);
    REQUIRE(bar.getSpacing() == 4);
}

TEST_CASE("ActivityBar setWidth and getWidth", "[unit][activitybar]")
{
    ActivityBar bar;
    bar.setWidth(100);
    REQUIRE(bar.getWidth() == 100);
    bar.setWidth(0);
    REQUIRE(bar.getWidth() == 0);
}

TEST_CASE("ActivityBar setSpacing and getSpacing", "[unit][activitybar]")
{
    ActivityBar bar;
    bar.setSpacing(10);
    REQUIRE(bar.getSpacing() == 10);
    bar.setSpacing(0);
    REQUIRE(bar.getSpacing() == 0);
}

TEST_CASE("ActivityBar setSpacing affects vertical spacing", "[unit][activitybar]")
{
    ActivityBar bar;
    bar.setSpacing(8);
    REQUIRE(bar.getVerticalSpacing() == 8);
}

TEST_CASE("ActivityBar showAll with no children", "[unit][activitybar]")
{
    ActivityBar bar;
    bar.showAll();
    // no crash with no children
}

TEST_CASE("ActivityBar hideAll with no children", "[unit][activitybar]")
{
    ActivityBar bar;
    bar.hideAll();
    // no crash with no children
}

TEST_CASE("ActivityBar resizeToContent", "[unit][activitybar]")
{
    ActivityBar bar;
    bar.resizeToContent();
    REQUIRE(bar.getWidth() == 48);
}

TEST_CASE("ActivityBar adjustSize", "[unit][activitybar]")
{
    ActivityBar bar;
    bar.adjustSize();
    REQUIRE(bar.getWidth() == 48);
}

TEST_CASE("ActivityBar addActionListener with null", "[unit][activitybar]")
{
    ActivityBar bar;
    bar.addActionListener(nullptr);
    bar.removeActionListener(nullptr);
    // no crash adding and removing null
}
