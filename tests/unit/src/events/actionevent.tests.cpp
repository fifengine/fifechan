// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2013 - 2026 Fifengine contributors

// Third-party library includes
#include <catch2/catch_test_macros.hpp>

// Project headers (subdirs before local)
#include <fifechan/events/actionevent.hpp>
#include <fifechan/widget.hpp>

TEST_CASE("ActionEvent constructors initialize properly", "[unit][actionevent]")
{
    SECTION("empty id constructor")
    {
        fcn::Widget* widget = reinterpret_cast<fcn::Widget*>(1);
        fcn::ActionEvent event(widget, "");

        REQUIRE(event.getSource() == widget);
        REQUIRE(event.getId() == "");
    }

    SECTION("non-empty id constructor")
    {
        fcn::Widget* widget = reinterpret_cast<fcn::Widget*>(2);
        fcn::ActionEvent event(widget, "click");

        REQUIRE(event.getSource() == widget);
        REQUIRE(event.getId() == "click");
    }

    SECTION("null source with empty id")
    {
        fcn::ActionEvent event(nullptr, "");

        REQUIRE(event.getSource() == nullptr);
        REQUIRE(event.getId() == "");
    }

    SECTION("null source with id")
    {
        fcn::ActionEvent event(nullptr, "action");

        REQUIRE(event.getSource() == nullptr);
        REQUIRE(event.getId() == "action");
    }
}

TEST_CASE("ActionEvent getId returns correct id", "[unit][actionevent]")
{
    SECTION("empty string")
    {
        fcn::ActionEvent event(nullptr, "");
        REQUIRE(event.getId() == "");
    }

    SECTION("simple id")
    {
        fcn::ActionEvent event(nullptr, "button_click");
        REQUIRE(event.getId() == "button_click");
    }

    SECTION("long id")
    {
        fcn::ActionEvent event(nullptr, "very_long_action_identifier_that_describes_the_action");
        REQUIRE(event.getId() == "very_long_action_identifier_that_describes_the_action");
    }

    SECTION("special characters in id")
    {
        fcn::ActionEvent event(nullptr, "action_123");
        REQUIRE(event.getId() == "action_123");
    }

    SECTION("multiple ids from different events")
    {
        fcn::ActionEvent event1(nullptr, "open");
        fcn::ActionEvent event2(nullptr, "save");
        fcn::ActionEvent event3(nullptr, "cancel");

        REQUIRE(event1.getId() == "open");
        REQUIRE(event2.getId() == "save");
        REQUIRE(event3.getId() == "cancel");
    }
}

TEST_CASE("ActionEvent inherits from Event", "[unit][actionevent]")
{
    SECTION("source is accessible via getSource")
    {
        fcn::Widget* widget = reinterpret_cast<fcn::Widget*>(1);
        fcn::ActionEvent event(widget, "test");

        // getSource is inherited from Event
        REQUIRE(event.getSource() == widget);
    }
}

TEST_CASE("ActionEvent edge cases", "[unit][actionevent]")
{
    SECTION("whitespace id")
    {
        fcn::ActionEvent event(nullptr, " ");
        REQUIRE(event.getId() == " ");
    }

    SECTION("id with spaces")
    {
        fcn::ActionEvent event(nullptr, "hello world");
        REQUIRE(event.getId() == "hello world");
    }

    SECTION("numeric id")
    {
        fcn::ActionEvent event(nullptr, "12345");
        REQUIRE(event.getId() == "12345");
    }

    SECTION("unicode id")
    {
        fcn::ActionEvent event(nullptr, "\u4F60\u597D");
        REQUIRE(event.getId() == "\u4F60\u597D");
    }
}
