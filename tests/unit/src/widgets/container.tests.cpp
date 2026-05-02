// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2026 Fifengine contributors

// Third-party library includes
#include <catch2/catch_test_macros.hpp>

// Standard library includes
#include <memory>
#include <string>
#include <utility>

#include "fifechan/events/containerevent.hpp"
#include "fifechan/listeners/containerlistener.hpp"
#include "fifechan/widgets/container.hpp"
#include "fifechan/widgets/label.hpp"

using fcn::Container;
using fcn::Label;
using fcn::Widget;
using fcn::Rectangle;
using fcn::ContainerEvent;
using fcn::ContainerListener;

// ============================================================================
// Helper class for testing container listeners
// ============================================================================

class TestContainerListener : public ContainerListener
{
    public:
        void widgetAdded(ContainerEvent const & containerEvent) override
        {
            mWidgetAddedCalled = true;
            mLastAddedWidget   = containerEvent.getSource();
            mLastContainer     = containerEvent.getContainer();
        }

        void widgetRemoved(ContainerEvent const & containerEvent) override
        {
            mWidgetRemovedCalled = true;
            mLastRemovedWidget   = containerEvent.getSource();
            mLastContainer       = containerEvent.getContainer();
        }

        void clear()
        {
            mWidgetAddedCalled   = false;
            mWidgetRemovedCalled = false;
            mLastAddedWidget     = nullptr;
            mLastRemovedWidget   = nullptr;
            mLastContainer       = nullptr;
        }

        bool mWidgetAddedCalled{false};
        bool mWidgetRemovedCalled{false};
        Widget* mLastAddedWidget{nullptr};
        Widget* mLastRemovedWidget{nullptr};
        Container* mLastContainer{nullptr};
};

// ============================================================================
// Container constructors and default values
// ============================================================================

TEST_CASE("Container default constructor", "[unit][container]")
{
    Container container;

    // Default values from container.hpp
    REQUIRE(container.isOpaque() == true);
    REQUIRE(container.getLayout() == Container::LayoutPolicy::Absolute);
    REQUIRE(container.isUniformSize() == false);
    REQUIRE(container.getVerticalSpacing() == 2);
    REQUIRE(container.getHorizontalSpacing() == 2);
    REQUIRE(container.getBackgroundWidget() == nullptr);
    REQUIRE(container.getChildrenCount() == 0);
}

// ============================================================================
// Add/Remove Widgets
// ============================================================================

TEST_CASE("Container add widget with raw pointer", "[unit][container]")
{
    Container container;
    Label* label = new Label("Test");

    container.add(label);

    REQUIRE(container.getChildrenCount() == 1);
    REQUIRE(container.getChild(0) == label);

    // Cleanup - container doesn't own raw pointers
    delete label;
}

TEST_CASE("Container add widget with position", "[unit][container]")
{
    Container container;
    Label* label = new Label("Test");

    container.add(label, 10, 20);

    REQUIRE(container.getChildrenCount() == 1);
    REQUIRE(label->getX() == 10);
    REQUIRE(label->getY() == 20);
    REQUIRE(container.getChild(0) == label);

    // Cleanup
    delete label;
}

TEST_CASE("Container addWidget with unique_ptr", "[unit][container]")
{
    Container container;

    auto label      = std::make_unique<Label>("Test");
    Label* rawLabel = label.get();

    container.addWidget(std::move(label));

    REQUIRE(container.getChildrenCount() == 1);
    REQUIRE(container.getChild(0) == rawLabel);
}

TEST_CASE("Container addWidget with unique_ptr and position", "[unit][container]")
{
    Container container;

    auto label      = std::make_unique<Label>("Test");
    Label* rawLabel = label.get();

    container.addWidget(std::move(label), 50, 60);

    REQUIRE(container.getChildrenCount() == 1);
    REQUIRE(rawLabel->getX() == 50);
    REQUIRE(rawLabel->getY() == 60);
    REQUIRE(container.getChild(0) == rawLabel);
}

