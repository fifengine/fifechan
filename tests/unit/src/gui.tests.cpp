// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2026 Fifengine contributors

// Standard library includes
#include <memory>
#include <utility>

// Third-party library includes
#include <catch2/catch_test_macros.hpp>

// Project headers
#include "fifechan/events/keyevent.hpp"
#include "fifechan/events/mouseevent.hpp"
#include "fifechan/focushandler.hpp"
#include "fifechan/gui.hpp"
#include "fifechan/listeners/actionlistener.hpp"
#include "fifechan/listeners/keylistener.hpp"
#include "fifechan/listeners/mouselistener.hpp"
#include "fifechan/mouseinput.hpp"
#include "fifechan/widgets/button.hpp"
#include "fifechan/widgets/container.hpp"
#include "fifechan/widgets/label.hpp"
#include "fifechan/widgets/window.hpp"

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
    auto container  = std::make_unique<Container>();
    auto* const ptr = container.get();
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
    auto container  = std::make_unique<Container>();
    auto* const ptr = container.get();
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

namespace
{
    class TestGui : public Gui
    {
        public:
            using Gui::handleMouseMoved;
            using Gui::handleMousePressed;
            using Gui::handleMouseReleased;
    };

    class RecordingMouseListener : public fcn::MouseListener
    {
        public:
            int entered = 0;

            void mouseEntered(fcn::MouseEvent&) override
            {
                ++entered;
            }
    };

    class RecordingActionListener : public fcn::ActionListener
    {
        public:
            int actions = 0;

            void action(fcn::ActionEvent const &) override
            {
                ++actions;
            }
    };
} // namespace

TEST_CASE("Keyboard modal focus still delivers Entered events and clicks", "[unit][gui]")
{
    TestGui gui;
    Container root;
    root.setSize(400, 300);
    fcn::Window window;
    window.setPosition(20, 20);
    window.setSize(300, 200);

    fcn::Button button("Ok");
    button.setPosition(10, 10);
    button.setSize(100, 30);
    button.setActionEventId("ok");
    RecordingMouseListener mouseListener;
    RecordingActionListener actionListener;
    button.addMouseListener(&mouseListener);
    button.addActionListener(&actionListener);
    window.add(&button);
    root.add(&window);
    gui.setTop(&root);

    int x = 0;
    int y = 0;
    button.getAbsolutePosition(x, y);
    int const cx = x + 50;
    int const cy = y + 15;

    auto const moveToButton = [&]() {
        gui.handleMouseMoved(fcn::MouseInput(fcn::MouseInput::Button::Empty, fcn::MouseInput::Type::Moved, cx, cy, 0));
    };
    auto const clickButton = [&]() {
        moveToButton();
        gui.handleMousePressed(
            fcn::MouseInput(fcn::MouseInput::Button::Left, fcn::MouseInput::Type::Pressed, cx, cy, 1));
        gui.handleMouseReleased(
            fcn::MouseInput(fcn::MouseInput::Button::Left, fcn::MouseInput::Type::Released, cx, cy, 2));
    };

    moveToButton();
    REQUIRE(mouseListener.entered >= 1);

    gui.getFocusHandler()->pushModal(&window, nullptr);
    REQUIRE(button.isModalFocused());
    REQUIRE_FALSE(button.isUnderMouseModal());

    // Leave and re-enter the button while keyboard modal focus is active.
    gui.handleMouseMoved(fcn::MouseInput(fcn::MouseInput::Button::Empty, fcn::MouseInput::Type::Moved, 0, 0, 0));
    int const enteredBefore = mouseListener.entered;
    moveToButton();
    REQUIRE(mouseListener.entered > enteredBefore);

    int const actionsBefore = actionListener.actions;
    clickButton();
    REQUIRE(actionListener.actions > actionsBefore);
}
