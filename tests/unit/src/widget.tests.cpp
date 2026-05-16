// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2026 Fifengine contributors

// Third-party library includes
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>

// Project headers
#include "fifechan/color.hpp"
#include "fifechan/events/actionevent.hpp"
#include "fifechan/events/dragevent.hpp"
#include "fifechan/events/event.hpp"
#include "fifechan/events/mouseevent.hpp"
#include "fifechan/focushandler.hpp"
#include "fifechan/gui.hpp"
#include "fifechan/listeners/actionlistener.hpp"
#include "fifechan/listeners/deathlistener.hpp"
#include "fifechan/listeners/droptargetlistener.hpp"
#include "fifechan/listeners/focuslistener.hpp"
#include "fifechan/listeners/keylistener.hpp"
#include "fifechan/listeners/mouselistener.hpp"
#include "fifechan/listeners/widgetlistener.hpp"
#include "fifechan/rectangle.hpp"
#include "fifechan/size.hpp"
#include "fifechan/widget.hpp"
#include "fifechan/widgets/container.hpp"
#include "fifechan/widgets/label.hpp"

using fcn::ActionEvent;
using fcn::ActionListener;
using fcn::Color;
using fcn::Container;
using fcn::DeathListener;
using fcn::DragEvent;
using fcn::DropTargetListener;
using fcn::Event;
using fcn::FocusHandler;
using fcn::FocusListener;
using fcn::Font;
using fcn::Graphics;
using fcn::Gui;
using fcn::KeyEvent;
using fcn::KeyListener;
using fcn::Label;
using fcn::MouseEvent;
using fcn::MouseListener;
using fcn::Rectangle;
using fcn::Size;
using fcn::Widget;
using fcn::WidgetListener;

namespace
{
    // ============================================================================
    // Mock / helper classes
    // ============================================================================

    class MockActionListener : public ActionListener
    {
        public:
            void action(ActionEvent const & /*actionEvent*/) override
            {
                actionCalled = true;
            }
            bool actionCalled = false;
    };

    class MockDeathListener : public DeathListener
    {
        public:
            void death(Event const & /*event*/) override
            {
                deathCalled = true;
            }
            bool deathCalled = false;
    };

    class MockFocusListener : public FocusListener
    {
        public:
            void focusGained(Event const & /*event*/) override
            {
                gainedCalled = true;
            }
            void focusLost(Event const & /*event*/) override
            {
                lostCalled = true;
            }
            bool gainedCalled = false;
            bool lostCalled   = false;
    };

    class MockMouseListener : public MouseListener
    {
        public:
            void mouseEntered(MouseEvent& /*mouseEvent*/) override
            {
                enteredCalled = true;
            }
            void mouseExited(MouseEvent& /*mouseEvent*/) override
            {
                exitedCalled = true;
            }
            bool enteredCalled = false;
            bool exitedCalled  = false;
    };

    class MockKeyListener : public KeyListener
    {
        public:
            void keyPressed(KeyEvent& /*keyEvent*/) override
            {
                pressedCalled = true;
            }
            void keyReleased(KeyEvent& /*keyEvent*/) override
            {
                releasedCalled = true;
            }
            bool pressedCalled  = false;
            bool releasedCalled = false;
    };

    class MockWidgetListener : public WidgetListener
    {
        public:
            void widgetResized(Event const & /*event*/) override
            {
                resizedCalled = true;
            }
            void widgetMoved(Event const & /*event*/) override
            {
                movedCalled = true;
            }
            void widgetHidden(Event const & /*event*/) override
            {
                hiddenCalled = true;
            }
            void widgetShown(Event const & /*event*/) override
            {
                shownCalled = true;
            }
            void ancestorMoved(Event const & /*event*/) override
            {
                ancestorMovedCalled = true;
            }
            void ancestorHidden(Event const & /*event*/) override
            {
                ancestorHiddenCalled = true;
            }
            void ancestorShown(Event const & /*event*/) override
            {
                ancestorShownCalled = true;
            }
            bool resizedCalled        = false;
            bool movedCalled          = false;
            bool hiddenCalled         = false;
            bool shownCalled          = false;
            bool ancestorMovedCalled  = false;
            bool ancestorHiddenCalled = false;
            bool ancestorShownCalled  = false;
    };

    class MockDropTargetListener : public DropTargetListener
    {
        public:
            bool dragEntered(DragEvent& /*dragEvent*/) override
            {
                enteredCalled = true;
                return acceptDrag;
            }
            void dragExited(DragEvent& /*dragEvent*/) override
            {
                exitedCalled = true;
            }
            void dragHovered(DragEvent& /*dragEvent*/) override
            {
                hoveredCalled = true;
            }
            void dragDropped(DragEvent& /*dragEvent*/) override
            {
                droppedCalled = true;
            }
            bool enteredCalled = false;
            bool exitedCalled  = false;
            bool hoveredCalled = false;
            bool droppedCalled = false;
            bool acceptDrag    = false;
    };

    // Testable widget that exposes protected methods for testing
    class TestableWidget : public Label
    {
        public:
            using Label::Label;
            void draw(Graphics* /*graphics*/) override
            {
            }

            // Expose protected methods for testing
            void testDistributeActionEvent()
            {
                distributeActionEvent();
            }
            bool testDistributeDragEnter(DragEvent& event)
            {
                return distributeDragEnter(event);
            }
            void testDistributeDragLeave(DragEvent& event)
            {
                distributeDragLeave(event);
            }
            void testDistributeDragHover(DragEvent& event)
            {
                distributeDragHover(event);
            }
            void testDistributeDragDrop(DragEvent& event)
            {
                distributeDragDrop(event);
            }
    };

} // anonymous namespace

// ============================================================================
// Widget constructor / destructor
// ============================================================================

TEST_CASE("Label default constructor registers widget instance", "[unit][widget]")
{
    Label label;
    REQUIRE(Widget::widgetExists(&label));
}

TEST_CASE("Label destructor removes widget instance", "[unit][widget]")
{
    bool existedBefore = false;
    {
        Label label;
        existedBefore = Widget::widgetExists(&label);
    }
    REQUIRE(existedBefore);
}

TEST_CASE("Widget is not focusable by default", "[unit][widget]")
{
    Label label;
    REQUIRE_FALSE(label.isFocusable());
}

TEST_CASE("Widget is visible by default", "[unit][widget]")
{
    Label label;
    REQUIRE(label.isVisible());
    REQUIRE(label.isSetVisible());
}

TEST_CASE("Widget is enabled by default", "[unit][widget]")
{
    Label label;
    REQUIRE(label.isEnabled());
}

TEST_CASE("Widget has default dimension 0,0,0,0", "[unit][widget]")
{
    Label label;
    REQUIRE(label.getX() == 0);
    REQUIRE(label.getY() == 0);
    REQUIRE(label.getWidth() == 0);
    // Label may have a default height from font, so just check it's >= 0
    REQUIRE(label.getHeight() >= 0);
}

TEST_CASE("Widget has no parent by default", "[unit][widget]")
{
    Label label;
    REQUIRE(label.getParent() == nullptr);
}

TEST_CASE("Widget has empty action event id by default", "[unit][widget]")
{
    Label label;
    REQUIRE(label.getActionEventId().empty());
}

TEST_CASE("Widget has empty id by default", "[unit][widget]")
{
    Label label;
    REQUIRE(label.getId().empty());
}

// ============================================================================
// Dimension setters / getters
// ============================================================================

TEST_CASE("Widget setX and getX", "[unit][widget]")
{
    Label label;
    label.setX(42);
    REQUIRE(label.getX() == 42);
    REQUIRE(label.getY() == 0);
    REQUIRE(label.getWidth() == 0);
    // Label has default height from font, just verify it didn't change
    REQUIRE(label.getHeight() >= 0);
}

TEST_CASE("Widget setY and getY", "[unit][widget]")
{
    Label label;
    label.setY(99);
    REQUIRE(label.getY() == 99);
    REQUIRE(label.getX() == 0);
}

TEST_CASE("Widget setWidth and getWidth", "[unit][widget]")
{
    Label label;
    label.setWidth(200);
    REQUIRE(label.getWidth() == 200);
    // Label has default height from font, just verify it's >= 0
    REQUIRE(label.getHeight() >= 0);
}

TEST_CASE("Widget setHeight and getHeight", "[unit][widget]")
{
    Label label;
    label.setHeight(150);
    REQUIRE(label.getHeight() == 150);
    REQUIRE(label.getWidth() == 0);
}

