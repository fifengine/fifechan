// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2013 - 2026 Fifengine contributors

// Standard library includes
#include <stdexcept>

// Third-party library includes
#include <catch2/catch_test_macros.hpp>

// Project headers (subdirs before local)
#include "fifechan/rectangle.hpp"

TEST_CASE("Rectangle constructors initialize members", "[unit][rectangle]")
{
    SECTION("default constructor initializes to zero")
    {
        fcn::Rectangle const rect;

        REQUIRE(rect.x == 0);
        REQUIRE(rect.y == 0);
        REQUIRE(rect.width == 0);
        REQUIRE(rect.height == 0);
    }

    SECTION("parameter constructor initializes all members")
    {
        fcn::Rectangle const rect(10, 20, 30, 40);

        REQUIRE(rect.x == 10);
        REQUIRE(rect.y == 20);
        REQUIRE(rect.width == 30);
        REQUIRE(rect.height == 40);
    }
}

TEST_CASE("Rectangle setAll updates all members", "[unit][rectangle]")
{
    fcn::Rectangle rect;

    rect.setAll(5, 10, 15, 20);

    REQUIRE(rect.x == 5);
    REQUIRE(rect.y == 10);
    REQUIRE(rect.width == 15);
    REQUIRE(rect.height == 20);
}

TEST_CASE("Rectangle point containment checks", "[unit][rectangle]")
{
    fcn::Rectangle const rect(0, 0, 10, 10);

    SECTION("point inside returns true")
    {
        REQUIRE(rect.isContaining(5, 5) == true);
    }

    SECTION("point on edge returns true")
    {
        REQUIRE(rect.isContaining(0, 0) == true);
        REQUIRE(rect.isContaining(9, 9) == true);
    }

    SECTION("point outside returns false")
    {
        REQUIRE(rect.isContaining(10, 10) == false);
        REQUIRE(rect.isContaining(-1, 5) == false);
        REQUIRE(rect.isContaining(5, -1) == false);
    }

    SECTION("empty rectangle returns false")
    {
        fcn::Rectangle const empty;
        REQUIRE(empty.isContaining(0, 0) == false);
    }
}

TEST_CASE("Rectangle intersection checks", "[unit][rectangle]")
{
    SECTION("overlapping rectangles return true")
    {
        fcn::Rectangle const rect1(0, 0, 10, 10);
        fcn::Rectangle const rect2(5, 5, 10, 10);

        REQUIRE(rect1.isIntersecting(rect2) == true);
    }

    SECTION("non-overlapping rectangles return false")
    {
        fcn::Rectangle const rect1(0, 0, 10, 10);
        fcn::Rectangle const rect2(20, 20, 10, 10);

        REQUIRE(rect1.isIntersecting(rect2) == false);
    }

    SECTION("touching rectangles return false")
    {
        fcn::Rectangle const rect1(0, 0, 10, 10);
        fcn::Rectangle const rect2(10, 0, 10, 10);

        REQUIRE(rect1.isIntersecting(rect2) == false);
    }
}

TEST_CASE("Rectangle isEmpty works correctly", "[unit][rectangle]")
{
    SECTION("Empty rectangle when width is zero")
    {
        fcn::Rectangle const rect(0, 0, 0, 10);
        REQUIRE(rect.isEmpty() == true);
    }

    SECTION("Empty rectangle when height is zero")
    {
        fcn::Rectangle const rect(0, 0, 10, 0);
        REQUIRE(rect.isEmpty() == true);
    }

    SECTION("Non-empty rectangle")
    {
        fcn::Rectangle const rect(0, 0, 10, 10);
        REQUIRE(rect.isEmpty() == false);
    }
}

TEST_CASE("Rectangle rectangle intersection checks", "[unit][rectangle]")
{
    fcn::Rectangle const rect1(0, 0, 10, 10);
    fcn::Rectangle const rect2(10, 10, 10, 10);

    SECTION("union combines rectangles")
    {
        fcn::Rectangle const result = rect1 + rect2;

        REQUIRE(result.x == 0);
        REQUIRE(result.y == 0);
        REQUIRE(result.width == 20);
        REQUIRE(result.height == 20);
    }

    SECTION("union with empty returns same rectangle")
    {
        fcn::Rectangle const empty;
        fcn::Rectangle const result = rect1 + empty;

        REQUIRE(result.x == rect1.x);
        REQUIRE(result.y == rect1.y);
        REQUIRE(result.width == rect1.width);
        REQUIRE(result.height == rect1.height);
    }
}

TEST_CASE("Rectangle union assignment operator", "[unit][rectangle]")
{
    fcn::Rectangle rect(0, 0, 10, 10);
    fcn::Rectangle const other(10, 10, 10, 10);

    rect += other;

    REQUIRE(rect.x == 0);
    REQUIRE(rect.y == 0);
    REQUIRE(rect.width == 20);
    REQUIRE(rect.height == 20);
}

TEST_CASE("Rectangle intersection method", "[unit][rectangle]")
{
    fcn::Rectangle const rect1(0, 0, 10, 10);
    fcn::Rectangle const rect2(5, 5, 10, 10);

    SECTION("intersection returns overlapping area")
    {
        fcn::Rectangle const result = rect1.intersection(rect2);

        REQUIRE(result.x == 5);
        REQUIRE(result.y == 5);
        REQUIRE(result.width == 5);
        REQUIRE(result.height == 5);
    }

    SECTION("non-overlapping returns empty")
    {
        fcn::Rectangle const nonOverlapping(20, 20, 10, 10);
        fcn::Rectangle const result = rect1.intersection(nonOverlapping);

        REQUIRE(result.width == 0);
        REQUIRE(result.height == 0);
    }

    SECTION("empty intersection returns empty")
    {
        fcn::Rectangle const empty;
        fcn::Rectangle const result = rect1.intersection(empty);

        REQUIRE(result.isEmpty() == true);
    }
}

TEST_CASE("Rectangle output stream", "[unit][rectangle]")
{
    fcn::Rectangle const rect(1, 2, 3, 4);
    std::ostringstream oss;

    oss << rect;

    REQUIRE(oss.str() == "Rectangle [x = 1, y = 2, width = 3, height = 4]");
}
