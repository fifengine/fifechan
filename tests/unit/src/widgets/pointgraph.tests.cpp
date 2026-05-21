// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2026 Fifengine contributors

// Corresponding header include
#include "fifechan/widgets/pointgraph.hpp"

// Third-party library includes
#include <catch2/catch_test_macros.hpp>

// Project headers (subdirs before local)
#include "fifechan/point.hpp"

using fcn::PointGraph;

TEST_CASE("PointGraph default construction", "[unit][pointgraph]")
{
    PointGraph g;
    REQUIRE_FALSE(g.isOpaque());
    REQUIRE(g.getThickness() == 1);
    REQUIRE(g.getPointVector().empty());
}

TEST_CASE("PointGraph setOpaque and isOpaque", "[unit][pointgraph]")
{
    PointGraph g;
    g.setOpaque(true);
    REQUIRE(g.isOpaque());
    g.setOpaque(false);
    REQUIRE_FALSE(g.isOpaque());
}

TEST_CASE("PointGraph setThickness and getThickness", "[unit][pointgraph]")
{
    PointGraph g;
    g.setThickness(5);
    REQUIRE(g.getThickness() == 5);
    g.setThickness(0);
    REQUIRE(g.getThickness() == 0);
    g.setThickness(1);
    REQUIRE(g.getThickness() == 1);
}

TEST_CASE("PointGraph setPointVector and getPointVector", "[unit][pointgraph]")
{
    PointGraph g;
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

TEST_CASE("PointGraph setPointVector replaces data", "[unit][pointgraph]")
{
    PointGraph g;
    fcn::PointVector const data1 = {fcn::Point(1, 2)};
    g.setPointVector(data1);
    REQUIRE(g.getPointVector().size() == 1);

    fcn::PointVector const data2 = {fcn::Point(3, 4), fcn::Point(5, 6)};
    g.setPointVector(data2);
    REQUIRE(g.getPointVector().size() == 2);
}

TEST_CASE("PointGraph resetPointVector clears data", "[unit][pointgraph]")
{
    PointGraph g;
    fcn::PointVector const data = {fcn::Point(10, 20), fcn::Point(30, 40)};
    g.setPointVector(data);
    REQUIRE_FALSE(g.getPointVector().empty());

    g.resetPointVector();
    REQUIRE(g.getPointVector().empty());
}

TEST_CASE("PointGraph resetPointVector on empty graph", "[unit][pointgraph]")
{
    PointGraph g;
    g.resetPointVector();
    REQUIRE(g.getPointVector().empty());
}

TEST_CASE("PointGraph constructor with data", "[unit][pointgraph]")
{
    fcn::PointVector const data = {fcn::Point(5, 10), fcn::Point(15, 20)};
    PointGraph g(data);
    REQUIRE_FALSE(g.isOpaque());
    REQUIRE(g.getThickness() == 1);
    REQUIRE(g.getPointVector().size() == 2);
    REQUIRE(g.getPointVector().at(0).x == 5);
    REQUIRE(g.getPointVector().at(1).y == 20);
}

TEST_CASE("PointGraph setPointVector with empty vector", "[unit][pointgraph]")
{
    PointGraph g;
    fcn::PointVector const data = {fcn::Point(1, 2)};
    g.setPointVector(data);
    REQUIRE(g.getPointVector().size() == 1);

    g.setPointVector({});
    REQUIRE(g.getPointVector().empty());
}

TEST_CASE("PointGraph getPointVector returns const reference to internal data", "[unit][pointgraph]")
{
    PointGraph g;
    fcn::PointVector const data = {fcn::Point(7, 8)};
    g.setPointVector(data);
    fcn::PointVector const & ref1 = g.getPointVector();
    fcn::PointVector const & ref2 = g.getPointVector();
    // cppcheck-suppress knownConditionTrueFalse
    REQUIRE(&ref1 == &ref2);
}

TEST_CASE("PointGraph thickness defaults after opaque toggle", "[unit][pointgraph]")
{
    PointGraph g;
    g.setOpaque(true);
    REQUIRE(g.getThickness() == 1);

    g.setOpaque(false);
    REQUIRE(g.getThickness() == 1);
}
