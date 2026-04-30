// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2013 - 2026 Fifengine contributors

// Third-party library includes
#include <catch2/catch_test_macros.hpp>

// Project headers (subdirs before local)
#include <fifechan/events/event.hpp>
#include <fifechan/widget.hpp>

TEST_CASE("Event constructors initialize properly", "[unit][event]")
{
    SECTION("nullptr source constructor")
    {
        fcn::Event event(nullptr);
        REQUIRE(event.getSource() == nullptr);
    }

    SECTION("widget source constructor")
    {
        // Fake widget pointer - never dereferenced, just stored/returned
        fcn::Widget* widget = reinterpret_cast<fcn::Widget*>(1);
        fcn::Event event(widget);
        REQUIRE(event.getSource() == widget);
    }
}

TEST_CASE("Event getSource returns correct source", "[unit][event]")
{
    SECTION("null source")
    {
        fcn::Event event(nullptr);
        REQUIRE(event.getSource() == nullptr);
    }

    SECTION("various widget addresses")
    {
        fcn::Widget* widget1 = reinterpret_cast<fcn::Widget*>(1);
        fcn::Widget* widget2 = reinterpret_cast<fcn::Widget*>(2);
        fcn::Widget* widget3 = nullptr;

        fcn::Event event1(widget1);
        fcn::Event event2(widget2);
        fcn::Event event3(widget3);

        REQUIRE(event1.getSource() == widget1);
        REQUIRE(event2.getSource() == widget2);
        REQUIRE(event3.getSource() == nullptr);
    }
}

TEST_CASE("Event edge cases", "[unit][event]")
{
    SECTION("zero address is null")
    {
        fcn::Widget* zeroWidget = reinterpret_cast<fcn::Widget*>(0);
        fcn::Event event(zeroWidget);
        REQUIRE(event.getSource() == nullptr);
    }

    SECTION("max address is valid source")
    {
        fcn::Widget* maxWidget = reinterpret_cast<fcn::Widget*>(UINTPTR_MAX);
        fcn::Event event(maxWidget);
        REQUIRE(event.getSource() == maxWidget);
    }
}
