// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2013 - 2026 Fifengine contributors

#include <catch2/catch_test_macros.hpp>

#include "fifechan/size.hpp"

TEST_CASE("Size stores and updates dimensions", "[unit][size]")
{
    fcn::Size size(10, 20);

    SECTION("constructor initializes width and height")
    {
        REQUIRE(size.getWidth() == 10);
        REQUIRE(size.getHeight() == 20);
    }

    SECTION("setters update both values")
    {
        size.setWidth(42);
        size.setHeight(24);

        REQUIRE(size.getWidth() == 42);
        REQUIRE(size.getHeight() == 24);
    }
}