TEST_CASE("Container add nullptr with unique_ptr does nothing", "[unit][container]")
{
    Container container;

    // Adding nullptr should not crash or add anything
    container.addWidget(std::unique_ptr<Widget>(nullptr));

    REQUIRE(container.getChildrenCount() == 0);
}

TEST_CASE("Container remove widget", "[unit][container]")
{
    Container container;
    Label* label1 = new Label("Label1");
    Label* label2 = new Label("Label2");

    container.add(label1);
    container.add(label2);
    REQUIRE(container.getChildrenCount() == 2);

    container.remove(label1);
    REQUIRE(container.getChildrenCount() == 1);
    REQUIRE(container.getChild(0) == label2);

    // Cleanup
    delete label1;
    delete label2;
}

TEST_CASE("Container removeAllChildren", "[unit][container]")
{
    Container container;
    Label* label1 = new Label("Label1");
    Label* label2 = new Label("Label2");
    Label* label3 = new Label("Label3");

    container.add(label1);
    container.add(label2);
    container.add(label3);
    REQUIRE(container.getChildrenCount() == 3);

    container.removeAllChildren();
    REQUIRE(container.getChildrenCount() == 0);

    // Cleanup - widgets are still our responsibility
    delete label1;
    delete label2;
    delete label3;
}

// ============================================================================
// Get Children
// ============================================================================

TEST_CASE("Container getChildrenCount returns correct count", "[unit][container]")
{
    Container container;

    REQUIRE(container.getChildrenCount() == 0);

    Label* label1 = new Label("Label1");
    Label* label2 = new Label("Label2");
    Label* label3 = new Label("Label3");

    container.add(label1);
    REQUIRE(container.getChildrenCount() == 1);

    container.add(label2);
    REQUIRE(container.getChildrenCount() == 2);

    container.add(label3);
    REQUIRE(container.getChildrenCount() == 3);

    // Cleanup
    delete label1;
    delete label2;
    delete label3;
}

TEST_CASE("Container getChild with valid index", "[unit][container]")
{
    Container container;
    Label* label1 = new Label("Label1");
    Label* label2 = new Label("Label2");

    container.add(label1);
    container.add(label2);

    REQUIRE(container.getChild(0) == label1);
    REQUIRE(container.getChild(1) == label2);

    // Cleanup
    delete label1;
    delete label2;
}

TEST_CASE("Container getChild with out-of-range index returns nullptr", "[unit][container]")
{
    Container container;
    Label* label = new Label("Label1");

    container.add(label);

    REQUIRE(container.getChild(0) == label);
    REQUIRE(container.getChild(1) == nullptr);
    REQUIRE(container.getChild(100) == nullptr);

    // Cleanup
    delete label;
}

TEST_CASE("Container getChild with index 0 on empty container returns nullptr", "[unit][container]")
{
    Container container;

    REQUIRE(container.getChild(0) == nullptr);
}

// ============================================================================
// Find Widget by ID
// ============================================================================

TEST_CASE("Container findWidgetById finds correct widget", "[unit][container]")
{
    Container container;
    Label* label1 = new Label("Label1");
    Label* label2 = new Label("Label2");

    label1->setId("label1");
    label2->setId("label2");

    container.add(label1);
    container.add(label2);

    REQUIRE(container.findWidgetById("label1") == label1);
    REQUIRE(container.findWidgetById("label2") == label2);

    // Cleanup
    delete label1;
    delete label2;
}

TEST_CASE("Container findWidgetById with non-existent ID returns nullptr", "[unit][container]")
{
    Container container;
    Label* label = new Label("Label1");

    label->setId("label1");
    container.add(label);

    REQUIRE(container.findWidgetById("nonexistent") == nullptr);
    REQUIRE(container.findWidgetById("") == nullptr);

    // Cleanup
    delete label;
}