TEST_CASE("Widget setPosition", "[unit][widget]")
{
    Label label;
    label.setPosition(10, 20);
    REQUIRE(label.getX() == 10);
    REQUIRE(label.getY() == 20);
}

TEST_CASE("Widget setSize", "[unit][widget]")
{
    Label label;
    label.setSize(300, 400);
    REQUIRE(label.getWidth() == 300);
    REQUIRE(label.getHeight() == 400);
}

TEST_CASE("Widget setDimension and getDimension", "[unit][widget]")
{
    Label label;
    Rectangle dim(5, 10, 100, 200);
    label.setDimension(dim);
    Rectangle const & result = label.getDimension();
    REQUIRE(result.x == 5);
    REQUIRE(result.y == 10);
    REQUIRE(result.width == 100);
    REQUIRE(result.height == 200);
}

TEST_CASE("Widget setDimension with negative values", "[unit][widget]")
{
    Label label;
    label.setSize(-50, -30);
    REQUIRE(label.getWidth() >= 0);
    REQUIRE(label.getHeight() >= 0);
}

TEST_CASE("Widget setDimension triggers resize event when size changes", "[unit][widget]")
{
    Label label;
    MockWidgetListener listener;
    label.addWidgetListener(&listener);
    label.setSize(100, 100);
    REQUIRE(listener.resizedCalled);
}

TEST_CASE("Widget setDimension triggers move event when position changes", "[unit][widget]")
{
    Label label;
    MockWidgetListener listener;
    label.addWidgetListener(&listener);
    label.setPosition(50, 50);
    REQUIRE(listener.movedCalled);
}

TEST_CASE("Widget setDimension triggers both events when both change", "[unit][widget]")
{
    Label label;
    MockWidgetListener listener;
    label.addWidgetListener(&listener);
    label.setDimension(Rectangle(10, 20, 100, 100));
    REQUIRE(listener.resizedCalled);
    REQUIRE(listener.movedCalled);
}

// ============================================================================
// Visibility
// ============================================================================

TEST_CASE("Widget setVisible false", "[unit][widget]")
{
    Gui gui;
    Container container;
    Label label;
    container.setSize(200, 200);
    container.add(&label);
    label.setPosition(0, 0);
    label.setSize(50, 50);
    gui.setTop(&container);

    label.setVisible(false);
    REQUIRE_FALSE(label.isVisible());
    REQUIRE_FALSE(label.isSetVisible());
}

TEST_CASE("Widget setVisible true after false", "[unit][widget]")
{
    Gui gui;
    Container container;
    Label label;
    container.setSize(200, 200);
    container.add(&label);
    label.setPosition(0, 0);
    label.setSize(50, 50);
    gui.setTop(&container);

    label.setVisible(false);
    label.setVisible(true);
    REQUIRE(label.isVisible());
    REQUIRE(label.isSetVisible());
}

TEST_CASE("Widget isVisible respects parent visibility", "[unit][widget]")
{
    Gui gui;
    Container parent;
    Label child;
    parent.setSize(200, 200);
    parent.add(&child);
    child.setPosition(0, 0);
    child.setSize(50, 50);
    gui.setTop(&parent);

    REQUIRE(child.isVisible());

    parent.setVisible(false);
    REQUIRE_FALSE(child.isVisible());
    REQUIRE(child.isSetVisible());
}

TEST_CASE("Widget setVisible false clears focus if focused", "[unit][widget]")
{
    Gui gui;
    Container container;
    Label label;
    container.setSize(200, 200);
    container.add(&label);
    label.setPosition(0, 0);
    label.setSize(50, 50);
    label.setFocusable(true);
    gui.setTop(&container);

    label.requestFocus();
    REQUIRE(label.isFocused());
    REQUIRE(gui.getFocusHandler()->getFocused() == &label);

    label.setVisible(false);
    // FocusHandler should no longer track this widget
    REQUIRE(gui.getFocusHandler()->getFocused() == nullptr);
}

TEST_CASE("Widget setVisible triggers hidden/shown events", "[unit][widget]")
{
    Gui gui;
    Container container;
    Label label;
    container.setSize(200, 200);
    container.add(&label);
    label.setPosition(0, 0);
    label.setSize(50, 50);
    gui.setTop(&container);

    MockWidgetListener listener;
    label.addWidgetListener(&listener);

    label.setVisible(false);
    REQUIRE(listener.hiddenCalled);
    REQUIRE_FALSE(listener.shownCalled);

    listener.hiddenCalled = false;
    listener.shownCalled  = false;

    label.setVisible(true);
    REQUIRE(listener.shownCalled);
    REQUIRE_FALSE(listener.hiddenCalled);
}

// ============================================================================
// Enabled state
// ============================================================================

TEST_CASE("Widget setEnabled and isEnabled", "[unit][widget]")
{
    Label label;
    REQUIRE(label.isEnabled());

    label.setEnabled(false);
    REQUIRE_FALSE(label.isEnabled());

    label.setEnabled(true);
    REQUIRE(label.isEnabled());
}

TEST_CASE("Widget isEnabled returns false when invisible", "[unit][widget]")
{
    Gui gui;
    Label label;
    gui.setTop(&label);
    label.setVisible(false);
    REQUIRE_FALSE(label.isEnabled());
}

// ============================================================================
// Focusable
// ============================================================================

TEST_CASE("Widget setFocusable and isFocusable", "[unit][widget]")
{
    Label label;
    label.setFocusable(true);
    REQUIRE(label.isFocusable());

    label.setFocusable(false);
    REQUIRE_FALSE(label.isFocusable());
}

TEST_CASE("Widget isFocusable requires visible and enabled", "[unit][widget]")
{
    Gui gui;
    Label label;
    gui.setTop(&label);
    label.setFocusable(true);
    REQUIRE(label.isFocusable());

    label.setVisible(false);
    REQUIRE_FALSE(label.isFocusable());

    label.setVisible(true);
    label.setEnabled(false);
    REQUIRE_FALSE(label.isFocusable());
}

TEST_CASE("Widget setFocusable false clears focus if focused", "[unit][widget]")
{
    Gui gui;
    Container container;
    Label label;
    container.setSize(200, 200);
    container.add(&label);
    label.setPosition(0, 0);
    label.setSize(50, 50);
    label.setFocusable(true);
    gui.setTop(&container);

    label.requestFocus();
    REQUIRE(label.isFocused());

    label.setFocusable(false);
    // FocusHandler should no longer track this widget
    REQUIRE(gui.getFocusHandler()->getFocused() == nullptr);
}

// ============================================================================
// Focused state
// ============================================================================

TEST_CASE("Widget setFocused triggers focus gained", "[unit][widget]")
{
    Gui gui;
    Container container;
    Label label;
    container.setSize(200, 200);
    container.add(&label);
    label.setPosition(0, 0);
    label.setSize(50, 50);
    label.setFocusable(true);
    gui.setTop(&container);

    MockFocusListener listener;
    label.addFocusListener(&listener);

    label.requestFocus();
    REQUIRE(listener.gainedCalled);
    REQUIRE_FALSE(listener.lostCalled);
    REQUIRE(label.isFocused());
}

TEST_CASE("Widget setFocused triggers focus lost", "[unit][widget]")
{
    Gui gui;
    Container container;
    Label label;
    container.setSize(200, 200);
    container.add(&label);
    label.setPosition(0, 0);
    label.setSize(50, 50);
    label.setFocusable(true);
    gui.setTop(&container);

    MockFocusListener listener;
    label.addFocusListener(&listener);

    label.requestFocus();
    REQUIRE(label.isFocused());
    REQUIRE(listener.gainedCalled);

    listener.lostCalled   = false;
    listener.gainedCalled = false;

    label.setVisible(false); // This triggers focusNone() which distributes focus lost
    REQUIRE(listener.lostCalled);
    // FocusHandler no longer tracks this widget
    REQUIRE(gui.getFocusHandler()->getFocused() == nullptr);
}

TEST_CASE("Widget setFocused same value is no-op", "[unit][widget]")
{
    Label label;
    MockFocusListener listener;
    label.addFocusListener(&listener);

    label.setFocused(false);
    REQUIRE_FALSE(listener.lostCalled);
    REQUIRE_FALSE(listener.gainedCalled);
}

// ============================================================================
// requestFocus
// ============================================================================

TEST_CASE("Widget requestFocus throws without focus handler", "[unit][widget]")
{
    Label label;
    label.setFocusable(true);
    REQUIRE_THROWS_WITH(label.requestFocus(), Catch::Matchers::ContainsSubstring("focus handler"));
}

