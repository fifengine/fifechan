// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2026 Fifengine contributors

// Third-party library includes
#include <catch2/catch_test_macros.hpp>

// Project headers
#include "fifechan/dragdrop.hpp"
#include "fifechan/events/dragevent.hpp"
#include "fifechan/widgets/label.hpp" // Use concrete widget

using namespace fcn;

// ============================================================================
// DragEvent constructor and getters
// ============================================================================

TEST_CASE("DragEvent constructor sets source and values", "[unit][dragevent]")
{
    Label sourceWidget;
    DragPayload payload;
    DragEvent event(&sourceWidget, &payload, DragEvent::Type::Enter, 100, 200, 300, 400);

    REQUIRE(event.getSource() == &sourceWidget);
    REQUIRE(event.getType() == DragEvent::Type::Enter);
    REQUIRE(event.getX() == 100);
    REQUIRE(event.getY() == 200);
    REQUIRE(event.getScreenX() == 300);
    REQUIRE(event.getScreenY() == 400);
    REQUIRE(event.getPayload() == &payload);
}

TEST_CASE("DragEvent different types", "[unit][dragevent]")
{
    Label sourceWidget;
    DragPayload payload;

    SECTION("Enter type")
    {
        DragEvent event(&sourceWidget, &payload, DragEvent::Type::Enter, 0, 0, 0, 0);
        REQUIRE(event.getType() == DragEvent::Type::Enter);
    }

    SECTION("Leave type")
    {
        DragEvent event(&sourceWidget, &payload, DragEvent::Type::Leave, 0, 0, 0, 0);
        REQUIRE(event.getType() == DragEvent::Type::Leave);
    }

    SECTION("Hover type")
    {
        DragEvent event(&sourceWidget, &payload, DragEvent::Type::Hover, 0, 0, 0, 0);
        REQUIRE(event.getType() == DragEvent::Type::Hover);
    }

    SECTION("Drop type")
    {
        DragEvent event(&sourceWidget, &payload, DragEvent::Type::Drop, 0, 0, 0, 0);
        REQUIRE(event.getType() == DragEvent::Type::Drop);
    }

    SECTION("Cancel type")
    {
        DragEvent event(&sourceWidget, &payload, DragEvent::Type::Cancel, 0, 0, 0, 0);
        REQUIRE(event.getType() == DragEvent::Type::Cancel);
    }
}

// ============================================================================
// DragEvent coordinate getters
// ============================================================================

TEST_CASE("DragEvent getX and getY", "[unit][dragevent]")
{
    Label sourceWidget;
    DragPayload payload;

    SECTION("positive coordinates")
    {
        DragEvent event(&sourceWidget, &payload, DragEvent::Type::Hover, 50, 60, 0, 0);
        REQUIRE(event.getX() == 50);
        REQUIRE(event.getY() == 60);
    }

    SECTION("zero coordinates")
    {
        DragEvent event(&sourceWidget, &payload, DragEvent::Type::Hover, 0, 0, 0, 0);
        REQUIRE(event.getX() == 0);
        REQUIRE(event.getY() == 0);
    }

    SECTION("negative coordinates")
    {
        DragEvent event(&sourceWidget, &payload, DragEvent::Type::Hover, -10, -20, 0, 0);
        REQUIRE(event.getX() == -10);
        REQUIRE(event.getY() == -20);
    }
}

TEST_CASE("DragEvent getScreenX and getScreenY", "[unit][dragevent]")
{
    Label sourceWidget;
    DragPayload payload;

    SECTION("positive screen coordinates")
    {
        DragEvent event(&sourceWidget, &payload, DragEvent::Type::Drop, 0, 0, 500, 600);
        REQUIRE(event.getScreenX() == 500);
        REQUIRE(event.getScreenY() == 600);
    }

    SECTION("zero screen coordinates")
    {
        DragEvent event(&sourceWidget, &payload, DragEvent::Type::Drop, 0, 0, 0, 0);
        REQUIRE(event.getScreenX() == 0);
        REQUIRE(event.getScreenY() == 0);
    }

    SECTION("negative screen coordinates")
    {
        DragEvent event(&sourceWidget, &payload, DragEvent::Type::Drop, 0, 0, -100, -200);
        REQUIRE(event.getScreenX() == -100);
        REQUIRE(event.getScreenY() == -200);
    }
}

// ============================================================================
// DragEvent getPayload
// ============================================================================

TEST_CASE("DragEvent getPayload returns correct payload", "[unit][dragevent]")
{
    Label sourceWidget;
    DragPayload payload1;
    DragPayload payload2;

    DragEvent event1(&sourceWidget, &payload1, DragEvent::Type::Enter, 0, 0, 0, 0);
    DragEvent event2(&sourceWidget, &payload2, DragEvent::Type::Enter, 0, 0, 0, 0);

    REQUIRE(event1.getPayload() == &payload1);
    REQUIRE(event2.getPayload() == &payload2);
}

// ============================================================================
// DragEvent edge cases
// ============================================================================

TEST_CASE("DragEvent with null payload", "[unit][dragevent]")
{
    Label sourceWidget;

    DragEvent event(&sourceWidget, nullptr, DragEvent::Type::Enter, 0, 0, 0, 0);

    REQUIRE(event.getPayload() == nullptr);
}

TEST_CASE("DragEvent with different source widgets", "[unit][dragevent]")
{
    Label widget1;
    Label widget2;
    DragPayload payload;

    DragEvent event1(&widget1, &payload, DragEvent::Type::Enter, 0, 0, 0, 0);
    DragEvent event2(&widget2, &payload, DragEvent::Type::Enter, 0, 0, 0, 0);

    REQUIRE(event1.getSource() == &widget1);
    REQUIRE(event2.getSource() == &widget2);
}
