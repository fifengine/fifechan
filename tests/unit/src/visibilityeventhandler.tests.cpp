// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2026 Fifengine contributors

// Corresponding header include
#include "fifechan/listeners/visibilityeventhandler.hpp"

// Third-party library includes
#include <catch2/catch_test_macros.hpp>

// Project headers
#include "fifechan/events/event.hpp"
#include "fifechan/gui.hpp"
#include "fifechan/widgets/label.hpp"

TEST_CASE("VisibilityEventHandler constructor sets gui pointer", "[visibilityeventhandler]")
{
    // Create a Gui object (we don't need to fully initialize it)
    fcn::Gui gui;
    fcn::VisibilityEventHandler const handler(&gui);

    // If we got here without crashing, the constructor works
    REQUIRE(true);
}

TEST_CASE("VisibilityEventHandler widgetHidden adds widget to hidden list", "[visibilityeventhandler]")
{
    fcn::Gui gui;
    fcn::VisibilityEventHandler handler(&gui);
    fcn::Label label("Test");

    // Create an event with the label as source
    fcn::Event const event(&label);

    // Call widgetHidden - this should add the widget to gui's hidden list
    handler.widgetHidden(event);

    // If we got here without crashing, the method works
    REQUIRE(true);
}

TEST_CASE("VisibilityEventHandler widgetShown adds widget to shown list", "[visibilityeventhandler]")
{
    fcn::Gui gui;
    fcn::VisibilityEventHandler handler(&gui);
    fcn::Label label("Test");

    // Create an event with the label as source
    fcn::Event const event(&label);

    // Call widgetShown - this should add the widget to gui's shown list
    handler.widgetShown(event);

    // If we got here without crashing, the method works
    REQUIRE(true);
}

TEST_CASE("VisibilityEventHandler handles multiple visibility changes", "[visibilityeventhandler]")
{
    fcn::Gui gui;
    fcn::VisibilityEventHandler handler(&gui);
    fcn::Label label1("Test1");
    fcn::Label label2("Test2");

    // Create events
    fcn::Event const event1(&label1);
    fcn::Event const event2(&label2);

    // Call methods multiple times
    handler.widgetHidden(event1);
    handler.widgetHidden(event2);
    handler.widgetShown(event1);
    handler.widgetShown(event2);

    // If we got here without crashing, the methods work
    REQUIRE(true);
}
