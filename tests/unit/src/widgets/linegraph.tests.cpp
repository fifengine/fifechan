// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2026 Fifengine contributors

#include <catch2/catch_test_macros.hpp>

#include "fifechan/point.hpp"
#include "fifechan/widgets/linegraph.hpp"

using fcn::LineGraph;

TEST_CASE("LineGraph default construction", "[unit][linegraph]")
{
    LineGraph g;
    REQUIRE_FALSE(g.isOpaque());
    REQUIRE(g.getThickness() == 1);
    REQUIRE(g.getPointVector().empty());
}

TEST_CASE("LineGraph setOpaque and isOpaque", "[unit][linegraph]")
{
    LineGraph g;
    g.setOpaque(true);
    REQUIRE(g.isOpaque());
    g.setOpaque(false);
    REQUIRE_FALSE(g.isOpaque());
}

TEST_CASE("LineGraph setThickness and getThickness", "[unit][linegraph]")
{
    LineGraph g;
    g.setThickness(5);
    REQUIRE(g.getThickness() == 5);
    g.setThickness(0);
    REQUIRE(g.getThickness() == 0);
    g.setThickness(1);
    REQUIRE(g.getThickness() == 1);
}

TEST_CASE("LineGraph setPointVector and getPointVector", "[unit][linegraph]")
{
    LineGraph g;
    fcn::PointVector const data = {fcn::Point(10, 20), fcn::Point(30, 40), fcn::Point(50, 60)};
    g.setPointVector(data);
    fcn::PointVector const & result = g.getPointVector();
    REQUIRE(result.size() == 3);
    REQUIRE(result[0].x == 10);
    REQUIRE(result[0].y == 20);
    REQUIRE(result[1].x == 30);
    REQUIRE(result[1].y == 40);
    REQUIRE(result[2].x == 50);
    REQUIRE(result[2].y == 60);
}

TEST_CASE("LineGraph setPointVector replaces data", "[unit][linegraph]")
{
    LineGraph g;
    fcn::PointVector const data1 = {fcn::Point(1, 2)};
    g.setPointVector(data1);
    REQUIRE(g.getPointVector().size() == 1);

    fcn::PointVector const data2 = {fcn::Point(3, 4), fcn::Point(5, 6)};
    g.setPointVector(data2);
    REQUIRE(g.getPointVector().size() == 2);
}

TEST_CASE("LineGraph resetPointVector clears data", "[unit][linegraph]")
{
    LineGraph g;
    fcn::PointVector const data = {fcn::Point(10, 20), fcn::Point(30, 40)};
    g.setPointVector(data);
    REQUIRE_FALSE(g.getPointVector().empty());

    g.resetPointVector();
    REQUIRE(g.getPointVector().empty());
}

TEST_CASE("LineGraph resetPointVector on empty graph", "[unit][linegraph]")
{
    LineGraph g;
    g.resetPointVector();
    REQUIRE(g.getPointVector().empty());
}

TEST_CASE("LineGraph constructor with data", "[unit][linegraph]")
{
    fcn::PointVector const data = {fcn::Point(5, 10), fcn::Point(15, 20)};
    LineGraph g(data);
    REQUIRE_FALSE(g.isOpaque());
    REQUIRE(g.getThickness() == 1);
    REQUIRE(g.getPointVector().size() == 2);
    REQUIRE(g.getPointVector()[0].x == 5);
    REQUIRE(g.getPointVector()[1].y == 20);
}

TEST_CASE("LineGraph setPointVector with empty vector", "[unit][linegraph]")
{
    LineGraph g;
    fcn::PointVector const data = {fcn::Point(1, 2)};
    g.setPointVector(data);
    REQUIRE(g.getPointVector().size() == 1);

    g.setPointVector({});
    REQUIRE(g.getPointVector().empty());
}

TEST_CASE("LineGraph getPointVector returns const reference to internal data", "[unit][linegraph]")
{
    LineGraph g;
    fcn::PointVector const data = {fcn::Point(7, 8)};
    g.setPointVector(data);
    fcn::PointVector const & ref1 = g.getPointVector();
    fcn::PointVector const & ref2 = g.getPointVector();
    REQUIRE(&ref1 == &ref2);
}

TEST_CASE("LineGraph thickness defaults after opaque toggle", "[unit][linegraph]")
{
    LineGraph g;
    g.setOpaque(true);
    REQUIRE(g.getThickness() == 1);

    g.setOpaque(false);
    REQUIRE(g.getThickness() == 1);
}
