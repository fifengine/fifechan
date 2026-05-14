// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2026 Fifengine contributors

// Corresponding header include
#include "fifechan/events/selectionevent.hpp"

// Third-party library includes
#include <catch2/catch_test_macros.hpp>

// Project headers
#include "fifechan/widgets/label.hpp" // Use concrete widget

using fcn::Label;
using fcn::SelectionEvent;

// ============================================================================
// SelectionEvent constructor and getSource
// ============================================================================

TEST_CASE("SelectionEvent constructor sets source", "[unit][selectionevent]")
{
    Label widget;
    SelectionEvent event(&widget);

    REQUIRE(event.getSource() == &widget);
}

TEST_CASE("SelectionEvent with different widgets", "[unit][selectionevent]")
{
    Label widget1;
    Label widget2;
    Label widget3;

    SelectionEvent event1(&widget1);
    SelectionEvent event2(&widget2);
    SelectionEvent event3(&widget3);

    REQUIRE(event1.getSource() == &widget1);
    REQUIRE(event2.getSource() == &widget2);
    REQUIRE(event3.getSource() == &widget3);
}

// ============================================================================
// SelectionEvent - edge cases
// ============================================================================

TEST_CASE("SelectionEvent multiple events", "[unit][selectionevent]")
{
    Label widgets[5];

    for (auto& widget : widgets) {
        SelectionEvent event(&widget);
        REQUIRE(event.getSource() == &widget);
    }
}