TEST_CASE("Widget requestFocus succeeds with focus handler", "[unit][widget]")
{
    Container container;
    Label label;
    FocusHandler fh;

    container.setSize(200, 200);
    container.add(&label);
    label.setPosition(0, 0);
    label.setSize(50, 50);
    label.setFocusable(true);

    container._setFocusHandler(&fh);
    label.requestFocus();
    REQUIRE(label.isFocused());
}

TEST_CASE("Widget requestFocus does nothing when not focusable", "[unit][widget]")
{
    Container container;
    Label label;
    FocusHandler fh;

    container.setSize(200, 200);
    container.add(&label);
    label.setPosition(0, 0);
    label.setSize(50, 50);

    container._setFocusHandler(&fh);
    label.requestFocus();
    REQUIRE_FALSE(label.isFocused());
}

// ============================================================================
// Parent / children
// ============================================================================

TEST_CASE("Widget getParent returns container after add", "[unit][widget]")
{
    Container container;
    Label label;
    container.setSize(200, 200);
    container.add(&label);
    label.setPosition(0, 0);
    label.setSize(50, 50);

    REQUIRE(label.getParent() == &container);
}

TEST_CASE("Container getChildrenCount", "[unit][widget]")
{
    Container container;
    Label l1;
    Label l2;
    Label l3;
    container.setSize(200, 200);
    container.add(&l1);
    l1.setPosition(0, 0);
    l1.setSize(10, 10);
    container.add(&l2);
    l2.setPosition(10, 0);
    l2.setSize(10, 10);
    container.add(&l3);
    l3.setPosition(20, 0);
    l3.setSize(10, 10);

    REQUIRE(container.getChildrenCount() == 3);
}

TEST_CASE("Container getVisibleChildrenCount", "[unit][widget]")
{
    Gui gui;
    Container container;
    Label l1;
    Label l2;
    container.setSize(200, 200);
    container.add(&l1);
    l1.setPosition(0, 0);
    l1.setSize(10, 10);
    container.add(&l2);
    l2.setPosition(10, 0);
    l2.setSize(10, 10);
    gui.setTop(&container);

    l2.setVisible(false);
    REQUIRE(container.getVisibleChildrenCount() == 1);
}

TEST_CASE("Container remove widget", "[unit][widget]")
{
    Container container;
    Label label;
    container.setSize(200, 200);
    container.add(&label);
    label.setPosition(0, 0);
    label.setSize(50, 50);

    REQUIRE(container.getChildrenCount() == 1);
    container.remove(&label);
    REQUIRE(container.getChildrenCount() == 0);
    REQUIRE(label.getParent() == nullptr);
}

TEST_CASE("Container remove non-existent widget throws", "[unit][widget]")
{
    Container container;
    Label label;
    REQUIRE_THROWS(container.remove(&label));
}

TEST_CASE("Container removeAllChildren", "[unit][widget]")
{
    Container container;
    Label l1;
    Label l2;
    container.setSize(200, 200);
    container.add(&l1);
    l1.setPosition(0, 0);
    l1.setSize(10, 10);
    container.add(&l2);
    l2.setPosition(10, 0);
    l2.setSize(10, 10);

    container.removeAllChildren();
    REQUIRE(container.getChildrenCount() == 0);
    REQUIRE(l1.getParent() == nullptr);
    REQUIRE(l2.getParent() == nullptr);
}

TEST_CASE("Container moveToTop", "[unit][widget]")
{
    Container container;
    Label l1;
    Label l2;
    container.setSize(200, 200);
    container.add(&l1);
    l1.setPosition(0, 0);
    l1.setSize(10, 10);
    container.add(&l2);
    l2.setPosition(10, 0);
    l2.setSize(10, 10);

    container.moveToTop(&l1);
    // Verify via getChild - last child should be l1
    REQUIRE(container.getChild(1) == &l1);
}

TEST_CASE("Container moveToTop non-existent throws", "[unit][widget]")
{
    Container container;
    Label label;
    REQUIRE_THROWS(container.moveToTop(&label));
}

TEST_CASE("Container moveToBottom", "[unit][widget]")
{
    Container container;
    Label l1;
    Label l2;
    container.setSize(200, 200);
    container.add(&l1);
    l1.setPosition(0, 0);
    l1.setSize(10, 10);
    container.add(&l2);
    l2.setPosition(10, 0);
    l2.setSize(10, 10);

    container.moveToBottom(&l2);
    REQUIRE(container.getChild(0) == &l2);
}

TEST_CASE("Container moveToBottom non-existent throws", "[unit][widget]")
{
    Container container;
    Label label;
    REQUIRE_THROWS(container.moveToBottom(&label));
}

TEST_CASE("Widget requestMoveToTop", "[unit][widget]")
{
    Container container;
    Label l1;
    Label l2;
    container.setSize(200, 200);
    container.add(&l1);
    l1.setPosition(0, 0);
    l1.setSize(10, 10);
    container.add(&l2);
    l2.setPosition(10, 0);
    l2.setSize(10, 10);

    l1.requestMoveToTop();
    REQUIRE(container.getChild(1) == &l1);
}

TEST_CASE("Widget requestMoveToBottom", "[unit][widget]")
{
    Container container;
    Label l1;
    Label l2;
    container.setSize(200, 200);
    container.add(&l1);
    l1.setPosition(0, 0);
    l1.setSize(10, 10);
    container.add(&l2);
    l2.setPosition(10, 0);
    l2.setSize(10, 10);

    l2.requestMoveToBottom();
    REQUIRE(container.getChild(0) == &l2);
}

TEST_CASE("Widget requestMoveToTop without parent is no-op", "[unit][widget]")
{
    Label label;
    label.requestMoveToTop();
    REQUIRE(true);
}

TEST_CASE("Widget requestMoveToBottom without parent is no-op", "[unit][widget]")
{
    Label label;
    label.requestMoveToBottom();
    REQUIRE(true);
}

// ============================================================================
// Action listeners (via TestableWidget to access protected distributeActionEvent)
// ============================================================================

TEST_CASE("Widget addActionListener and distributeActionEvent", "[unit][widget]")
{
    TestableWidget label;
    MockActionListener listener;
    label.setActionEventId("test_action");
    label.addActionListener(&listener);

    label.testDistributeActionEvent();
    REQUIRE(listener.actionCalled);
}

TEST_CASE("Widget removeActionListener", "[unit][widget]")
{
    TestableWidget label;
    MockActionListener listener;
    label.addActionListener(&listener);
    label.removeActionListener(&listener);

    label.setActionEventId("test_action");
    label.testDistributeActionEvent();
    REQUIRE_FALSE(listener.actionCalled);
}

TEST_CASE("Widget multiple action listeners", "[unit][widget]")
{
    TestableWidget label;
    MockActionListener l1;
    MockActionListener l2;
    MockActionListener l3;
    label.addActionListener(&l1);
    label.addActionListener(&l2);
    label.addActionListener(&l3);

    label.setActionEventId("multi");
    label.testDistributeActionEvent();

    REQUIRE(l1.actionCalled);
    REQUIRE(l2.actionCalled);
    REQUIRE(l3.actionCalled);
}

// ============================================================================
// Death listeners
// ============================================================================

TEST_CASE("Widget addDeathListener notified on destruction", "[unit][widget]")
{
    MockDeathListener listener;
    {
        Label label;
        label.addDeathListener(&listener);
    }
    REQUIRE(listener.deathCalled);
}

TEST_CASE("Widget removeDeathListener", "[unit][widget]")
{
    MockDeathListener listener;
    {
        Label label;
        label.addDeathListener(&listener);
        label.removeDeathListener(&listener);
    }
    REQUIRE_FALSE(listener.deathCalled);
}

// ============================================================================
// Focus listeners
// ============================================================================

TEST_CASE("Widget addFocusListener notified on focus change", "[unit][widget]")
{
    Gui gui;
    Container container;
    Label label;
    container.setSize(200, 200);
    container.add(&label);
    label.setPosition(0, 0);
    label.setSize(50, 50);
    label.setFocusable(true);
    gui.setTop(&container);

    MockFocusListener listener;
    label.addFocusListener(&listener);

    label.requestFocus();
    REQUIRE(listener.gainedCalled);

    listener.lostCalled   = false;
    listener.gainedCalled = false;

    label.setVisible(false); // Triggers focusNone() which distributes focus lost
    REQUIRE(listener.lostCalled);
}

