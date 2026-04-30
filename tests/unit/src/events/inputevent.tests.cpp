// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2013 - 2026 Fifengine contributors

// Third-party library includes
#include <catch2/catch_test_macros.hpp>

// Project headers (subdirs before local)
#include <fifechan/events/inputevent.hpp>
#include <fifechan/widget.hpp>

TEST_CASE("InputEvent constructors initialize properly", "[unit][inputevent]")
{
    SECTION("all modifiers false")
    {
        fcn::InputEvent event(nullptr, nullptr, false, false, false, false);

        REQUIRE(event.isShiftPressed() == false);
        REQUIRE(event.isControlPressed() == false);
        REQUIRE(event.isAltPressed() == false);
        REQUIRE(event.isMetaPressed() == false);
        REQUIRE(event.isConsumed() == false);
    }

    SECTION("all modifiers true")
    {
        fcn::InputEvent event(nullptr, nullptr, true, true, true, true);

        REQUIRE(event.isShiftPressed() == true);
        REQUIRE(event.isControlPressed() == true);
        REQUIRE(event.isAltPressed() == true);
        REQUIRE(event.isMetaPressed() == true);
    }

    SECTION("mixed modifiers")
    {
        fcn::InputEvent event(nullptr, nullptr, true, false, true, false);

        REQUIRE(event.isShiftPressed() == true);
        REQUIRE(event.isControlPressed() == false);
        REQUIRE(event.isAltPressed() == true);
        REQUIRE(event.isMetaPressed() == false);
    }

    SECTION("with source widget")
    {
        fcn::Widget* widget      = reinterpret_cast<fcn::Widget*>(0x1234);
        fcn::Widget* distributor = reinterpret_cast<fcn::Widget*>(0x5678);

        fcn::InputEvent event(widget, distributor, false, false, false, false);

        REQUIRE(event.getSource() == widget);
        REQUIRE(event.getDistributor() == distributor);
    }

    SECTION("null distributor")
    {
        fcn::Widget* widget = reinterpret_cast<fcn::Widget*>(0x1234);

        fcn::InputEvent event(widget, nullptr, false, false, false, false);

        REQUIRE(event.getDistributor() == nullptr);
    }
}

TEST_CASE("InputEvent modifier key checks", "[unit][inputevent]")
{
    SECTION("shift pressed")
    {
        fcn::InputEvent event(nullptr, nullptr, true, false, false, false);
        REQUIRE(event.isShiftPressed() == true);
    }

    SECTION("control pressed")
    {
        fcn::InputEvent event(nullptr, nullptr, false, true, false, false);
        REQUIRE(event.isControlPressed() == true);
    }

    SECTION("alt pressed")
    {
        fcn::InputEvent event(nullptr, nullptr, false, false, true, false);
        REQUIRE(event.isAltPressed() == true);
    }

    SECTION("meta pressed")
    {
        fcn::InputEvent event(nullptr, nullptr, false, false, false, true);
        REQUIRE(event.isMetaPressed() == true);
    }

    SECTION("all modifiers")
    {
        fcn::InputEvent event(nullptr, nullptr, true, true, true, true);
        REQUIRE(event.isShiftPressed() == true);
        REQUIRE(event.isControlPressed() == true);
        REQUIRE(event.isAltPressed() == true);
        REQUIRE(event.isMetaPressed() == true);
    }
}

TEST_CASE("InputEvent consume functionality", "[unit][inputevent]")
{
    SECTION("default is not consumed")
    {
        fcn::InputEvent event(nullptr, nullptr, false, false, false, false);
        REQUIRE(event.isConsumed() == false);
    }

    SECTION("consume marks event as consumed")
    {
        fcn::InputEvent event(nullptr, nullptr, false, false, false, false);
        event.consume();
        REQUIRE(event.isConsumed() == true);
    }

    SECTION("isConsumed returns true after consume")
    {
        fcn::InputEvent event(nullptr, nullptr, false, false, false, false);
        event.consume();
        REQUIRE(event.isConsumed() == true);
    }
}

TEST_CASE("InputEvent getDistributor returns correct widget", "[unit][inputevent]")
{
    SECTION("null distributor")
    {
        fcn::InputEvent event(nullptr, nullptr, false, false, false, false);
        REQUIRE(event.getDistributor() == nullptr);
    }

    SECTION("valid distributor")
    {
        fcn::Widget* distributor = reinterpret_cast<fcn::Widget*>(0x9999);
        fcn::InputEvent event(nullptr, distributor, false, false, false, false);
        REQUIRE(event.getDistributor() == distributor);
    }

    SECTION("different distributor from source")
    {
        fcn::Widget* source      = reinterpret_cast<fcn::Widget*>(0x1111);
        fcn::Widget* distributor = reinterpret_cast<fcn::Widget*>(0x2222);

        fcn::InputEvent event(source, distributor, false, false, false, false);

        REQUIRE(event.getSource() == source);
        REQUIRE(event.getDistributor() == distributor);
    }
}

TEST_CASE("InputEvent edge cases", "[unit][inputevent]")
{
    SECTION("source and distributor are same")
    {
        fcn::Widget* widget = reinterpret_cast<fcn::Widget*>(0x1234);
        fcn::InputEvent event(widget, widget, false, false, false, false);

        REQUIRE(event.getSource() == widget);
        REQUIRE(event.getDistributor() == widget);
    }

    SECTION("consume can be called multiple times")
    {
        fcn::InputEvent event(nullptr, nullptr, false, false, false, false);
        event.consume();
        event.consume();
        event.consume();
        REQUIRE(event.isConsumed() == true);
    }

    SECTION("all false modifiers works")
    {
        fcn::InputEvent event(nullptr, nullptr, false, false, false, false);
        REQUIRE(event.isShiftPressed() == false);
        REQUIRE(event.isControlPressed() == false);
        REQUIRE(event.isAltPressed() == false);
        REQUIRE(event.isMetaPressed() == false);
    }
}
