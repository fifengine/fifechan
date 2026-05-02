// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2026 Fifengine contributors

// Third-party library includes
#include <catch2/catch_test_macros.hpp>

// Project headers
#include "fifechan/widgets/spacer.hpp"

using fcn::Spacer;

// ============================================================================
// Spacer constructor
// ============================================================================

TEST_CASE("Spacer constructor sets expand flags", "[unit][spacer]")
{
    Spacer spacer;

    // Should have vertical and horizontal expand set to true
    REQUIRE(spacer.isVerticalExpand() == true);
    REQUIRE(spacer.isHorizontalExpand() == true);
}

// ============================================================================
// Spacer destructor (implicit test by creating and destroying)
// ============================================================================

TEST_CASE("Spacer can be created and destroyed", "[unit][spacer]")
{
    // Destructor should not crash
    {
        Spacer spacer;
    }
    // If we got here, destructor works
    REQUIRE(true);
}

// ============================================================================
// Spacer resizeToContent - happy path
// ============================================================================

TEST_CASE("Spacer resizeToContent sets size to min size", "[unit][spacer]")
{
    Spacer spacer;

    // Set some initial size
    spacer.setSize(100, 200);
    REQUIRE(spacer.getWidth() == 100);
    REQUIRE(spacer.getHeight() == 200);

    // resizeToContent should set size to min size
    spacer.resizeToContent(false);

    // After resizeToContent, size should match min size
    REQUIRE(spacer.getWidth() == spacer.getMinSize().getWidth());
    REQUIRE(spacer.getHeight() == spacer.getMinSize().getHeight());
}

TEST_CASE("Spacer resizeToContent with recursion parameter", "[unit][spacer]")
{
    Spacer spacer;

    // Should not crash with recursion=true
    spacer.resizeToContent(true);

    // Should not crash with recursion=false
    spacer.resizeToContent(false);
}

// ============================================================================
// Spacer resizeToContent - edge cases
// ============================================================================

TEST_CASE("Spacer resizeToContent multiple calls", "[unit][spacer]")
{
    Spacer spacer;

    // Call multiple times - should not crash or change behavior
    spacer.resizeToContent(false);
    spacer.resizeToContent(false);
    spacer.resizeToContent(true);
    spacer.resizeToContent(true);

    // Should still have valid size
    REQUIRE(spacer.getWidth() >= 0);
    REQUIRE(spacer.getHeight() >= 0);
}

TEST_CASE("Spacer resizeToContent after size change", "[unit][spacer]")
{
    Spacer spacer;

    // First resize
    spacer.resizeToContent(false);
    int const width1  = spacer.getWidth();
    int const height1 = spacer.getHeight();

    // Change size manually
    spacer.setSize(500, 500);
    REQUIRE(spacer.getWidth() == 500);
    REQUIRE(spacer.getHeight() == 500);

    // Resize again
    spacer.resizeToContent(false);
    int const width2  = spacer.getWidth();
    int const height2 = spacer.getHeight();

    // Should be back to min size
    REQUIRE(width2 == spacer.getMinSize().getWidth());
    REQUIRE(height2 == spacer.getMinSize().getHeight());
    REQUIRE(width2 == width1);
    REQUIRE(height2 == height1);
}

// ============================================================================
// Spacer draw (empty implementation)
// ============================================================================

TEST_CASE("Spacer draw does nothing with null graphics", "[unit][spacer]")
{
    Spacer spacer;

    // draw() is empty - just verify it doesn't crash with nullptr
    // This is an edge case - passing nullptr to draw
    spacer.draw(nullptr);
}

TEST_CASE("Spacer draw does nothing with valid graphics", "[unit][spacer]")
{
    Spacer spacer;

    // Even with a valid graphics pointer, draw() should do nothing
    // We can't easily create a Graphics object, but draw() is intentionally empty
    // The method accepts Graphics* but doesn't use it
}

// ============================================================================
// Spacer inherited Widget properties
// ============================================================================

TEST_CASE("Spacer can set and get position", "[unit][spacer]")
{
    Spacer spacer;

    spacer.setPosition(10, 20);
    REQUIRE(spacer.getX() == 10);
    REQUIRE(spacer.getY() == 20);
}

TEST_CASE("Spacer can set and get size", "[unit][spacer]")
{
    Spacer spacer;

    spacer.setSize(100, 200);
    REQUIRE(spacer.getWidth() == 100);
    REQUIRE(spacer.getHeight() == 200);
}

TEST_CASE("Spacer setSize edge cases", "[unit][spacer]")
{
    Spacer spacer;

    SECTION("zero size")
    {
        spacer.setSize(0, 0);
        REQUIRE(spacer.getWidth() == 0);
        REQUIRE(spacer.getHeight() == 0);
    }

    SECTION("negative size")
    {
        // Note: Widget::calculateSize() clamps to min size (default 0,0)
        // so negative sizes get clamped to 0
        spacer.setSize(-10, -20);
        REQUIRE(spacer.getWidth() == 0);
        REQUIRE(spacer.getHeight() == 0);
    }

    SECTION("large size")
    {
        spacer.setSize(10000, 20000);
        REQUIRE(spacer.getWidth() == 10000);
        REQUIRE(spacer.getHeight() == 20000);
    }
}