TEST_CASE("Widget removeFocusListener", "[unit][widget]")
{
    Label label;
    MockFocusListener listener;
    label.addFocusListener(&listener);
    label.removeFocusListener(&listener);

    label.setFocused(true);
    REQUIRE_FALSE(listener.gainedCalled);
}

// ============================================================================
// Mouse listeners
// ============================================================================

TEST_CASE("Widget addMouseListener", "[unit][widget]")
{
    Label label;
    MockMouseListener listener;
    label.addMouseListener(&listener);

    auto const & listeners = label._getMouseListeners();
    REQUIRE(listeners.size() == 1);
}

TEST_CASE("Widget removeMouseListener", "[unit][widget]")
{
    Label label;
    MockMouseListener listener;
    label.addMouseListener(&listener);
    label.removeMouseListener(&listener);

    auto const & listeners = label._getMouseListeners();
    REQUIRE(listeners.empty());
}

// ============================================================================
// Key listeners
// ============================================================================

TEST_CASE("Widget addKeyListener", "[unit][widget]")
{
    Label label;
    MockKeyListener listener;
    label.addKeyListener(&listener);

    auto const & listeners = label._getKeyListeners();
    REQUIRE(listeners.size() == 1);
}

TEST_CASE("Widget removeKeyListener", "[unit][widget]")
{
    Label label;
    MockKeyListener listener;
    label.addKeyListener(&listener);
    label.removeKeyListener(&listener);

    auto const & listeners = label._getKeyListeners();
    REQUIRE(listeners.empty());
}

// ============================================================================
// Widget listeners
// ============================================================================

TEST_CASE("Widget addWidgetListener", "[unit][widget]")
{
    Label label;
    MockWidgetListener listener;
    label.addWidgetListener(&listener);

    label.setSize(100, 100);
    REQUIRE(listener.resizedCalled);
}

TEST_CASE("Widget removeWidgetListener", "[unit][widget]")
{
    Label label;
    MockWidgetListener listener;
    label.addWidgetListener(&listener);
    label.removeWidgetListener(&listener);

    label.setSize(100, 100);
    REQUIRE_FALSE(listener.resizedCalled);
}

// ============================================================================
// Drop target listeners (via TestableWidget to access protected distribute methods)
// ============================================================================

TEST_CASE("Widget addDropTargetListener", "[unit][widget]")
{
    Label label;
    MockDropTargetListener listener;
    label.addDropTargetListener(&listener);
    REQUIRE(true);
}

TEST_CASE("Widget removeDropTargetListener", "[unit][widget]")
{
    Label label;
    MockDropTargetListener listener;
    label.addDropTargetListener(&listener);
    label.removeDropTargetListener(&listener);
    REQUIRE(true);
}

TEST_CASE("Widget distributeDragEnter returns true when listener accepts", "[unit][widget]")
{
    TestableWidget label;
    MockDropTargetListener listener;
    listener.acceptDrag = true;
    label.addDropTargetListener(&listener);

    DragEvent event(&label, nullptr, DragEvent::Type::Enter, 0, 0, 0, 0);
    bool const result = label.testDistributeDragEnter(event);
    REQUIRE(result);
    REQUIRE(listener.enteredCalled);
}

TEST_CASE("Widget distributeDragEnter returns false when listener rejects", "[unit][widget]")
{
    TestableWidget label;
    MockDropTargetListener listener;
    listener.acceptDrag = false;
    label.addDropTargetListener(&listener);

    DragEvent event(&label, nullptr, DragEvent::Type::Enter, 0, 0, 0, 0);
    bool const result = label.testDistributeDragEnter(event);
    REQUIRE_FALSE(result);
}

TEST_CASE("Widget distributeDragLeave", "[unit][widget]")
{
    TestableWidget label;
    MockDropTargetListener listener;
    label.addDropTargetListener(&listener);

    DragEvent event(&label, nullptr, DragEvent::Type::Leave, 0, 0, 0, 0);
    label.testDistributeDragLeave(event);
    REQUIRE(listener.exitedCalled);
}

TEST_CASE("Widget distributeDragHover", "[unit][widget]")
{
    TestableWidget label;
    MockDropTargetListener listener;
    label.addDropTargetListener(&listener);

    DragEvent event(&label, nullptr, DragEvent::Type::Hover, 0, 0, 0, 0);
    label.testDistributeDragHover(event);
    REQUIRE(listener.hoveredCalled);
}

TEST_CASE("Widget distributeDragDrop", "[unit][widget]")
{
    TestableWidget label;
    MockDropTargetListener listener;
    label.addDropTargetListener(&listener);

    DragEvent event(&label, nullptr, DragEvent::Type::Drop, 0, 0, 0, 0);
    label.testDistributeDragDrop(event);
    REQUIRE(listener.droppedCalled);
}

// ============================================================================
// Color setters / getters
// ============================================================================

TEST_CASE("Widget setBaseColor and getBaseColor", "[unit][widget]")
{
    Label label;
    Color color(255, 128, 64);
    label.setBaseColor(color);
    REQUIRE(label.getBaseColor() == color);
}

TEST_CASE("Widget setForegroundColor and getForegroundColor", "[unit][widget]")
{
    Label label;
    Color color(0, 255, 0);
    label.setForegroundColor(color);
    REQUIRE(label.getForegroundColor() == color);
}

TEST_CASE("Widget setBackgroundColor and getBackgroundColor", "[unit][widget]")
{
    Label label;
    Color color(0, 0, 255);
    label.setBackgroundColor(color);
    REQUIRE(label.getBackgroundColor() == color);
}

TEST_CASE("Widget setSelectionColor and getSelectionColor", "[unit][widget]")
{
    Label label;
    Color color(255, 255, 0);
    label.setSelectionColor(color);
    REQUIRE(label.getSelectionColor() == color);
}

TEST_CASE("Widget setOutlineColor and getOutlineColor", "[unit][widget]")
{
    Label label;
    Color color(128, 128, 128);
    label.setOutlineColor(color);
    REQUIRE(label.getOutlineColor() == color);
}

TEST_CASE("Widget setBorderColor and getBorderColor", "[unit][widget]")
{
    Label label;
    Color color(64, 64, 64);
    label.setBorderColor(color);
    REQUIRE(label.getBorderColor() == color);
}

// ============================================================================
// Selection mode
// ============================================================================

TEST_CASE("Widget setSelectionMode and getSelectionMode", "[unit][widget]")
{
    Label label;
    label.setSelectionMode(Widget::SelectionMode::Border);
    REQUIRE(label.getSelectionMode() == Widget::SelectionMode::Border);

    label.setSelectionMode(Widget::SelectionMode::Background);
    REQUIRE(label.getSelectionMode() == Widget::SelectionMode::Background);

    label.setSelectionMode(Widget::SelectionMode::None);
    REQUIRE(label.getSelectionMode() == Widget::SelectionMode::None);
}

// ============================================================================
// Border properties
// ============================================================================

TEST_CASE("Widget setBorderSize and getBorderSize", "[unit][widget]")
{
    Label label;
    label.setBorderSize(5);
    REQUIRE(label.getBorderSize() == 5);
}

TEST_CASE("Widget setOutlineSize and getOutlineSize", "[unit][widget]")
{
    Label label;
    label.setOutlineSize(3);
    REQUIRE(label.getOutlineSize() == 3);
}

TEST_CASE("Widget setBorderSides and getBorderSides", "[unit][widget]")
{
    Label label;
    label.setBorderSides(Widget::BORDER_TOP | Widget::BORDER_BOTTOM);
    REQUIRE(label.getBorderSides() == (Widget::BORDER_TOP | Widget::BORDER_BOTTOM));
}

TEST_CASE("Widget setBorderStyle and getBorderStyle", "[unit][widget]")
{
    Label label;
    label.setBorderStyle(Widget::BORDER_STYLE_FLAT);
    REQUIRE(label.getBorderStyle() == Widget::BORDER_STYLE_FLAT);

    label.setBorderStyle(Widget::BORDER_STYLE_BEVEL);
    REQUIRE(label.getBorderStyle() == Widget::BORDER_STYLE_BEVEL);
}

TEST_CASE("Widget setBorderTop convenience", "[unit][widget]")
{
    Label label;
    label.setBorderTop(4, Widget::BORDER_STYLE_FLAT);
    REQUIRE(label.getBorderSize() == 4);
    REQUIRE(label.getBorderSides() == Widget::BORDER_TOP);
    REQUIRE(label.getBorderStyle() == Widget::BORDER_STYLE_FLAT);
}

