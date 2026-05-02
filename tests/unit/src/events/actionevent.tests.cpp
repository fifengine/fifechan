// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2026 Fifengine contributors

// Third-party library includes
#include <string>

#include <catch2/catch_test_macros.hpp>

// Project headers
#include "fifechan/events/actionevent.hpp"
#include "fifechan/widgets/label.hpp" // Use concrete widget

using namespace fcn;

// ============================================================================
// ActionEvent constructor and getId
// ============================================================================

TEST_CASE("ActionEvent constructor sets source and id", "[unit][actionevent]")
{
    Label widget;
    ActionEvent event(&widget, "test_id");

    REQUIRE(event.getSource() == &widget);
    REQUIRE(event.getId() == "test_id");
}

TEST_CASE("ActionEvent getId returns correct id", "[unit][actionevent]")
{
    Label widget;

    SECTION("simple id")
    {
        ActionEvent event(&widget, "button_click");
        REQUIRE(event.getId() == "button_click");
    }

    SECTION("empty id")
    {
        ActionEvent event(&widget, "");
        REQUIRE(event.getId() == "");
    }

    SECTION("id with special characters")
    {
        ActionEvent event(&widget, "btn_123!@#");
        REQUIRE(event.getId() == "btn_123!@#");
    }
}

TEST_CASE("ActionEvent with different widgets", "[unit][actionevent]")
{
    Label widget1;
    Label widget2;

    ActionEvent event1(&widget1, "action1");
    ActionEvent event2(&widget2, "action2");

    REQUIRE(event1.getSource() == &widget1);
    REQUIRE(event2.getSource() == &widget2);
    REQUIRE(event1.getId() == "action1");
    REQUIRE(event2.getId() == "action2");
}

// ============================================================================
// ActionEvent - edge cases
// ============================================================================

TEST_CASE("ActionEvent with long id string", "[unit][actionevent]")
{
    Label widget;
    std::string longId(1000, 'x'); // 1000 character string

    ActionEvent event(&widget, longId);
    REQUIRE(event.getId() == longId);
}

TEST_CASE("ActionEvent id is copied not referenced", "[unit][actionevent]")
{
    Label widget;
    std::string id = "original";

    ActionEvent event(&widget, id);

    // Modify original string
    id = "modified";

    // Event should still have original id
    REQUIRE(event.getId() == "original");
}