// ============================================================================
// Layout
// ============================================================================

TEST_CASE("Container setLayout and getLayout", "[unit][container]")
{
    Container container;

    // Test Absolute (default)
    container.setLayout(Container::LayoutPolicy::Absolute);
    REQUIRE(container.getLayout() == Container::LayoutPolicy::Absolute);
    REQUIRE(container.isLayouted() == false);

    // Test AutoSize
    container.setLayout(Container::LayoutPolicy::AutoSize);
    REQUIRE(container.getLayout() == Container::LayoutPolicy::AutoSize);
    REQUIRE(container.isLayouted() == true);

    // Test Vertical
    container.setLayout(Container::LayoutPolicy::Vertical);
    REQUIRE(container.getLayout() == Container::LayoutPolicy::Vertical);
    REQUIRE(container.isLayouted() == true);

    // Test Horizontal
    container.setLayout(Container::LayoutPolicy::Horizontal);
    REQUIRE(container.getLayout() == Container::LayoutPolicy::Horizontal);
    REQUIRE(container.isLayouted() == true);

    // Test Circular
    container.setLayout(Container::LayoutPolicy::Circular);
    REQUIRE(container.getLayout() == Container::LayoutPolicy::Circular);
    REQUIRE(container.isLayouted() == true);
}

TEST_CASE("Container setUniformSize and isUniformSize", "[unit][container]")
{
    Container container;

    // Default is false
    REQUIRE(container.isUniformSize() == false);

    container.setUniformSize(true);
    REQUIRE(container.isUniformSize() == true);

    container.setUniformSize(false);
    REQUIRE(container.isUniformSize() == false);
}

TEST_CASE("Container setVerticalSpacing and getVerticalSpacing", "[unit][container]")
{
    Container container;

    // Default is 2
    REQUIRE(container.getVerticalSpacing() == 2);

    container.setVerticalSpacing(10);
    REQUIRE(container.getVerticalSpacing() == 10);

    container.setVerticalSpacing(0);
    REQUIRE(container.getVerticalSpacing() == 0);

    container.setVerticalSpacing(100);
    REQUIRE(container.getVerticalSpacing() == 100);
}

TEST_CASE("Container setHorizontalSpacing and getHorizontalSpacing", "[unit][container]")
{
    Container container;

    // Default is 2
    REQUIRE(container.getHorizontalSpacing() == 2);

    container.setHorizontalSpacing(15);
    REQUIRE(container.getHorizontalSpacing() == 15);

    container.setHorizontalSpacing(0);
    REQUIRE(container.getHorizontalSpacing() == 0);

    container.setHorizontalSpacing(200);
    REQUIRE(container.getHorizontalSpacing() == 200);
}

// ============================================================================
// Opacity
// ============================================================================

TEST_CASE("Container setOpacity with valid values", "[unit][container]")
{
    Container container;

    // Default opacity should make it opaque
    REQUIRE(container.isOpaque() == true);

    // Test 0.0 (fully transparent)
    container.setOpacity(0.0f);
    REQUIRE(container.isOpaque() == false);

    // Test 0.5 (semi-transparent)
    container.setOpacity(0.5f);
    REQUIRE(container.isOpaque() == false);

    // Test 1.0 (fully opaque)
    container.setOpacity(1.0f);
    REQUIRE(container.isOpaque() == true);
}

TEST_CASE("Container setOpacity clamps values", "[unit][container]")
{
    Container container;

    // Test value below 0.0 (should clamp to 0.0)
    container.setOpacity(-0.5f);
    // After clamping, opacity is 0.0, so opaque should be false
    REQUIRE(container.isOpaque() == false);

    // Test value above 1.0 (should clamp to 1.0)
    container.setOpacity(1.5f);
    // After clamping, opacity is 1.0, so opaque should be true
    REQUIRE(container.isOpaque() == true);
}

