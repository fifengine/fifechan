// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2013 - 2026 Fifengine contributors

#include <stdexcept>

#include <catch2/catch_test_macros.hpp>

#include "fifechan/color.hpp"

TEST_CASE("Color constructors parse common formats", "[unit][color]")
{
    SECTION("hex integer constructor")
    {
        fcn::Color const color(0x112233);
        REQUIRE(color.r == 0x11);
        REQUIRE(color.g == 0x22);
        REQUIRE(color.b == 0x33);
        REQUIRE(color.a == 0xFF);
    }

    SECTION("rgb string constructor")
    {
        fcn::Color const color("rgb(1,2,3)");
        REQUIRE(color.r == 1);
        REQUIRE(color.g == 2);
        REQUIRE(color.b == 3);
        REQUIRE(color.a == 255);
    }

    SECTION("invalid rgb string throws")
    {
        REQUIRE_THROWS_AS(fcn::Color("rgb(1,2)"), std::invalid_argument);
    }
}

TEST_CASE("Color arithmetic clamps channels", "[unit][color]")
{
    fcn::Color const lhs(250, 10, 5, 200);
    fcn::Color const rhs(10, 20, 30, 100);

    SECTION("addition is clamped")
    {
        fcn::Color const result = lhs + rhs;
        REQUIRE(result.r == 255);
        REQUIRE(result.g == 30);
        REQUIRE(result.b == 35);
        REQUIRE(result.a == 200);
    }

    SECTION("subtraction is clamped")
    {
        fcn::Color const result = lhs - rhs;
        REQUIRE(result.r == 240);
        REQUIRE(result.g == 0);
        REQUIRE(result.b == 0);
        REQUIRE(result.a == 200);
    }
}
