// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2026 Fifengine contributors

// Third-party library includes
#include <catch2/catch_test_macros.hpp>

// Project headers
#include "fifechan/widgets/icon.hpp"

using fcn::Icon;

TEST_CASE("Icon default constructor", "[unit][icon]")
{
    Icon icon;
    REQUIRE(icon.getImage() == nullptr);
    REQUIRE(!icon.isScaling());
    REQUIRE(!icon.isTiling());
    REQUIRE(icon.isOpaque());
}

TEST_CASE("Icon setScaling and isScaling", "[unit][icon]")
{
    Icon icon;
    icon.setScaling(true);
    REQUIRE(icon.isScaling());

    icon.setScaling(false);
    REQUIRE(!icon.isScaling());
}

TEST_CASE("Icon setTiling and isTiling", "[unit][icon]")
{
    Icon icon;
    icon.setTiling(true);
    REQUIRE(icon.isTiling());

    icon.setTiling(false);
    REQUIRE(!icon.isTiling());
}

TEST_CASE("Icon setOpaque and isOpaque", "[unit][icon]")
{
    Icon icon;
    REQUIRE(icon.isOpaque());

    icon.setOpaque(false);
    REQUIRE(!icon.isOpaque());

    icon.setOpaque(true);
    REQUIRE(icon.isOpaque());
}

TEST_CASE("Icon adjustSize with no image", "[unit][icon]")
{
    Icon icon;
    icon.setBorderSize(0);
    icon.setPadding(0);
    icon.adjustSize();
    REQUIRE(icon.getWidth() == 0);
    REQUIRE(icon.getHeight() == 0);
}

TEST_CASE("Icon adjustSize with border and padding", "[unit][icon]")
{
    Icon icon;
    icon.setBorderSize(2);
    icon.setPaddingLeft(3);
    icon.setPaddingRight(3);
    icon.setPaddingTop(1);
    icon.setPaddingBottom(1);
    icon.adjustSize();
    REQUIRE(icon.getWidth() == 10);
    REQUIRE(icon.getHeight() == 6);
}

TEST_CASE("Icon resizeToContent", "[unit][icon]")
{
    Icon icon;
    icon.resizeToContent();
    REQUIRE(true);
}

TEST_CASE("Icon resizeToContent with recursion", "[unit][icon]")
{
    Icon icon;
    icon.resizeToContent(true);
    REQUIRE(true);

    icon.resizeToContent(false);
    REQUIRE(true);
}