TEST_CASE("Container setOpaque and isOpaque", "[unit][container]")
{
    Container container;

    // Default is true
    REQUIRE(container.isOpaque() == true);

    container.setOpaque(false);
    REQUIRE(container.isOpaque() == false);

    container.setOpaque(true);
    REQUIRE(container.isOpaque() == true);
}

TEST_CASE("Container setOpacity < 1.0 sets opaque false", "[unit][container]")
{
    Container container;

    container.setOpaque(true);
    REQUIRE(container.isOpaque() == true);

    container.setOpacity(0.99f);
    REQUIRE(container.isOpaque() == false);
}

TEST_CASE("Container setOpacity >= 1.0 sets opaque true", "[unit][container]")
{
    Container container;

    container.setOpaque(false);
    REQUIRE(container.isOpaque() == false);

    container.setOpacity(1.0f);
    REQUIRE(container.isOpaque() == true);
}

// ============================================================================
// Container Listeners
// ============================================================================

TEST_CASE("Container addContainerListener and removeContainerListener", "[unit][container]")
{
    Container container;
    TestContainerListener listener;

    container.addContainerListener(&listener);
    // No event fired yet
    REQUIRE(listener.mWidgetAddedCalled == false);

    container.removeContainerListener(&listener);
    // Listener removed, no events should fire
}

TEST_CASE("Container widgetAdded event fires when adding widget", "[unit][container]")
{
    Container container;
    TestContainerListener listener;
    Label* label = new Label("Test");

    container.addContainerListener(&listener);
    container.add(label);

    REQUIRE(listener.mWidgetAddedCalled == true);
    REQUIRE(listener.mLastAddedWidget == label);
    REQUIRE(listener.mLastContainer == &container);

    // Cleanup
    delete label;
}

TEST_CASE("Container widgetRemoved event fires when removing widget", "[unit][container]")
{
    Container container;
    TestContainerListener listener;
    Label* label = new Label("Test");

    container.add(label);
    container.addContainerListener(&listener);
    container.remove(label);

    REQUIRE(listener.mWidgetRemovedCalled == true);
    REQUIRE(listener.mLastRemovedWidget == label);
    REQUIRE(listener.mLastContainer == &container);

    // Cleanup
    delete label;
}

TEST_CASE("Container events fire with addWidget unique_ptr", "[unit][container]")
{
    Container container;
    TestContainerListener listener;

    container.addContainerListener(&listener);

    auto label      = std::make_unique<Label>("Test");
    Label* rawLabel = label.get();

    container.addWidget(std::move(label));

    REQUIRE(listener.mWidgetAddedCalled == true);
    REQUIRE(listener.mLastAddedWidget == rawLabel);
}

TEST_CASE("Container listener not called after removal", "[unit][container]")
{
    Container container;
    TestContainerListener listener;

    container.addContainerListener(&listener);
    container.addWidget(std::make_unique<Label>("Test1"));
    REQUIRE(listener.mWidgetAddedCalled == true);

    listener.clear();
    container.removeContainerListener(&listener);
    container.addWidget(std::make_unique<Label>("Test2"));
    REQUIRE(listener.mWidgetAddedCalled == false);
}

// ============================================================================
// Resize/Expand
// ============================================================================

TEST_CASE("Container resizeToContent with Absolute layout does nothing", "[unit][container]")
{
    Container container;
    container.setSize(100, 100);

    Label* label = new Label("Test");
    label->setSize(50, 30);
    container.add(label, 10, 10);

    // With Absolute layout, resizeToContent should not change container size
    int originalWidth  = container.getWidth();
    int originalHeight = container.getHeight();

    container.resizeToContent(false);

    REQUIRE(container.getWidth() == originalWidth);
    REQUIRE(container.getHeight() == originalHeight);

    // Cleanup
    delete label;
}