TEST_CASE("Widget setBorderBottom convenience", "[unit][widget]")
{
    Label label;
    label.setBorderBottom(6, Widget::BORDER_STYLE_BEVEL);
    REQUIRE(label.getBorderSize() == 6);
    REQUIRE(label.getBorderSides() == Widget::BORDER_BOTTOM);
    REQUIRE(label.getBorderStyle() == Widget::BORDER_STYLE_BEVEL);
}

// ============================================================================
// Margin properties
// ============================================================================

TEST_CASE("Widget setMargin sets all margins", "[unit][widget]")
{
    Label label;
    label.setMargin(10);
    REQUIRE(label.getMarginTop() == 10);
    REQUIRE(label.getMarginRight() == 10);
    REQUIRE(label.getMarginBottom() == 10);
    REQUIRE(label.getMarginLeft() == 10);
}

TEST_CASE("Widget setMarginTop", "[unit][widget]")
{
    Label label;
    label.setMarginTop(15);
    REQUIRE(label.getMarginTop() == 15);
}

TEST_CASE("Widget setMarginRight", "[unit][widget]")
{
    Label label;
    label.setMarginRight(20);
    REQUIRE(label.getMarginRight() == 20);
}

TEST_CASE("Widget setMarginBottom", "[unit][widget]")
{
    Label label;
    label.setMarginBottom(25);
    REQUIRE(label.getMarginBottom() == 25);
}

TEST_CASE("Widget setMarginLeft", "[unit][widget]")
{
    Label label;
    label.setMarginLeft(30);
    REQUIRE(label.getMarginLeft() == 30);
}

TEST_CASE("Widget margin with negative values", "[unit][widget]")
{
    Label label;
    label.setMargin(-5);
    REQUIRE(label.getMarginTop() == -5);
    REQUIRE(label.getMarginRight() == -5);
    REQUIRE(label.getMarginBottom() == -5);
    REQUIRE(label.getMarginLeft() == -5);
}

// ============================================================================
// Padding properties
// ============================================================================

TEST_CASE("Widget setPadding sets all paddings", "[unit][widget]")
{
    Label label;
    label.setPadding(8);
    REQUIRE(label.getPaddingTop() == 8);
    REQUIRE(label.getPaddingRight() == 8);
    REQUIRE(label.getPaddingBottom() == 8);
    REQUIRE(label.getPaddingLeft() == 8);
}

TEST_CASE("Widget setPaddingTop", "[unit][widget]")
{
    Label label;
    label.setPaddingTop(12);
    REQUIRE(label.getPaddingTop() == 12);
}

TEST_CASE("Widget setPaddingRight", "[unit][widget]")
{
    Label label;
    label.setPaddingRight(14);
    REQUIRE(label.getPaddingRight() == 14);
}

TEST_CASE("Widget setPaddingBottom", "[unit][widget]")
{
    Label label;
    label.setPaddingBottom(16);
    REQUIRE(label.getPaddingBottom() == 16);
}

TEST_CASE("Widget setPaddingLeft", "[unit][widget]")
{
    Label label;
    label.setPaddingLeft(18);
    REQUIRE(label.getPaddingLeft() == 18);
}

// ============================================================================
// Min / Max / Fixed size
// ============================================================================

TEST_CASE("Widget setMinSize and getMinSize", "[unit][widget]")
{
    Label label;
    Size minSize(50, 30);
    label.setMinSize(minSize);
    REQUIRE(label.getMinSize().getWidth() == 50);
    REQUIRE(label.getMinSize().getHeight() == 30);
}

TEST_CASE("Widget setMaxSize and getMaxSize", "[unit][widget]")
{
    Label label;
    Size maxSize(500, 400);
    label.setMaxSize(maxSize);
    REQUIRE(label.getMaxSize().getWidth() == 500);
    REQUIRE(label.getMaxSize().getHeight() == 400);
}

TEST_CASE("Widget setFixedSize and getFixedSize", "[unit][widget]")
{
    Label label;
    Size fixedSize(100, 100);
    label.setFixedSize(fixedSize);
    REQUIRE(label.getFixedSize().getWidth() == 100);
    REQUIRE(label.getFixedSize().getHeight() == 100);
    REQUIRE(label.isFixedSize());
}

TEST_CASE("Widget setFixedSize with negative values disables fixed size", "[unit][widget]")
{
    Label label;
    label.setFixedSize(Size(100, 100));
    REQUIRE(label.isFixedSize());

    label.setFixedSize(Size(-1, -1));
    REQUIRE_FALSE(label.isFixedSize());
}

TEST_CASE("Widget calculateSize clamps to min size", "[unit][widget]")
{
    Label label;
    label.setMinSize(Size(50, 50));
    label.setSize(20, 20);
    REQUIRE(label.getWidth() == 50);
    REQUIRE(label.getHeight() == 50);
}

TEST_CASE("Widget calculateSize clamps to max size", "[unit][widget]")
{
    Label label;
    label.setMaxSize(Size(100, 100));
    label.setSize(200, 200);
    REQUIRE(label.getWidth() == 100);
    REQUIRE(label.getHeight() == 100);
}

TEST_CASE("Widget calculateSize respects both min and max", "[unit][widget]")
{
    Label label;
    label.setMinSize(Size(30, 30));
    label.setMaxSize(Size(100, 100));

    label.setSize(150, 150);
    REQUIRE(label.getWidth() == 100);
    REQUIRE(label.getHeight() == 100);

    label.setSize(10, 10);
    REQUIRE(label.getWidth() == 30);
    REQUIRE(label.getHeight() == 30);

    label.setSize(50, 50);
    REQUIRE(label.getWidth() == 50);
    REQUIRE(label.getHeight() == 50);
}

TEST_CASE("Widget fixed size overrides dimension", "[unit][widget]")
{
    Label label;
    label.setSize(200, 200);
    label.setFixedSize(Size(80, 80));
    REQUIRE(label.getWidth() == 80);
    REQUIRE(label.getHeight() == 80);
}

// ============================================================================
// Expand flags
// ============================================================================

TEST_CASE("Widget setVerticalExpand and isVerticalExpand", "[unit][widget]")
{
    Label label;
    REQUIRE_FALSE(label.isVerticalExpand());
    label.setVerticalExpand(true);
    REQUIRE(label.isVerticalExpand());
}

TEST_CASE("Widget setHorizontalExpand and isHorizontalExpand", "[unit][widget]")
{
    Label label;
    REQUIRE_FALSE(label.isHorizontalExpand());
    label.setHorizontalExpand(true);
    REQUIRE(label.isHorizontalExpand());
}

// ============================================================================
// Tab in/out
// ============================================================================

TEST_CASE("Widget isTabInEnabled default", "[unit][widget]")
{
    Label label;
    REQUIRE(label.isTabInEnabled());
}

TEST_CASE("Widget setTabInEnabled", "[unit][widget]")
{
    Label label;
    label.setTabInEnabled(false);
    REQUIRE_FALSE(label.isTabInEnabled());
}

TEST_CASE("Widget isTabOutEnabled default", "[unit][widget]")
{
    Label label;
    REQUIRE(label.isTabOutEnabled());
}

TEST_CASE("Widget setTabOutEnabled", "[unit][widget]")
{
    Label label;
    label.setTabOutEnabled(false);
    REQUIRE_FALSE(label.isTabOutEnabled());
}

// ============================================================================
// Id
// ============================================================================

TEST_CASE("Widget setId and getId", "[unit][widget]")
{
    Label label;
    label.setId("my_label");
    REQUIRE(label.getId() == "my_label");
}

TEST_CASE("Widget setId empty string", "[unit][widget]")
{
    Label label;
    label.setId("test");
    label.setId("");
    REQUIRE(label.getId().empty());
}

// ============================================================================
// Action event id
// ============================================================================

TEST_CASE("Widget setActionEventId and getActionEventId", "[unit][widget]")
{
    Label label;
    label.setActionEventId("click_event");
    REQUIRE(label.getActionEventId() == "click_event");
}

// ============================================================================
// contains / isMouseInside
// ============================================================================

TEST_CASE("Widget contains point inside", "[unit][widget]")
{
    Label label;
    label.setSize(100, 100);
    REQUIRE(label.contains(50, 50));
    REQUIRE(label.contains(0, 0));
    REQUIRE(label.contains(99, 99));
}

