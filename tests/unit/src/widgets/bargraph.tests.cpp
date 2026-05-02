// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2026 Fifengine contributors

// Third-party library includes
#include <catch2/catch_test_macros.hpp>

// Project headers
#include "fifechan/widgets/bargraph.hpp"

using fcn::BarGraph;
using fcn::Point;

// ============================================================================
// BarGraph constructors
// ============================================================================

TEST_CASE("BarGraph default constructor", "[unit][bargraph]")
{
    BarGraph graph;

    // Default bar position should be (0,0)
    REQUIRE(graph.getBarX() == 0);
    REQUIRE(graph.getBarY() == 0);

    // Default bar size should be (0,0)
    REQUIRE(graph.getBarWidth() == 0);
    REQUIRE(graph.getBarHeight() == 0);

    // Default opaque should be false
    REQUIRE(graph.isOpaque() == false);
}

TEST_CASE("BarGraph parameterized constructor", "[unit][bargraph]")
{
    BarGraph graph(10, 20, 30, 40);

    // Check bar position
    REQUIRE(graph.getBarX() == 10);
    REQUIRE(graph.getBarY() == 20);

    // Check bar size
    REQUIRE(graph.getBarWidth() == 30);
    REQUIRE(graph.getBarHeight() == 40);
}

// ============================================================================
// BarGraph position setters/getters - happy path
// ============================================================================

TEST_CASE("BarGraph setBarX and getBarX", "[unit][bargraph]")
{
    BarGraph graph;

    graph.setBarX(50);
    REQUIRE(graph.getBarX() == 50);

    graph.setBarX(-10);
    REQUIRE(graph.getBarX() == -10);
}

TEST_CASE("BarGraph setBarY and getBarY", "[unit][bargraph]")
{
    BarGraph graph;

    graph.setBarY(100);
    REQUIRE(graph.getBarY() == 100);

    graph.setBarY(-20);
    REQUIRE(graph.getBarY() == -20);
}

TEST_CASE("BarGraph setBarPosition with x,y", "[unit][bargraph]")
{
    BarGraph graph;

    graph.setBarPosition(15, 25);
    REQUIRE(graph.getBarX() == 15);
    REQUIRE(graph.getBarY() == 25);
}

TEST_CASE("BarGraph setBarPosition with Point", "[unit][bargraph]")
{
    BarGraph graph;

    graph.setBarPosition(Point{35, 45});
    REQUIRE(graph.getBarX() == 35);
    REQUIRE(graph.getBarY() == 45);
}

// ============================================================================
// BarGraph size setters/getters - happy path
// ============================================================================

TEST_CASE("BarGraph setBarWidth and getBarWidth", "[unit][bargraph]")
{
    BarGraph graph;

    graph.setBarWidth(200);
    REQUIRE(graph.getBarWidth() == 200);

    graph.setBarWidth(0);
    REQUIRE(graph.getBarWidth() == 0);
}

TEST_CASE("BarGraph setBarHeight and getBarHeight", "[unit][bargraph]")
{
    BarGraph graph;

    graph.setBarHeight(150);
    REQUIRE(graph.getBarHeight() == 150);

    graph.setBarHeight(0);
    REQUIRE(graph.getBarHeight() == 0);
}

TEST_CASE("BarGraph setBarSize", "[unit][bargraph]")
{
    BarGraph graph;

    graph.setBarSize(300, 250);
    REQUIRE(graph.getBarWidth() == 300);
    REQUIRE(graph.getBarHeight() == 250);
}

// ============================================================================
// BarGraph opacity - happy path
// ============================================================================

TEST_CASE("BarGraph setOpaque and isOpaque", "[unit][bargraph]")
{
    BarGraph graph;

    // Default is false
    REQUIRE(graph.isOpaque() == false);

    // Set to true
    graph.setOpaque(true);
    REQUIRE(graph.isOpaque() == true);

    // Set back to false
    graph.setOpaque(false);
    REQUIRE(graph.isOpaque() == false);
}

// ============================================================================
// BarGraph edge cases
// ============================================================================

TEST_CASE("BarGraph large position values", "[unit][bargraph]")
{
    BarGraph graph;

    graph.setBarPosition(1000, 2000);

    REQUIRE(graph.getBarX() == 1000);
    REQUIRE(graph.getBarY() == 2000);
}

TEST_CASE("BarGraph zero size", "[unit][bargraph]")
{
    BarGraph graph;

    graph.setBarSize(0, 0);

    REQUIRE(graph.getBarWidth() == 0);
    REQUIRE(graph.getBarHeight() == 0);
}

TEST_CASE("BarGraph large size values", "[unit][bargraph]")
{
    BarGraph graph;

    graph.setBarSize(1000, 2000);

    REQUIRE(graph.getBarWidth() == 1000);
    REQUIRE(graph.getBarHeight() == 2000);
}

TEST_CASE("BarGraph toggle opacity multiple times", "[unit][bargraph]")
{
    BarGraph graph;

    graph.setOpaque(true);
    REQUIRE(graph.isOpaque() == true);

    graph.setOpaque(false);
    REQUIRE(graph.isOpaque() == false);

    graph.setOpaque(true);
    REQUIRE(graph.isOpaque() == true);
}

TEST_CASE("BarGraph parameterized constructor with zero values", "[unit][bargraph]")
{
    BarGraph graph(0, 0, 0, 0);

    REQUIRE(graph.getBarX() == 0);
    REQUIRE(graph.getBarY() == 0);
    REQUIRE(graph.getBarWidth() == 0);
    REQUIRE(graph.getBarHeight() == 0);
}