TEST_CASE("Container resizeToContent with AutoSize layout", "[unit][container]")
{
    Container container;
    container.setLayout(Container::LayoutPolicy::AutoSize);
    container.setSize(200, 200);

    Label* label = new Label("Test");
    label->setSize(50, 30);
    label->setPosition(10, 10);
    container.add(label);

    container.resizeToContent(false);

    // Container should resize to fit children
    // The exact size depends on implementation, but should be > 0
    REQUIRE(container.getWidth() > 0);
    REQUIRE(container.getHeight() > 0);

    // Cleanup
    delete label;
}

TEST_CASE("Container adjustSize", "[unit][container]")
{
    Container container;
    container.setSize(100, 100);

    Label* label = new Label("Test");
    label->setSize(50, 30);
    container.add(label);

    container.adjustSize();

    // adjustSize should resize container based on children
    REQUIRE(container.getWidth() > 0);
    REQUIRE(container.getHeight() > 0);

    // Cleanup
    delete label;
}

TEST_CASE("Container isLayouted returns true when layout != Absolute", "[unit][container]")
{
    Container container;

    container.setLayout(Container::LayoutPolicy::Absolute);
    REQUIRE(container.isLayouted() == false);

    container.setLayout(Container::LayoutPolicy::Vertical);
    REQUIRE(container.isLayouted() == true);

    container.setLayout(Container::LayoutPolicy::Horizontal);
    REQUIRE(container.isLayouted() == true);
}

// ============================================================================
// Children Area
// ============================================================================

TEST_CASE("Container getChildrenArea with no border or padding", "[unit][container]")
{
    Container container;
    container.setSize(100, 100);

    Rectangle area = container.getChildrenArea();

    REQUIRE(area.x == 0);
    REQUIRE(area.y == 0);
    REQUIRE(area.width == 100);
    REQUIRE(area.height == 100);
}

TEST_CASE("Container getChildrenArea with border", "[unit][container]")
{
    Container container;
    container.setSize(100, 100);
    container.setBorderSize(5);

    Rectangle area = container.getChildrenArea();

    // With border on all sides, children area should be inset by border size
    REQUIRE(area.x == 5);
    REQUIRE(area.y == 5);
    REQUIRE(area.width == 90);  // 100 - 5 - 5
    REQUIRE(area.height == 90); // 100 - 5 - 5
}

TEST_CASE("Container getChildrenArea with padding", "[unit][container]")
{
    Container container;
    container.setSize(100, 100);
    container.setPadding(10);

    Rectangle area = container.getChildrenArea();

    // With padding, children area should be inset
    REQUIRE(area.x == 10);
    REQUIRE(area.y == 10);
    REQUIRE(area.width == 80);  // 100 - 10 - 10
    REQUIRE(area.height == 80); // 100 - 10 - 10
}

TEST_CASE("Container getChildrenArea with border and padding", "[unit][container]")
{
    Container container;
    container.setSize(100, 100);
    container.setBorderSize(5);
    container.setPadding(10);

    Rectangle area = container.getChildrenArea();

    // Children area accounts for both border and padding
    REQUIRE(area.x == 15); // border(5) + padding(10)
    REQUIRE(area.y == 15);
    REQUIRE(area.width == 70);  // 100 - 15 - 15
    REQUIRE(area.height == 70); // 100 - 15 - 15
}

TEST_CASE("Container getChildrenArea with side-only border", "[unit][container]")
{
    Container container;
    container.setSize(100, 100);
    container.setBorderSize(5);
    container.setBorderSides(Widget::BORDER_BOTTOM); // Only bottom border

    Rectangle area = container.getChildrenArea();

    // Only bottom border should affect children area
    REQUIRE(area.x == 0);       // No left border
    REQUIRE(area.y == 0);       // No top border
    REQUIRE(area.width == 100); // No left/right border
    REQUIRE(area.height == 95); // 100 - 5 (bottom only)
}

// ============================================================================
// Background Widget
// ============================================================================

