// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2026 Fifengine contributors

// Third-party library includes
#include <catch2/catch_test_macros.hpp>

// Project headers
#include "fifechan/events/event.hpp"
#include "fifechan/widgets/label.hpp" // Use concrete widget

using fcn::Label;
using fcn::Event;

// ============================================================================
// Event constructor and getSource
// ============================================================================

TEST_CASE("Event constructor sets source", "[unit][event]")
{
    Label widget;
    Event event(&widget);

    REQUIRE(event.getSource() == &widget);
}

TEST_CASE("Event getSource returns correct source", "[unit][event]")
{
    Label widget1;
    Label widget2;

    Event event1(&widget1);
    Event event2(&widget2);

    REQUIRE(event1.getSource() == &widget1);
    REQUIRE(event2.getSource() == &widget2);
}

TEST_CASE("Event multiple events different sources", "[unit][event]")
{
    Label widgets[5];

    for (int i = 0; i < 5; ++i) {
        Event event(&widgets[i]);
        REQUIRE(event.getSource() == &widgets[i]);
    }
}
