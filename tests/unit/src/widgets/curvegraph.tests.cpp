// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2026 Fifengine contributors

// Corresponding header include
#include "fifechan/widgets/curvegraph.hpp"

// Third-party library includes
#include <catch2/catch_test_macros.hpp>

// Project headers (subdirs before local)
#include "fifechan/point.hpp"

using fcn::CurveGraph;

TEST_CASE("CurveGraph default construction", "[unit][curvegraph]")
{
    CurveGraph g;
    REQUIRE_FALSE(g.isOpaque());
    REQUIRE(g.getThickness() == 1);
    REQUIRE(g.isAutomaticControlPoints());
    REQUIRE(g.getPointVector().empty());
}

TEST_CASE("CurveGraph constructor with data", "[unit][curvegraph]")
{
    fcn::PointVector const data = {fcn::Point(5, 10), fcn::Point(15, 20)};
    CurveGraph g(data);
    REQUIRE_FALSE(g.isOpaque());
    REQUIRE(g.getThickness() == 1);
    REQUIRE(g.getPointVector().size() == 2);
    REQUIRE(g.getPointVector().at(0).x == 5);
    REQUIRE(g.getPointVector().at(1).y == 20);
}

TEST_CASE("CurveGraph setOpaque and isOpaque", "[unit][curvegraph]")
{
    CurveGraph g;
    g.setOpaque(true);
    REQUIRE(g.isOpaque());
    g.setOpaque(false);
    REQUIRE_FALSE(g.isOpaque());
}

TEST_CASE("CurveGraph setThickness and getThickness", "[unit][curvegraph]")
{
    CurveGraph g;
    g.setThickness(5);
    REQUIRE(g.getThickness() == 5);
    g.setThickness(0);
    REQUIRE(g.getThickness() == 0);
    g.setThickness(1);
    REQUIRE(g.getThickness() == 1);
}

TEST_CASE("CurveGraph setAutomaticControlPoints and isAutomaticControlPoints", "[unit][curvegraph]")
{
    CurveGraph g;
    REQUIRE(g.isAutomaticControlPoints());
    g.setAutomaticControlPoints(false);
    REQUIRE_FALSE(g.isAutomaticControlPoints());
    g.setAutomaticControlPoints(true);
    REQUIRE(g.isAutomaticControlPoints());
}

TEST_CASE("CurveGraph setPointVector and getPointVector", "[unit][curvegraph]")
{
    CurveGraph g;
    fcn::PointVector const data = {fcn::Point(10, 20), fcn::Point(30, 40), fcn::Point(50, 60)};
    g.setPointVector(data);
    fcn::PointVector const & result = g.getPointVector();
    REQUIRE(result.size() == 3);
    REQUIRE(result.at(0).x == 10);
    REQUIRE(result.at(0).y == 20);
    REQUIRE(result.at(1).x == 30);
    REQUIRE(result.at(1).y == 40);
    REQUIRE(result.at(2).x == 50);
    REQUIRE(result.at(2).y == 60);
}

TEST_CASE("CurveGraph setPointVector replaces data", "[unit][curvegraph]")
{
    CurveGraph g;
    fcn::PointVector const data1 = {fcn::Point(1, 2)};
    g.setPointVector(data1);
    REQUIRE(g.getPointVector().size() == 1);

    fcn::PointVector const data2 = {fcn::Point(3, 4), fcn::Point(5, 6)};
    g.setPointVector(data2);
    REQUIRE(g.getPointVector().size() == 2);
}

TEST_CASE("CurveGraph resetPointVector clears data", "[unit][curvegraph]")
{
    CurveGraph g;
    fcn::PointVector const data = {fcn::Point(10, 20), fcn::Point(30, 40)};
    g.setPointVector(data);
    REQUIRE_FALSE(g.getPointVector().empty());

    g.resetPointVector();
    REQUIRE(g.getPointVector().empty());
}

TEST_CASE("CurveGraph resetPointVector on empty graph", "[unit][curvegraph]")
{
    CurveGraph g;
    g.resetPointVector();
    REQUIRE(g.getPointVector().empty());
}

TEST_CASE("CurveGraph setPointVector with empty vector", "[unit][curvegraph]")
{
    CurveGraph g;
    fcn::PointVector const data = {fcn::Point(1, 2)};
    g.setPointVector(data);
    REQUIRE(g.getPointVector().size() == 1);

    g.setPointVector({});
    REQUIRE(g.getPointVector().empty());
}

TEST_CASE("CurveGraph getPointVector returns const reference to internal data", "[unit][curvegraph]")
{
    CurveGraph g;
    fcn::PointVector const data = {fcn::Point(7, 8)};
    g.setPointVector(data);
    fcn::PointVector const & ref1 = g.getPointVector();
    fcn::PointVector const & ref2 = g.getPointVector();
    // cppcheck-suppress knownConditionTrueFalse
    REQUIRE(&ref1 == &ref2);
}