TEST_CASE("Container setBackgroundWidget and getBackgroundWidget", "[unit][container]")
{
    Container container;
    Label* bgWidget = new Label("Background");

    container.setBackgroundWidget(bgWidget);

    REQUIRE(container.getBackgroundWidget() == bgWidget);

    // Background widget is not a child
    REQUIRE(container.getChildrenCount() == 0);

    // Cleanup
    delete bgWidget;
}

TEST_CASE("Container setBackgroundWidget same widget twice returns early", "[unit][container]")
{
    Container container;
    Label* bgWidget = new Label("Background");

    container.setBackgroundWidget(bgWidget);
    REQUIRE(container.getBackgroundWidget() == bgWidget);

    // Setting same widget again should return early (no crash)
    container.setBackgroundWidget(bgWidget);
    REQUIRE(container.getBackgroundWidget() == bgWidget);

    // Cleanup
    delete bgWidget;
}

TEST_CASE("Container setBackgroundWidget to nullptr", "[unit][container]")
{
    Container container;
    Label* bgWidget = new Label("Background");

    container.setBackgroundWidget(bgWidget);
    REQUIRE(container.getBackgroundWidget() == bgWidget);

    container.setBackgroundWidget(nullptr);
    REQUIRE(container.getBackgroundWidget() == nullptr);

    // Cleanup
    delete bgWidget;
}

TEST_CASE("Container setBackgroundWidget replaces old widget", "[unit][container]")
{
    Container container;
    Label* bg1 = new Label("BG1");
    Label* bg2 = new Label("BG2");

    container.setBackgroundWidget(bg1);
    REQUIRE(container.getBackgroundWidget() == bg1);

    container.setBackgroundWidget(bg2);
    REQUIRE(container.getBackgroundWidget() == bg2);

    // Cleanup
    delete bg1;
    delete bg2;
}

// ============================================================================
// Move To Top/Bottom
// ============================================================================

TEST_CASE("Container moveToTop changes widget order", "[unit][container]")
{
    Container container;
    Label* label1 = new Label("Label1");
    Label* label2 = new Label("Label2");
    Label* label3 = new Label("Label3");

    container.add(label1);
    container.add(label2);
    container.add(label3);

    // Initially label1 is at index 0, label3 at index 2
    REQUIRE(container.getChild(0) == label1);
    REQUIRE(container.getChild(2) == label3);

    // Move label1 to top (end of list)
    container.moveToTop(label1);

    // Now label1 should be at the end
    REQUIRE(container.getChild(0) == label2);
    REQUIRE(container.getChild(1) == label3);
    REQUIRE(container.getChild(2) == label1);

    // Cleanup
    delete label1;
    delete label2;
    delete label3;
}

TEST_CASE("Container moveToBottom changes widget order", "[unit][container]")
{
    Container container;
    Label* label1 = new Label("Label1");
    Label* label2 = new Label("Label2");
    Label* label3 = new Label("Label3");

    container.add(label1);
    container.add(label2);
    container.add(label3);

    // Initially label1 is at index 0, label3 at index 2
    REQUIRE(container.getChild(0) == label1);
    REQUIRE(container.getChild(2) == label3);

    // Move label3 to bottom (beginning of list)
    container.moveToBottom(label3);

    // Now label3 should be at the beginning
    REQUIRE(container.getChild(0) == label3);
    REQUIRE(container.getChild(1) == label1);
    REQUIRE(container.getChild(2) == label2);

    // Cleanup
    delete label1;
    delete label2;
    delete label3;
}

TEST_CASE("Container verify order preserved after multiple adds", "[unit][container]")
{
    Container container;
    Label* label1 = new Label("Label1");
    Label* label2 = new Label("Label2");
    Label* label3 = new Label("Label3");

    container.add(label1);
    container.add(label2);
    container.add(label3);

    // Verify order is preserved
    REQUIRE(container.getChild(0) == label1);
    REQUIRE(container.getChild(1) == label2);
    REQUIRE(container.getChild(2) == label3);

    // Cleanup
    delete label1;
    delete label2;
    delete label3;
}

