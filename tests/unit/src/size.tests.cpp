// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2026 Fifengine contributors

// Standard library includes
#include <string>

// Third-party library includes
#include <catch2/catch_test_macros.hpp>

// Project headers
#include "fifechan/size.hpp"

using fcn::Size;

// ============================================================================
// Size constructor
// ============================================================================

TEST_CASE("Size default constructor", "[unit][size]")
{
    Size size;
    REQUIRE(size.getWidth() == 0);
    REQUIRE(size.getHeight() == 0);
}

TEST_CASE("Size parameterized constructor", "[unit][size]")
{
    Size size(10, 20);
    REQUIRE(size.getWidth() == 10);
    REQUIRE(size.getHeight() == 20);
}

// ============================================================================
// Size getters/setters
// ============================================================================

TEST_CASE("Size setWidth updates width", "[unit][size]")
{
    Size size(0, 0);
    size.setWidth(50);
    REQUIRE(size.getWidth() == 50);
}

TEST_CASE("Size setHeight updates height", "[unit][size]")
{
    Size size(0, 0);
    size.setHeight(50);
    REQUIRE(size.getHeight() == 50);
}

TEST_CASE("Size setWidth and setHeight together", "[unit][size]")
{
    Size size(0, 0);
    size.setWidth(10);
    size.setHeight(20);

    REQUIRE(size.getWidth() == 10);
    REQUIRE(size.getHeight() == 20);
}
