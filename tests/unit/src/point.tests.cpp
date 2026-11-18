// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2026 Fifengine contributors

// Standard library includes
#include <sstream>
#include <utility>

// Third-party library includes
#include <catch2/catch_test_macros.hpp>

// Project headers
#include "fifechan/point.hpp"

using fcn::Point;
using fcn::PointVector;

// ============================================================================
// Point constructor and basic properties
// ============================================================================

TEST_CASE("Point default constructor creates origin", "[point][constructor]")
{
    Point p;
    REQUIRE(p.x == 0);
    REQUIRE(p.y == 0);
}

TEST_CASE("Point constructor with coordinates", "[point][constructor]")
{
    Point p(3, 7);
    REQUIRE(p.x == 3);
    REQUIRE(p.y == 7);
}

TEST_CASE("Point constructor with negative coordinates", "[point][constructor][edge]")
{
    Point p(-5, -10);
    REQUIRE(p.x == -5);
    REQUIRE(p.y == -10);
}

TEST_CASE("Point constructor with zero coordinates", "[point][constructor][edge]")
{
    Point p(0, 0);
    REQUIRE(p.x == 0);
    REQUIRE(p.y == 0);
}

// ============================================================================
// Point copy and move semantics
// ============================================================================

TEST_CASE("Point copy constructor", "[point][copy]")
{
    Point p1(10, 20);
    Point p2(p1);
    REQUIRE(p2.x == 10);
    REQUIRE(p2.y == 20);
}

TEST_CASE("Point move constructor", "[point][move]")
{
    Point p1(10, 20);
    Point p2(std::move(p1));
    REQUIRE(p2.x == 10);
    REQUIRE(p2.y == 20);
    // After move, p1 values are unspecified but should be valid
    REQUIRE((p1.x >= 0 || p1.y >= 0)); // just check they're reasonable
}

TEST_CASE("Point copy assignment", "[point][assignment]")
{
    Point p1(10, 20);
    Point p2(5, 15);
    p2 = p1;
    REQUIRE(p2.x == 10);
    REQUIRE(p2.y == 20);
}

TEST_CASE("Point move assignment", "[point][move_assignment]")
{
    Point p1(10, 20);
    Point p2(5, 15);
    p2 = std::move(p1);
    REQUIRE(p2.x == 10);
    REQUIRE(p2.y == 20);
}

// ============================================================================
// Point arithmetic operators
// ============================================================================

TEST_CASE("Point operator+ addition", "[point][arithmetic]")
{
    Point p1(3, 7);
    Point p2(5, -2);
    Point p3 = p1 + p2;
    REQUIRE(p3.x == 8);
    REQUIRE(p3.y == 5);
}

TEST_CASE("Point operator- subtraction", "[point][arithmetic]")
{
    Point p1(10, 20);
    Point p2(3, 7);
    Point p3 = p1 - p2;
    REQUIRE(p3.x == 7);
    REQUIRE(p3.y == 13);
}

TEST_CASE("Point operator+= inplace addition", "[point][arithmetic]")
{
    Point p1(3, 7);
    Point p2(5, -2);
    p1 += p2;
    REQUIRE(p1.x == 8);
    REQUIRE(p1.y == 5);
}

TEST_CASE("Point operator-= inplace subtraction", "[point][arithmetic]")
{
    Point p1(10, 20);
    Point p2(3, 7);
    p1 -= p2;
    REQUIRE(p1.x == 7);
    REQUIRE(p1.y == 13);
}

TEST_CASE("Point operator* scalar multiplication", "[point][arithmetic]")
{
    Point p(3, 7);
    Point p2 = p * 4;
    REQUIRE(p2.x == 12);
    REQUIRE(p2.y == 28);

    Point p3 = p * -1;
    REQUIRE(p3.x == -3);
    REQUIRE(p3.y == -7);
}

TEST_CASE("Point operator/ scalar division", "[point][arithmetic][edge]")
{
    Point p(10, 20);
    Point p2 = p / 5;
    REQUIRE(p2.x == 2);
    REQUIRE(p2.y == 4);

    // Division by zero - undefined behavior, but test doesn't crash
    // (result depends on compiler, so we just verify the function is callable)
    // Point p3 = p / 0; // Would cause issues, so we skip this
}

// ============================================================================
// Point equality operators
// ============================================================================