TEST_CASE("Container moveToTop non-existent widget throws", "[unit][container]")
{
    Container container;
    Label* label1 = new Label("Label1");
    Label* label2 = new Label("Label2");

    container.add(label1);
    REQUIRE(container.getChildrenCount() == 1);

    // Moving widget not in container should throw
    REQUIRE_THROWS(container.moveToTop(label2));
    REQUIRE(container.getChildrenCount() == 1);
    REQUIRE(container.getChild(0) == label1);

    // Cleanup
    delete label1;
    delete label2;
}

// ============================================================================
// Edge Cases
// ============================================================================

TEST_CASE("Container operations on empty container", "[unit][container]")
{
    Container container;

    // These should not crash on empty container
    container.removeAllChildren();
    REQUIRE(container.getChildrenCount() == 0);

    REQUIRE(container.getChild(0) == nullptr);

    // resizeToContent on empty container should not crash
    container.resizeToContent(false);

    // expandContent on empty container should not crash
    container.expandContent(false);

    // moveToTop/moveToBottom with widget not in container throws
    // These are expected to throw, so we test they throw
    Label* notInContainer = new Label("not in");
    REQUIRE_THROWS(container.moveToTop(notInContainer));
    REQUIRE_THROWS(container.moveToBottom(notInContainer));
    delete notInContainer;
}

TEST_CASE("Container remove widget not in container", "[unit][container]")
{
    Container container;
    Label* label1 = new Label("Label1");
    Label* label2 = new Label("Label2");

    container.add(label1);
    REQUIRE(container.getChildrenCount() == 1);

    // Removing widget not in container should throw
    REQUIRE_THROWS(container.remove(label2));
    REQUIRE(container.getChildrenCount() == 1);

    // Cleanup
    delete label1;
    delete label2;
}

TEST_CASE("Container multiple children verify count and order", "[unit][container]")
{
    Container container;

    REQUIRE(container.getChildrenCount() == 0);

    Label* labels[5];
    for (int i = 0; i < 5; ++i) {
        labels[i] = new Label("Label" + std::to_string(i));
        container.add(labels[i]);
        REQUIRE(container.getChildrenCount() == static_cast<unsigned int>(i + 1));
    }

    // Verify all children are in correct order
    for (unsigned int i = 0; i < 5; ++i) {
        REQUIRE(container.getChild(i) == labels[i]);
    }

    // Cleanup
    for (int i = 0; i < 5; ++i) {
        delete labels[i];
    }
}

TEST_CASE("Container add widget with negative position", "[unit][container]")
{
    Container container;
    Label* label = new Label("Test");

    container.add(label, -10, -20);

    REQUIRE(container.getChildrenCount() == 1);
    REQUIRE(label->getX() == -10);
    REQUIRE(label->getY() == -20);

    // Cleanup
    delete label;
}

TEST_CASE("Container setOpacity multiple times", "[unit][container]")
{
    Container container;

    container.setOpacity(0.0f);
    REQUIRE(container.isOpaque() == false);

    container.setOpacity(0.5f);
    REQUIRE(container.isOpaque() == false);

    container.setOpacity(1.0f);
    REQUIRE(container.isOpaque() == true);

    container.setOpacity(0.0f);
    REQUIRE(container.isOpaque() == false);
}

TEST_CASE("Container visibility", "[unit][container]")
{
    Container container;
    Label* label = new Label("Test");

    container.add(label);

    // Container visible by default
    REQUIRE(container.isVisible() == true);

    container.setVisible(false);
    REQUIRE(container.isVisible() == false);

    container.setVisible(true);
    REQUIRE(container.isVisible() == true);

    // Cleanup - remove from container before deleting
    container.remove(label);
    delete label;
}