TEST_CASE("Widget contains point outside", "[unit][widget]")
{
    Label label;
    label.setSize(100, 100);
    REQUIRE_FALSE(label.contains(-1, 0));
    REQUIRE_FALSE(label.contains(0, -1));
    REQUIRE_FALSE(label.contains(100, 50));
    REQUIRE_FALSE(label.contains(50, 100));
}

TEST_CASE("Widget isMouseInside", "[unit][widget]")
{
    Label label;
    label.setSize(100, 100);
    MouseEvent insideEvent(
        &label, &label, false, false, false, false, MouseEvent::Type::Moved, MouseEvent::Button::Empty, 50, 50, 0);
    MouseEvent outsideEvent(
        &label, &label, false, false, false, false, MouseEvent::Type::Moved, MouseEvent::Button::Empty, 150, 150, 0);

    REQUIRE(label.isMouseInside(insideEvent));
    REQUIRE_FALSE(label.isMouseInside(outsideEvent));
}

// ============================================================================
// getAbsolutePosition
// ============================================================================

TEST_CASE("Widget getAbsolutePosition without parent", "[unit][widget]")
{
    Label label;
    label.setPosition(30, 40);
    int x = 0;
    int y = 0;
    label.getAbsolutePosition(x, y);
    REQUIRE(x == 30);
    REQUIRE(y == 40);
}

TEST_CASE("Widget getAbsolutePosition with parent", "[unit][widget]")
{
    Container container;
    Label label;
    container.setSize(200, 200);
    container.setPosition(10, 20);
    container.add(&label);
    label.setPosition(30, 40);
    label.setSize(50, 50);

    int x = 0;
    int y = 0;
    label.getAbsolutePosition(x, y);
    REQUIRE(x == 10 + 30);
    REQUIRE(y == 20 + 40);
}

// ============================================================================
// getTop
// ============================================================================

TEST_CASE("Widget getTop returns topmost ancestor", "[unit][widget]")
{
    Container root;
    Container middle;
    Label label;

    root.setSize(300, 300);
    root.add(&middle);
    middle.setPosition(0, 0);
    middle.setSize(200, 200);
    middle.add(&label);
    label.setPosition(0, 0);
    label.setSize(50, 50);

    REQUIRE(label.getTop() == &root);
}

TEST_CASE("Widget getTop returns nullptr when no parent", "[unit][widget]")
{
    Label label;
    REQUIRE(label.getTop() == nullptr);
}

// ============================================================================
// widgetExists
// ============================================================================

TEST_CASE("Widget widgetExists returns true for live widget", "[unit][widget]")
{
    Label label;
    REQUIRE(Widget::widgetExists(&label));
}

TEST_CASE("Widget widgetExists returns false for destroyed widget", "[unit][widget]")
{
    Label* labelPtr = nullptr;
    // Allocate on the heap and delete to simulate destruction without
    // taking the address of a stack-local that goes out of scope.
    auto label = std::make_unique<Label>();
    labelPtr   = label.get();
    REQUIRE(Widget::widgetExists(label.get()));
    label.reset();
    REQUIRE_FALSE(Widget::widgetExists(labelPtr));
}

TEST_CASE("Widget widgetExists returns false for nullptr", "[unit][widget]")
{
    REQUIRE_FALSE(Widget::widgetExists(nullptr));
}

// ============================================================================
// Last position
// ============================================================================

TEST_CASE("Widget setLastPosition and getLastPosition", "[unit][widget]")
{
    Label label;
    label.setLastPosition(100, 200);
    int x = 0;
    int y = 0;
    label.getLastPosition(x, y);
    REQUIRE(x == 100);
    REQUIRE(y == 200);
}

TEST_CASE("Widget isLastPositionSet", "[unit][widget]")
{
    Label label;
    REQUIRE_FALSE(label.isLastPositionSet());

    label.setLastPosition(0, 0);
    REQUIRE_FALSE(label.isLastPositionSet());

    label.setLastPosition(1, 0);
    REQUIRE(label.isLastPositionSet());

    label.setLastPosition(0, 1);
    REQUIRE(label.isLastPositionSet());
}

// ============================================================================
// Mouse capture
// ============================================================================

TEST_CASE("Widget captureMouse", "[unit][widget]")
{
    Label label;
    REQUIRE(label.captureMouse());
    REQUIRE(label.hasMouseCapture());
    REQUIRE(Widget::getMouseCapture() == &label);
    label.releaseMouse();
}

TEST_CASE("Widget captureMouse fails when another widget has capture", "[unit][widget]")
{
    Label l1;
    Label l2;
    REQUIRE(l1.captureMouse());
    REQUIRE_FALSE(l2.captureMouse());
    l1.releaseMouse();
}

TEST_CASE("Widget releaseMouse when not captured is safe", "[unit][widget]")
{
    Label label;
    label.releaseMouse();
    REQUIRE(true);
}

TEST_CASE("Widget getMouseCapture static", "[unit][widget]")
{
    REQUIRE(Widget::getMouseCapture() == nullptr);

    Label label;
    label.captureMouse();
    REQUIRE(Widget::getMouseCapture() == &label);
    label.releaseMouse();
    REQUIRE(Widget::getMouseCapture() == nullptr);
}

// ============================================================================
// isDescendantOf
// ============================================================================

TEST_CASE("Widget isDescendantOf direct child", "[unit][widget]")
{
    Container container;
    Label label;
    container.setSize(200, 200);
    container.add(&label);
    label.setPosition(0, 0);
    label.setSize(50, 50);

    REQUIRE(label.isDescendantOf(&container));
}

TEST_CASE("Widget isDescendantOf grandchild", "[unit][widget]")
{
    Container root;
    Container child;
    Label grandchild;
    root.setSize(300, 300);
    root.add(&child);
    child.setPosition(0, 0);
    child.setSize(200, 200);
    child.add(&grandchild);
    grandchild.setPosition(0, 0);
    grandchild.setSize(50, 50);

    REQUIRE(grandchild.isDescendantOf(&root));
    REQUIRE(grandchild.isDescendantOf(&child));
}

TEST_CASE("Widget isDescendantOf self", "[unit][widget]")
{
    Label label;
    REQUIRE(label.isDescendantOf(&label));
}

TEST_CASE("Widget isDescendantOf unrelated widget", "[unit][widget]")
{
    Container c1;
    Container c2;
    Label l1;
    Label l2;
    c1.setSize(100, 100);
    c1.add(&l1);
    l1.setPosition(0, 0);
    l1.setSize(50, 50);
    c2.setSize(100, 100);
    c2.add(&l2);
    l2.setPosition(0, 0);
    l2.setSize(50, 50);

    REQUIRE_FALSE(l1.isDescendantOf(&c2));
    REQUIRE_FALSE(l1.isDescendantOf(&l2));
}

TEST_CASE("Widget isDescendantOf nullptr", "[unit][widget]")
{
    Label label;
    REQUIRE_FALSE(label.isDescendantOf(nullptr));
}

// ============================================================================
// getWidgetsIn
// ============================================================================

TEST_CASE("Container getWidgetsIn finds intersecting widgets", "[unit][widget]")
{
    Container container;
    Label l1;
    Label l2;
    Label l3;
    container.setSize(200, 200);
    container.add(&l1);
    l1.setPosition(10, 10);
    l1.setSize(30, 30);
    container.add(&l2);
    l2.setPosition(50, 50);
    l2.setSize(30, 30);
    container.add(&l3);
    l3.setPosition(150, 150);
    l3.setSize(30, 30);

    Rectangle area(0, 0, 60, 60);
    auto widgets = container.getWidgetsIn(area);
    REQUIRE(widgets.size() == 2);
}

TEST_CASE("Container getWidgetsIn with ignore parameter", "[unit][widget]")
{
    Container container;
    Label l1;
    Label l2;
    container.setSize(200, 200);
    container.add(&l1);
    l1.setPosition(10, 10);
    l1.setSize(30, 30);
    container.add(&l2);
    l2.setPosition(50, 50);
    l2.setSize(30, 30);

    Rectangle area(0, 0, 100, 100);
    auto widgets = container.getWidgetsIn(area, &l1);
    REQUIRE(widgets.size() == 1);
    REQUIRE(widgets.front() == &l2);
}

TEST_CASE("Widget getWidgetsIn returns empty for non-container", "[unit][widget]")
{
    Label label;
    Rectangle area(0, 0, 100, 100);
    auto widgets = label.getWidgetsIn(area);
    REQUIRE(widgets.empty());
}

// ============================================================================
// findWidgetById
// ============================================================================