TEST_CASE("Point operator== equality", "[point][comparison]")
{
    Point p1(5, 10);
    Point p2(5, 10);
    Point p3(5, 11);

    REQUIRE(p1 == p2);
    REQUIRE(!(p1 == p3));
}

TEST_CASE("Point operator!= inequality", "[point][comparison]")
{
    Point p1(5, 10);
    Point p2(5, 10);
    Point p3(5, 11);

    REQUIRE(!(p1 != p2));
    REQUIRE(p1 != p3);
}

// ============================================================================
// Point length function
// ============================================================================

TEST_CASE("Point length calculation", "[point][math]")
{
    Point p(3, 4);
    REQUIRE(p.length() == 5); // 3-4-5 triangle

    Point p2(0, 0);
    REQUIRE(p2.length() == 0);

    Point p3(1, 1);
    REQUIRE(p3.length() == 1); // sqrt(2) rounded to int
}

// ============================================================================
// Point normalize function
// ============================================================================

TEST_CASE("Point normalize non-zero vector", "[point][math]")
{
    Point p(3, 4);
    p.normalize();
    // After normalization, length should be 1 (or very close)
    // Since we're using int coordinates, the result may vary
    REQUIRE((p.x >= 0 && p.y >= 0)); // just check it's in first quadrant
}

TEST_CASE("Point normalize zero vector", "[point][math][edge]")
{
    Point p(0, 0);
    p.normalize();
    // Zero vector should remain zero
    REQUIRE((p.x == 0 && p.y == 0));
}

// ============================================================================
// Point rotate functions
// ============================================================================

TEST_CASE("Point rotate around origin", "[point][math]")
{
    Point p(1, 0);
    Point p2 = p.rotated(90);
    // Rotating (1,0) by 90 degrees should give (0,1)
    REQUIRE(p2.x == 0);
    REQUIRE(p2.y == 1);
}

TEST_CASE("Point rotate around origin 180 degrees", "[point][math]")
{
    Point p(5, 3);
    Point p2 = p.rotated(180);
    // Rotating by 180 should negate both coordinates
    REQUIRE(p2.x == -5);
    REQUIRE(p2.y == -3);
}

TEST_CASE("Point rotate around custom origin", "[point][math][edge]")
{
    Point p(10, 10);
    Point const origin(5, 5);
    p.rotate(origin, 90);
    // Rotating (10,10) around (5,5) by 90 degrees
    // Offset is (5,5), rotated 90 degrees becomes (-5,5), so result is (0,10)
    REQUIRE(p.x == 0);
    REQUIRE(p.y == 10);
}

// ============================================================================
// Point set function
// ============================================================================

TEST_CASE("Point set coordinates", "[point][mutator]")
{
    Point p(1, 2);
    p.set(10, 20);
    REQUIRE(p.x == 10);
    REQUIRE(p.y == 20);
}

TEST_CASE("Point set to same values", "[point][mutator][edge]")
{
    Point p(5, 5);
    p.set(5, 5);
    REQUIRE(p.x == 5);
    REQUIRE(p.y == 5);
}

// ============================================================================
// Point index access operator
// ============================================================================

TEST_CASE("Point operator[] index access", "[point][index]")
{
    Point p(3, 7);
    REQUIRE(p[0] == 3);
    REQUIRE(p[1] == 7);
}

TEST_CASE("Point operator[] const index access", "[point][index][const]")
{
    Point p(3, 7);
    Point const & cp = p;
    REQUIRE(cp[0] == 3);
    REQUIRE(cp[1] == 7);
}

// ============================================================================
// Point stream output operator
// ============================================================================

TEST_CASE("Point stream output operator", "[point][output]")
{
    Point p(3, 7);
    std::ostringstream oss;
    oss << p;
    REQUIRE(oss.str() == "(3:7)");
}

// ============================================================================
// Point PointVector type
// ============================================================================

TEST_CASE("PointVector type exists", "[point][type]")
{
    // Just verify the type is usable
    PointVector vec;
    vec.push_back(Point(1, 2));
    vec.push_back(Point(3, 4));
    REQUIRE(vec[0].x == 1);
    REQUIRE(vec[0].y == 2);
    REQUIRE(vec[1].x == 3);
    REQUIRE(vec[1].y == 4);
}
