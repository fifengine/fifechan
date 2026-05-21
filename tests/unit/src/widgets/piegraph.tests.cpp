// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2026 Fifengine contributors

// Corresponding header include
#include "fifechan/widgets/piegraph.hpp"

// Third-party library includes
#include <catch2/catch_test_macros.hpp>

// Project headers (subdirs before local)
#include "fifechan/color.hpp"
#include "fifechan/point.hpp"

using fcn::PieGraph;

TEST_CASE("PieGraph default construction", "[unit][piegraph]")
{
    PieGraph g;
    REQUIRE_FALSE(g.isOpaque());
    REQUIRE(g.getRadius() == 10);
    REQUIRE(g.getCenterX() == 0);
    REQUIRE(g.getCenterY() == 0);
}

TEST_CASE("PieGraph constructor with center", "[unit][piegraph]")
{
    PieGraph g(fcn::Point(15, 25));
    REQUIRE_FALSE(g.isOpaque());
    REQUIRE(g.getRadius() == 10);
    REQUIRE(g.getCenterX() == 15);
    REQUIRE(g.getCenterY() == 25);
}

TEST_CASE("PieGraph setOpaque and isOpaque", "[unit][piegraph]")
{
    PieGraph g;
    g.setOpaque(true);
    REQUIRE(g.isOpaque());
    g.setOpaque(false);
    REQUIRE_FALSE(g.isOpaque());
}

TEST_CASE("PieGraph setRadius and getRadius", "[unit][piegraph]")
{
    PieGraph g;
    g.setRadius(25);
    REQUIRE(g.getRadius() == 25);
    g.setRadius(0);
    REQUIRE(g.getRadius() == 0);
    g.setRadius(-1);
    REQUIRE(g.getRadius() == -1);
    g.setRadius(10);
    REQUIRE(g.getRadius() == 10);
}

TEST_CASE("PieGraph setCenterX and getCenterX", "[unit][piegraph]")
{
    PieGraph g;
    g.setCenterX(42);
    REQUIRE(g.getCenterX() == 42);
    REQUIRE(g.getCenterY() == 0);
}

TEST_CASE("PieGraph setCenterY and getCenterY", "[unit][piegraph]")
{
    PieGraph g;
    g.setCenterY(99);
    REQUIRE(g.getCenterY() == 99);
    REQUIRE(g.getCenterX() == 0);
}

TEST_CASE("PieGraph setCenter with two ints", "[unit][piegraph]")
{
    PieGraph g;
    g.setCenter(7, 13);
    REQUIRE(g.getCenterX() == 7);
    REQUIRE(g.getCenterY() == 13);
}

TEST_CASE("PieGraph setCenter with Point object", "[unit][piegraph]")
{
    PieGraph g;
    g.setCenter(fcn::Point(100, 200));
    REQUIRE(g.getCenterX() == 100);
    REQUIRE(g.getCenterY() == 200);
}

TEST_CASE("PieGraph getCenter returns const reference to internal data", "[unit][piegraph]")
{
    PieGraph g;
    g.setCenter(fcn::Point(5, 10));
    fcn::Point const & ref1 = g.getCenter();
    fcn::Point const & ref2 = g.getCenter();
    // cppcheck-suppress knownConditionTrueFalse
    REQUIRE(&ref1 == &ref2);
}

TEST_CASE("PieGraph addSegment and clearSegments", "[unit][piegraph]")
{
    PieGraph g;
    g.addSegment(0, 90, fcn::Color(255, 0, 0));
    g.addSegment(90, 180, fcn::Color(0, 255, 0));
    g.addSegment(180, 270, fcn::Color(0, 0, 255));
    g.addSegment(270, 360, fcn::Color(255, 255, 0));
    // no crash, segments stored internally
    g.clearSegments();
    // no crash after clear
    g.clearSegments();
}

TEST_CASE("PieGraph addSegment edge cases", "[unit][piegraph]")
{
    PieGraph g;
    g.addSegment(0, 0, fcn::Color(0, 0, 0));
    g.addSegment(-90, 90, fcn::Color(255, 255, 255));
    g.addSegment(360, 720, fcn::Color(255, 0, 0));
    g.addSegment(0, 360, fcn::Color(0, 0, 255));
    // no crash with various angle values
    g.clearSegments();
}

TEST_CASE("PieGraph clearSegments on empty graph", "[unit][piegraph]")
{
    PieGraph g;
    g.clearSegments();
    // no crash when already empty
}

TEST_CASE("PieGraph construction sets defaults consistently", "[unit][piegraph]")
{
    PieGraph g(fcn::Point(-5, -10));
    REQUIRE(g.getCenterX() == -5);
    REQUIRE(g.getCenterY() == -10);
    REQUIRE(g.getRadius() == 10);
    REQUIRE_FALSE(g.isOpaque());

    g.setOpaque(true);
    g.setRadius(20);
    REQUIRE(g.isOpaque());
    REQUIRE(g.getRadius() == 20);
    REQUIRE(g.getCenterX() == -5);
}
