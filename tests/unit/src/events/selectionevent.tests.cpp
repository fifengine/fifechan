// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2013 - 2026 Fifengine contributors

// Third-party library includes
#include <catch2/catch_test_macros.hpp>

// Project headers (subdirs before local)
#include <fifechan/events/selectionevent.hpp>
#include <fifechan/widget.hpp>

TEST_CASE("SelectionEvent constructors initialize properly", "[unit][selectionevent]")
{
    SECTION("null source constructor")
    {
        fcn::SelectionEvent event(nullptr);
        REQUIRE(event.getSource() == nullptr);
    }

    SECTION("widget source constructor")
    {
        fcn::Widget* widget = reinterpret_cast<fcn::Widget*>(0x1234);
        fcn::SelectionEvent event(widget);
        REQUIRE(event.getSource() == widget);
    }

    SECTION("various widget addresses")
    {
        fcn::Widget* widget1 = reinterpret_cast<fcn::Widget*>(0x1000);
        fcn::Widget* widget2 = reinterpret_cast<fcn::Widget*>(0x2000);

        fcn::SelectionEvent event1(widget1);
        fcn::SelectionEvent event2(widget2);

        REQUIRE(event1.getSource() == widget1);
        REQUIRE(event2.getSource() == widget2);
    }
}

TEST_CASE("SelectionEvent inherits from Event", "[unit][selectionevent]")
{
    SECTION("getSource works")
    {
        fcn::Widget* widget = reinterpret_cast<fcn::Widget*>(0x5678);
        fcn::SelectionEvent event(widget);

        // getSource is inherited from Event
        REQUIRE(event.getSource() == widget);
    }
}

TEST_CASE("SelectionEvent edge cases", "[unit][selectionevent]")
{
    SECTION("zero address is valid source")
    {
        fcn::Widget* zeroWidget = reinterpret_cast<fcn::Widget*>(0);
        fcn::SelectionEvent event(zeroWidget);
        REQUIRE(event.getSource() == nullptr);
    }

    SECTION("multiple events with same source")
    {
        fcn::Widget* widget = reinterpret_cast<fcn::Widget*>(0x9999);

        fcn::SelectionEvent event1(widget);
        fcn::SelectionEvent event2(widget);
        fcn::SelectionEvent event3(widget);

        REQUIRE(event1.getSource() == widget);
        REQUIRE(event2.getSource() == widget);
        REQUIRE(event3.getSource() == widget);
    }
}