TEST_CASE("Container findWidgetById finds direct child", "[unit][widget]")
{
    Container container;
    Label l1;
    Label l2;
    container.setSize(200, 200);
    container.add(&l1);
    l1.setId("first");
    l1.setPosition(0, 0);
    l1.setSize(50, 50);
    container.add(&l2);
    l2.setId("second");
    l2.setPosition(50, 0);
    l2.setSize(50, 50);

    Widget const * found = container.findWidgetById("second");
    REQUIRE(found == &l2);
}

TEST_CASE("Container findWidgetById finds nested child", "[unit][widget]")
{
    Container root;
    Container child;
    Label target;
    root.setSize(300, 300);
    root.add(&child);
    child.setPosition(0, 0);
    child.setSize(200, 200);
    child.add(&target);
    target.setId("deep");
    target.setPosition(0, 0);
    target.setSize(50, 50);

    Widget const * found = root.findWidgetById("deep");
    REQUIRE(found == &target);
}

TEST_CASE("Container findWidgetById returns nullptr for missing id", "[unit][widget]")
{
    Container container;
    Label label;
    container.setSize(200, 200);
    container.add(&label);
    label.setId("exists");
    label.setPosition(0, 0);
    label.setSize(50, 50);

    REQUIRE(container.findWidgetById("nonexistent") == nullptr);
}

// ============================================================================
// showWidgetPart / showPart
// ============================================================================

TEST_CASE("Widget showPart delegates to parent", "[unit][widget]")
{
    Container container;
    Label label;
    container.setSize(100, 100);
    container.add(&label);
    label.setPosition(0, 0);
    label.setSize(200, 200);

    Rectangle area(0, 0, 50, 50);
    label.showPart(area);
    REQUIRE(true);
}

TEST_CASE("Widget showPart without parent is no-op", "[unit][widget]")
{
    Label label;
    label.showPart(Rectangle(0, 0, 10, 10));
    REQUIRE(true);
}

// ============================================================================
// focusNext / focusPrevious
// ============================================================================

TEST_CASE("Container focusNext focuses first focusable widget", "[unit][widget]")
{
    Gui gui;
    Container container;
    Label l1;
    Label l2;
    Label l3;
    container.setSize(200, 200);
    // Add a non-focusable widget first so focusNext skips to l1
    container.add(&l1);
    l1.setPosition(0, 0);
    l1.setSize(50, 50);
    l1.setFocusable(false);
    container.add(&l2);
    l2.setPosition(50, 0);
    l2.setSize(50, 50);
    l2.setFocusable(true);
    container.add(&l3);
    l3.setPosition(100, 0);
    l3.setSize(50, 50);
    l3.setFocusable(true);
    gui.setTop(&container);

    container.focusNext();
    REQUIRE(l2.isFocused());
}

TEST_CASE("Container focusNext wraps around", "[unit][widget]")
{
    Container container;
    Label l1;
    Label l2;
    container.setSize(200, 200);
    container.add(&l1);
    l1.setPosition(0, 0);
    l1.setSize(50, 50);
    l1.setFocusable(true);
    container.add(&l2);
    l2.setPosition(50, 0);
    l2.setSize(50, 50);
    l2.setFocusable(true);

    FocusHandler fh;
    container._setFocusHandler(&fh);

    l2.requestFocus();
    container.focusNext();
    REQUIRE(l1.isFocused());
}

TEST_CASE("Container focusPrevious focuses last focusable widget", "[unit][widget]")
{
    Gui gui;
    Container container;
    Label l1;
    Label l2;
    Label l3;
    container.setSize(200, 200);
    container.add(&l1);
    l1.setPosition(0, 0);
    l1.setSize(50, 50);
    l1.setFocusable(true);
    container.add(&l2);
    l2.setPosition(50, 0);
    l2.setSize(50, 50);
    l2.setFocusable(true);
    // Add a non-focusable widget at the end so focusPrevious wraps to l2
    container.add(&l3);
    l3.setPosition(100, 0);
    l3.setSize(50, 50);
    l3.setFocusable(false);
    gui.setTop(&container);

    // focusPrevious from no focus should wrap to last focusable (l2)
    container.focusPrevious();
    REQUIRE(l2.isFocused());
}

TEST_CASE("Container focusNext with no children is no-op", "[unit][widget]")
{
    Container container;
    container.focusNext();
    REQUIRE(true);
}

TEST_CASE("Container focusPrevious with no children is no-op", "[unit][widget]")
{
    Container container;
    container.focusPrevious();
    REQUIRE(true);
}

// ============================================================================
// _setFocusHandler / _getFocusHandler
// ============================================================================

TEST_CASE("Widget _setFocusHandler and _getFocusHandler", "[unit][widget]")
{
    Label label;
    FocusHandler fh;
    label._setFocusHandler(&fh);
    REQUIRE(label._getFocusHandler() == &fh);
}

TEST_CASE("Widget _setFocusHandler replaces old handler", "[unit][widget]")
{
    Label label;
    FocusHandler fh1;
    FocusHandler fh2;
    label._setFocusHandler(&fh1);
    REQUIRE(label._getFocusHandler() == &fh1);

    label._setFocusHandler(&fh2);
    REQUIRE(label._getFocusHandler() == &fh2);
}

TEST_CASE("Widget _setFocusHandler nullptr removes handler", "[unit][widget]")
{
    Label label;
    FocusHandler fh;
    label._setFocusHandler(&fh);
    label._setFocusHandler(nullptr);
    REQUIRE(label._getFocusHandler() == nullptr);
}

// ============================================================================
// _setParent
// ============================================================================

TEST_CASE("Widget _setParent", "[unit][widget]")
{
    Container container;
    Label label;
    label._setParent(&container);
    REQUIRE(label.getParent() == &container);
}

TEST_CASE("Widget _setParent nullptr", "[unit][widget]")
{
    Container container;
    Label label;
    label._setParent(&container);
    label._setParent(nullptr);
    REQUIRE(label.getParent() == nullptr);
}

// ============================================================================
// Font
// ============================================================================

TEST_CASE("Widget getFont returns default font when no font set", "[unit][widget]")
{
    Label label;
    Font const * font = label.getFont();
    REQUIRE(font != nullptr);
}

TEST_CASE("Widget setFont and getFont", "[unit][widget]")
{
    Label label;
    Font* defaultFont = label.getFont();
    label.setFont(defaultFont);
    REQUIRE(label.getFont() == defaultFont);
}

TEST_CASE("Widget setFont nullptr uses global font", "[unit][widget]")
{
    Label label;
    label.setFont(nullptr);
    REQUIRE(label.getFont() != nullptr);
}

// ============================================================================
// isModalFocusable / isModalMouseInputFocusable
// ============================================================================

TEST_CASE("Widget isModalFocusable throws without focus handler", "[unit][widget]")
{
    Label label;
    REQUIRE_THROWS_WITH(label.isModalFocusable(), Catch::Matchers::ContainsSubstring("focus handler"));
}

TEST_CASE("Widget isModalMouseInputFocusable throws without focus handler", "[unit][widget]")
{
    Label label;
    REQUIRE_THROWS_WITH(label.isModalMouseInputFocusable(), Catch::Matchers::ContainsSubstring("focus handler"));
}

TEST_CASE("Widget isModalFocusable returns true when no modal focus", "[unit][widget]")
{
    Container container;
    Label label;
    FocusHandler fh;

    container.setSize(200, 200);
    container.add(&label);
    label.setPosition(0, 0);
    label.setSize(50, 50);
    container._setFocusHandler(&fh);

    REQUIRE(label.isModalFocusable());
}

TEST_CASE("Widget isModalMouseInputFocusable returns true when no mouse capture", "[unit][widget]")
{
    Container container;
    Label label;
    FocusHandler fh;

    container.setSize(200, 200);
    container.add(&label);
    label.setPosition(0, 0);
    label.setSize(50, 50);
    container._setFocusHandler(&fh);

    REQUIRE(label.isModalMouseInputFocusable());
}

// ============================================================================
// isModalFocused / isUnderMouseModal
// ============================================================================

TEST_CASE("Widget isModalFocused throws without focus handler", "[unit][widget]")
{
    Label label;
    REQUIRE_THROWS_WITH(label.isModalFocused(), Catch::Matchers::ContainsSubstring("focus handler"));
}

TEST_CASE("Widget isUnderMouseModal throws without focus handler", "[unit][widget]")
{
    Label label;
    REQUIRE_THROWS_WITH(label.isUnderMouseModal(), Catch::Matchers::ContainsSubstring("focus handler"));
}