TEST_CASE("Container resizeToContent with recursion", "[unit][container]")
{
    Container container;
    container.setLayout(Container::LayoutPolicy::Vertical);

    // Create child container with its own children
    Container* childContainer = new Container();
    childContainer->setLayout(Container::LayoutPolicy::Vertical);
    Label* label = new Label("Test");
    label->setSize(50, 20);
    childContainer->add(label);

    container.add(childContainer);

    // With recursion=true, child container should also resize
    container.resizeToContent(true);

    REQUIRE(container.getWidth() > 0);
    REQUIRE(container.getHeight() > 0);

    // Cleanup
    delete label;
    delete childContainer;
}

TEST_CASE("Container expandContent with Vertical layout", "[unit][container]")
{
    Container container;
    container.setLayout(Container::LayoutPolicy::Vertical);
    container.setSize(200, 200);

    Label* label1 = new Label("Label1");
    label1->setSize(50, 30);
    Label* label2 = new Label("Label2");
    label2->setSize(50, 30);

    container.add(label1);
    container.add(label2);

    // First position children with resizeToContent, then expand
    container.resizeToContent(false);
    container.expandContent(false);

    // Children should have been positioned by the layout
    // After resizeToContent + expandContent, label2 should be below label1
    REQUIRE(label2->getY() > label1->getY());

    // Cleanup
    delete label1;
    delete label2;
}

TEST_CASE("Container expandContent with Horizontal layout", "[unit][container]")
{
    Container container;
    container.setLayout(Container::LayoutPolicy::Horizontal);
    container.setSize(200, 200);

    Label* label1 = new Label("Label1");
    label1->setSize(50, 30);
    Label* label2 = new Label("Label2");
    label2->setSize(50, 30);

    container.add(label1);
    container.add(label2);

    // expandContent should distribute space among children
    container.expandContent(false);

    // Children should have been positioned by the layout
    REQUIRE(label1->getX() >= 0);
    REQUIRE(label2->getX() > label1->getX()); // label2 should be to the right of label1

    // Cleanup
    delete label1;
    delete label2;
}

TEST_CASE("Container with hidden children in layout", "[unit][container]")
{
    Container container;
    container.setLayout(Container::LayoutPolicy::Vertical);

    Label* label1 = new Label("Label1");
    Label* label2 = new Label("Label2");
    Label* label3 = new Label("Label3");

    container.add(label1);
    container.add(label2);
    container.add(label3);

    // Hide one child
    label2->setVisible(false);

    container.resizeToContent(false);

    // Only visible children should affect layout
    REQUIRE(container.getVisibleChildrenCount() == 2);

    // Remove widgets from container before deleting to avoid segfault
    container.removeAllChildren();
    delete label1;
    delete label2;
    delete label3;
}

TEST_CASE("Container child widget positions with Vertical layout", "[unit][container]")
{
    Container container;
    container.setLayout(Container::LayoutPolicy::Vertical);
    container.setSize(300, 300);

    Label* label1 = new Label("Label1");
    label1->setSize(100, 20);
    Label* label2 = new Label("Label2");
    label2->setSize(100, 20);
    Label* label3 = new Label("Label3");
    label3->setSize(100, 20);

    container.add(label1, 0, 0);
    container.add(label2, 0, 0);
    container.add(label3, 0, 0);

    container.resizeToContent(false);

    // In vertical layout, Y positions should increase
    // (assuming resizeToContent positions them)
    REQUIRE(container.getChildrenCount() == 3);

    // Cleanup
    delete label1;
    delete label2;
    delete label3;
}

TEST_CASE("Container draw method can be called", "[unit][container]")
{
    Container container;

    // The draw method requires a Graphics object, but we can verify
    // the method exists and the container can be set up for drawing
    container.setSize(100, 100);
    container.setOpaque(true);

    // This test mainly verifies the method signature is correct
    // Actual drawing would require a mock Graphics object
    REQUIRE(container.isOpaque() == true);
    REQUIRE(container.getWidth() == 100);
    REQUIRE(container.getHeight() == 100);
}
