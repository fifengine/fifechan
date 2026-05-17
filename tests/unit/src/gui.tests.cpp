// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2026 Fifengine contributors

// Standard library includes
#include <memory>
#include <utility>

// Third-party library includes
#include <catch2/catch_test_macros.hpp>

// Project headers
#include "fifechan/events/keyevent.hpp"
#include "fifechan/gui.hpp"
#include "fifechan/listeners/keylistener.hpp"
#include "fifechan/widgets/container.hpp"
#include "fifechan/widgets/label.hpp"

using fcn::Container;
using fcn::Gui;
using fcn::KeyEvent;
using fcn::KeyListener;
using fcn::Label;

TEST_CASE("Gui default constructor", "[unit][gui]")
{
    Gui gui;
    REQUIRE(gui.getTop() == nullptr);
    REQUIRE(gui.getGraphics() == nullptr);
    REQUIRE(gui.getInput() == nullptr);
    REQUIRE(gui.getFocusHandler() != nullptr);
    REQUIRE(gui.isTabbingEnabled());
}

TEST_CASE("Gui setTop and getTop", "[unit][gui]")
{
    Gui gui;
    Container container;
    gui.setTop(&container);
    REQUIRE(gui.getTop() == &container);
}

TEST_CASE("Gui setTop nullptr clears", "[unit][gui]")
{
    Gui gui;
    Container container;
    gui.setTop(&container);
    REQUIRE(gui.getTop() == &container);

    gui.setTop(nullptr);
    REQUIRE(gui.getTop() == nullptr);
}

TEST_CASE("Gui setTop with unique_ptr", "[unit][gui]")
{
    Gui gui;
    auto container = std::make_unique<Container>();
    auto const ptr = container.get();
    gui.setTop(std::move(container));
    REQUIRE(gui.getTop() == ptr);
}

TEST_CASE("Gui setRoot alias", "[unit][gui]")
{
    Gui gui;
    Container container;
    gui.setRoot(&container);
    REQUIRE(gui.getTop() == &container);
}

TEST_CASE("Gui setRoot with unique_ptr", "[unit][gui]")
{
    Gui gui;
    auto container = std::make_unique<Container>();
    auto const ptr = container.get();
    gui.setRoot(std::move(container));
    REQUIRE(gui.getTop() == ptr);
}

TEST_CASE("Gui setTabbingEnabled and isTabbingEnabled", "[unit][gui]")
{
    Gui gui;
    REQUIRE(gui.isTabbingEnabled());

    gui.setTabbingEnabled(false);
    REQUIRE_FALSE(gui.isTabbingEnabled());

    gui.setTabbingEnabled(true);
    REQUIRE(gui.isTabbingEnabled());
}

TEST_CASE("Gui focusNone does not crash", "[unit][gui]")
{
    Gui gui;
    gui.focusNone();
    REQUIRE(true);
}

TEST_CASE("Gui focusNone does not crash with top widget", "[unit][gui]")
{
    Gui gui;
    Container container;
    gui.setTop(&container);
    gui.focusNone();
    REQUIRE(gui.getFocusHandler() != nullptr);
}

TEST_CASE("Gui getFocusHandler returns non-null", "[unit][gui]")
{
    Gui gui;
    REQUIRE(gui.getFocusHandler() != nullptr);
}

TEST_CASE("Gui addGlobalKeyListener and removeGlobalKeyListener", "[unit][gui]")
{
    Gui gui;
    class TestKeyListener : public KeyListener
    {
        public:
            void keyPressed(KeyEvent& /*event*/) override
            {
            }
            void keyReleased(KeyEvent& /*event*/) override
            {
            }
    };
    TestKeyListener listener;

    gui.addGlobalKeyListener(&listener);
    gui.removeGlobalKeyListener(&listener);
    REQUIRE(true);
}

TEST_CASE("Gui addHiddenWidget and addShownWidget", "[unit][gui]")
{
    Gui gui;
    Label label;
    gui.addHiddenWidget(&label);
    gui.addShownWidget(&label);
    REQUIRE(true);
}

TEST_CASE("Gui widgetDied does not crash", "[unit][gui]")
{
    Gui gui;
    Label label;
    gui.widgetDied(&label);
    REQUIRE(true);
}

TEST_CASE("Gui widgetDied with registered widgets", "[unit][gui]")
{
    Gui gui;
    Label label1;
    Label label2;

    gui.addHiddenWidget(&label1);
    gui.addHiddenWidget(&label2);

    gui.widgetDied(&label1);
    // label1 removed from hidden list, no crash
    gui.widgetDied(&label2);
    REQUIRE(true);
}