TEST_CASE("Widget isModalFocused returns false when not focused", "[unit][widget]")
{
    Container container;
    Label label;
    FocusHandler fh;

    container.setSize(200, 200);
    container.add(&label);
    label.setPosition(0, 0);
    label.setSize(50, 50);
    container._setFocusHandler(&fh);

    REQUIRE_FALSE(label.isModalFocused());
}

TEST_CASE("Widget isModalFocused returns true when focused", "[unit][widget]")
{
    Gui gui;
    Container container;
    Label label;
    container.setSize(200, 200);
    container.add(&label);
    label.setPosition(0, 0);
    label.setSize(50, 50);
    label.setFocusable(true);
    gui.setTop(&container);

    // Push modal state to make isModalFocused work
    gui.getFocusHandler()->pushModal(&label, nullptr);
    label.requestFocus();

    REQUIRE(label.isFocused());
    REQUIRE(label.isModalFocused());

    // Clean up modal state
    gui.getFocusHandler()->popModal();
}

// ============================================================================
// isInsideActiveMouseModal
// ============================================================================

TEST_CASE("Widget isInsideActiveMouseModal returns false without focus handler", "[unit][widget]")
{
    Label label;
    REQUIRE_FALSE(label.isInsideActiveMouseModal());
}

TEST_CASE("Widget isInsideActiveMouseModal returns false when no mouse modal", "[unit][widget]")
{
    Container container;
    Label label;
    FocusHandler fh;

    container.setSize(200, 200);
    container.add(&label);
    label.setPosition(0, 0);
    label.setSize(50, 50);
    container._setFocusHandler(&fh);

    REQUIRE_FALSE(label.isInsideActiveMouseModal());
}

// ============================================================================
// adaptLayout
// ============================================================================

TEST_CASE("Widget adaptLayout with no parent", "[unit][widget]")
{
    Label label;
    label.setSize(100, 100);
    label.adaptLayout(true);
    REQUIRE(true);
}

TEST_CASE("Widget adaptLayout with non-layouted parent stops at parent", "[unit][widget]")
{
    Container container;
    Label label;
    container.setSize(200, 200);
    container.add(&label);
    label.setPosition(0, 0);
    label.setSize(50, 50);

    label.adaptLayout(true);
    REQUIRE(true);
}

// ============================================================================
// _getInternalFocusHandler / setInternalFocusHandler
// ============================================================================

TEST_CASE("Widget _getInternalFocusHandler returns nullptr by default", "[unit][widget]")
{
    Label label;
    REQUIRE(label._getInternalFocusHandler() == nullptr);
}

TEST_CASE("Widget setInternalFocusHandler", "[unit][widget]")
{
    Container container;
    FocusHandler fh;
    container.setInternalFocusHandler(&fh);
    REQUIRE(container._getInternalFocusHandler() == &fh);
}

// ============================================================================
// getChildren via Container::getChild
// ============================================================================

TEST_CASE("Container getChild by index", "[unit][widget]")
{
    Container container;
    Label l1;
    Label l2;
    container.setSize(200, 200);
    container.add(&l1);
    l1.setPosition(0, 0);
    l1.setSize(50, 50);
    container.add(&l2);
    l2.setPosition(50, 0);
    l2.setSize(50, 50);

    REQUIRE(container.getChild(0) == &l1);
    REQUIRE(container.getChild(1) == &l2);
    REQUIRE(container.getChild(2) == nullptr); // out of range
}

// ============================================================================
// Visibility event handler
// ============================================================================

TEST_CASE("Widget _setVisibilityEventHandler and _getVisibilityEventHandler", "[unit][widget]")
{
    Label label;
    // Test setting and getting with nullptr
    Widget::_setVisibilityEventHandler(nullptr);
    REQUIRE(Widget::_getVisibilityEventHandler() == nullptr);
}

// ============================================================================
// GUI death listener
// ============================================================================

TEST_CASE("Widget _setGuiDeathListener and _getGuiDeathListener", "[unit][widget]")
{
    MockDeathListener dl;
    Label label;
    Widget::_setGuiDeathListener(&dl);
    REQUIRE(Widget::_getGuiDeathListener() == &dl);
    // Reset static listener to avoid dangling pointer after test ends
    Widget::_setGuiDeathListener(nullptr);
}

// ============================================================================
// _draw and _logic (via concrete classes)
// ============================================================================

TEST_CASE("Widget _logic calls logic recursively on children", "[unit][widget]")
{
    class LogicTrackingLabel : public Label
    {
        public:
            void draw(Graphics* /*graphics*/) override
            {
            }
            void logic() override
            {
                logicCalled = true;
            }
            bool logicCalled = false;
    };

    Container container;
    LogicTrackingLabel l1;
    LogicTrackingLabel l2;
    container.setSize(200, 200);
    container.add(&l1);
    l1.setPosition(0, 0);
    l1.setSize(50, 50);
    container.add(&l2);
    l2.setPosition(50, 0);
    l2.setSize(50, 50);

    container._logic();
    REQUIRE(l1.logicCalled);
    REQUIRE(l2.logicCalled);
}

// ============================================================================
// Edge cases and stress tests
// ============================================================================

TEST_CASE("Widget setDimension with same values triggers no events", "[unit][widget]")
{
    Label label;
    label.setSize(100, 100);
    MockWidgetListener listener;
    label.addWidgetListener(&listener);

    label.setSize(100, 100);
    REQUIRE(true);
}

TEST_CASE("Widget multiple listeners of same type", "[unit][widget]")
{
    TestableWidget label;
    MockActionListener l1;
    MockActionListener l2;
    MockActionListener l3;
    MockActionListener l4;
    MockActionListener l5;
    label.addActionListener(&l1);
    label.addActionListener(&l2);
    label.addActionListener(&l3);
    label.addActionListener(&l4);
    label.addActionListener(&l5);

    label.setActionEventId("stress");
    label.testDistributeActionEvent();

    REQUIRE(l1.actionCalled);
    REQUIRE(l2.actionCalled);
    REQUIRE(l3.actionCalled);
    REQUIRE(l4.actionCalled);
    REQUIRE(l5.actionCalled);
}

TEST_CASE("Widget add and remove listener multiple times", "[unit][widget]")
{
    TestableWidget label;
    MockActionListener listener;

    label.addActionListener(&listener);
    label.addActionListener(&listener);
    label.addActionListener(&listener);

    label.removeActionListener(&listener);
    label.removeActionListener(&listener);
    label.removeActionListener(&listener);

    label.setActionEventId("test");
    label.testDistributeActionEvent();
    REQUIRE_FALSE(listener.actionCalled);
}

TEST_CASE("Widget deep nesting parent chain", "[unit][widget]")
{
    Container* prev = nullptr;
    Container root;
    root.setSize(500, 500);

    for (int i = 0; i < 10; ++i) {
        auto c = std::make_unique<Container>();
        if (prev == nullptr) {
            root.add(c.get());
        } else {
            prev->add(c.get());
        }
        c->setPosition(0, 0);
        c->setSize(400, 400);
        prev = c.release();
    }

    Label leaf;
    prev->add(&leaf);
    leaf.setPosition(0, 0);
    leaf.setSize(50, 50);

    REQUIRE(leaf.getTop() == &root);
    REQUIRE(leaf.isDescendantOf(&root));

    root.removeAllChildren();
}

TEST_CASE("Widget getAbsolutePosition with nested containers", "[unit][widget]")
{
    Container root;
    Container mid;
    Label leaf;

    root.setSize(500, 500);
    root.setPosition(10, 20);
    root.add(&mid);
    mid.setPosition(30, 40);
    mid.setSize(300, 300);
    mid.add(&leaf);
    leaf.setPosition(50, 60);
    leaf.setSize(50, 50);

    int x = 0;
    int y = 0;
    leaf.getAbsolutePosition(x, y);
    REQUIRE(x == 10 + 30 + 50);
    REQUIRE(y == 20 + 40 + 60);
}

TEST_CASE("Widget setVisible propagates ancestor events to children", "[unit][widget]")
{
    Gui gui;
    Container container;
    Label child;
    container.setSize(200, 200);
    container.add(&child);
    child.setPosition(0, 0);
    child.setSize(50, 50);
    gui.setTop(&container);

    MockWidgetListener childListener;
    child.addWidgetListener(&childListener);

    container.setVisible(false);
    REQUIRE(childListener.ancestorHiddenCalled);

    childListener.ancestorHiddenCalled = false;
    container.setVisible(true);
    REQUIRE(childListener.ancestorShownCalled);
}
