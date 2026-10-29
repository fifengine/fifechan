// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2026 Fifengine contributors

// Standard library includes
#include <algorithm>
#include <memory>
#include <utility>
#include <vector>

// Third-party library includes
#include <catch2/catch_test_macros.hpp>

// Project headers
#include "fifechan/color.hpp"
#include "fifechan/dragdrop.hpp"
#include "fifechan/events/dragevent.hpp"
#include "fifechan/graphics.hpp"
#include "fifechan/gui.hpp"
#include "fifechan/listeners/droptargetlistener.hpp"
#include "fifechan/point.hpp"
#include "fifechan/rectangle.hpp"
#include "fifechan/widgets/container.hpp"
#include "fifechan/widgets/label.hpp"

using fcn::Color;
using fcn::Container;
using fcn::DragEvent;
using fcn::DragHandler;
using fcn::DragPayload;
using fcn::DragRenderConfig;
using fcn::DropResult;
using fcn::Graphics;
using fcn::Gui;
using fcn::Image;
using fcn::Label;
using fcn::Point;
using fcn::PointVector;
using fcn::Rectangle;

namespace
{
    class RecordingDropTarget : public fcn::DropTargetListener
    {
        public:
            bool dragEntered(DragEvent& dragEvent) override
            {
                types.push_back(dragEvent.getType());
                return acceptEnter;
            }

            void dragExited(DragEvent& dragEvent) override
            {
                types.push_back(dragEvent.getType());
            }

            void dragHovered(DragEvent& dragEvent) override
            {
                types.push_back(dragEvent.getType());
            }

            bool dragDropped(DragEvent& dragEvent) override
            {
                types.push_back(dragEvent.getType());
                return acceptDrop;
            }

            bool acceptEnter{true};
            bool acceptDrop{true};
            std::vector<DragEvent::Type> types;
    };

    /**
     * Graphics implementation that records which draw operations happened.
     */
    class RecordingGraphics : public Graphics
    {
        public:
            void drawImage(Image const *, int, int, int, int, int, int) override
            {
            }
            void drawPoint(int, int) override
            {
            }
            void drawLine(int, int, int, int) override
            {
            }
            void drawLine(int, int, int, int, unsigned int) override
            {
            }
            void drawPolyLine(PointVector const &, unsigned int) override
            {
            }
            void drawBezier(PointVector const &, int, unsigned int) override
            {
            }
            void drawRectangle(Rectangle const & rectangle) override
            {
                ++drawRectangleCalls;
                lastRectangle = rectangle;
            }
            void fillRectangle(Rectangle const &) override
            {
                ++fillRectangleCalls;
            }
            void drawCircle(Point const &, unsigned int) override
            {
            }
            void drawFillCircle(Point const &, unsigned int) override
            {
            }
            void fillTriangle(int, int, int, int, int, int) override
            {
            }
            void drawCircleSegment(Point const &, unsigned int, int, int) override
            {
            }
            void drawFillCircleSegment(Point const &, unsigned int, int, int) override
            {
            }
            void setColor(Color const & color) override
            {
                mColor = color;
            }
            Color const & getColor() const override
            {
                return mColor;
            }

            int drawRectangleCalls{0};
            int fillRectangleCalls{0};
            Rectangle lastRectangle;
            Color mColor;
    };

    /**
     * Build a GUI whose top widget is a 200x200 container.
     *
     * `root` must be declared before the Gui so that the Gui is destroyed
     * while the widget tree is still alive.
     */
    void setTopContainer(Gui& gui, Container& root)
    {
        root.setPosition(0, 0);
        root.setSize(200, 200);
        gui.setTop(&root);
    }
} // namespace

TEST_CASE("DragPayload default construction", "[unit][dragdrop]")
{
    DragPayload payload;
    REQUIRE(!payload.hasData());
}

TEST_CASE("DragPayload with shared data", "[unit][dragdrop]")
{
    auto data = std::make_shared<int>(42);
    DragPayload payload(data);
    REQUIRE(payload.hasData());
    REQUIRE(*payload.getShared<int>() == 42);
}

TEST_CASE("DragPayload get raw pointer", "[unit][dragdrop]")
{
    auto data = std::make_shared<int>(99);
    DragPayload payload(data);
    REQUIRE(*payload.get<int>() == 99);
}

TEST_CASE("DragPayload setData", "[unit][dragdrop]")
{
    DragPayload payload;
    REQUIRE(!payload.hasData());

    auto data = std::make_shared<int>(55);
    payload.setData(data);
    REQUIRE(payload.hasData());
    REQUIRE(*payload.getShared<int>() == 55);
}

TEST_CASE("DragPayload copy", "[unit][dragdrop]")
{
    auto data = std::make_shared<int>(99);
    DragPayload payload(data);
    DragPayload const & copy(payload);
    REQUIRE(copy.hasData());
    REQUIRE(*copy.getShared<int>() == 99);
}

TEST_CASE("DragHandler initial state", "[unit][dragdrop]")
{
    DragHandler handler(nullptr);
    REQUIRE(handler.getState() == fcn::DragState::Idle);
    REQUIRE(handler.getHoveredWidget() == nullptr);
    REQUIRE(handler.getActiveDropTarget() == nullptr);
    REQUIRE(handler.getPayload() == nullptr);
    REQUIRE(handler.getSourceWidget() == nullptr);
}

TEST_CASE("DragHandler beginDrag returns false without payload", "[unit][dragdrop]")
{
    DragHandler handler(nullptr);
    Label source;
    bool const result = handler.beginDrag(&source, nullptr, 0, 0);
    REQUIRE(!result);
}

TEST_CASE("DragHandler beginDrag with valid payload", "[unit][dragdrop]")
{
    DragHandler handler(nullptr);
    Label source;
    auto payload      = std::make_unique<DragPayload>();
    bool const result = handler.beginDrag(&source, std::move(payload), 100, 200);
    REQUIRE(result);
    REQUIRE(handler.getState() == fcn::DragState::Dragging);
    REQUIRE(handler.getPayload() != nullptr);
    REQUIRE(handler.getSourceWidget() == &source);
}

TEST_CASE("DragHandler beginDrag twice returns false", "[unit][dragdrop]")
{
    DragHandler handler(nullptr);
    Label source;
    auto payload1 = std::make_unique<DragPayload>();
    REQUIRE(handler.beginDrag(&source, std::move(payload1), 0, 0));

    auto payload2 = std::make_unique<DragPayload>();
    REQUIRE(!handler.beginDrag(&source, std::move(payload2), 0, 0));
}

TEST_CASE("DragHandler cancel resets state", "[unit][dragdrop]")
{
    DragHandler handler(nullptr);
    Label source;
    auto payload = std::make_unique<DragPayload>();
    handler.beginDrag(&source, std::move(payload), 0, 0);
    REQUIRE(handler.getState() == fcn::DragState::Dragging);

    handler.cancel();
    REQUIRE(handler.getState() == fcn::DragState::Idle);
    REQUIRE(handler.getPayload() == nullptr);
    REQUIRE(handler.getSourceWidget() == nullptr);
    REQUIRE(handler.getActiveDropTarget() == nullptr);
}

TEST_CASE("DragHandler cancel when idle does nothing", "[unit][dragdrop]")
{
    DragHandler handler(nullptr);
    REQUIRE(handler.getState() == fcn::DragState::Idle);
    handler.cancel();
    REQUIRE(handler.getState() == fcn::DragState::Idle);
}

TEST_CASE("DragHandler drop while idle returns Cancelled", "[unit][dragdrop]")
{
    DragHandler handler(nullptr);
    DropResult const result = handler.drop(0, 0);
    REQUIRE(result == DropResult::Cancelled);
}

TEST_CASE("DragHandler setModalWidget and getModalWidget", "[unit][dragdrop]")
{
    DragHandler handler(nullptr);
    REQUIRE(handler.getModalWidget() == nullptr);

    Label modal;
    handler.setModalWidget(&modal);
    REQUIRE(handler.getModalWidget() == &modal);

    handler.setModalWidget(nullptr);
    REQUIRE(handler.getModalWidget() == nullptr);
}

TEST_CASE("DragHandler findWidgetAt with null root", "[unit][dragdrop]")
{
    REQUIRE(DragHandler::findWidgetAt(nullptr, 0, 0) == nullptr);
}

TEST_CASE("DragHandler update when idle does nothing", "[unit][dragdrop]")
{
    DragHandler handler(nullptr);
    REQUIRE(handler.getState() == fcn::DragState::Idle);
    handler.update(50, 50);
    REQUIRE(handler.getState() == fcn::DragState::Idle);
}

// ============================================================================
// Hit testing
// ============================================================================

TEST_CASE("DragHandler findWidgetAt returns the deepest child", "[unit][dragdrop]")
{
    Container root;
    Label child;
    root.setSize(200, 200);
    child.setPosition(10, 10);
    child.setSize(50, 50);
    root.add(&child);

    REQUIRE(DragHandler::findWidgetAt(&root, 20, 20) == &child);
    REQUIRE(DragHandler::findWidgetAt(&root, 150, 150) == &root);
}

TEST_CASE("DragHandler findWidgetAt rejects points outside the root", "[unit][dragdrop]")
{
    Container root;
    root.setSize(100, 100);

    REQUIRE(DragHandler::findWidgetAt(&root, -1, 10) == nullptr);
    REQUIRE(DragHandler::findWidgetAt(&root, 10, -1) == nullptr);
    REQUIRE(DragHandler::findWidgetAt(&root, 100, 10) == nullptr);
    REQUIRE(DragHandler::findWidgetAt(&root, 10, 100) == nullptr);
}

TEST_CASE("DragHandler findWidgetAt skips invisible and disabled children", "[unit][dragdrop]")
{
    Container root;
    Label hidden;
    Label disabled;
    root.setSize(200, 200);
    root.add(&hidden, 10, 10);
    hidden.setSize(50, 50);
    root.add(&disabled, 100, 100);
    disabled.setSize(50, 50);

    hidden.setVisible(false);
    disabled.setEnabled(false);

    REQUIRE(DragHandler::findWidgetAt(&root, 20, 20) == &root);
    REQUIRE(DragHandler::findWidgetAt(&root, 110, 110) == &root);
}

TEST_CASE("DragHandler findWidgetAt rejects invisible roots", "[unit][dragdrop]")
{
    Container root;
    root.setSize(100, 100);
    root.setVisible(false);

    REQUIRE(DragHandler::findWidgetAt(&root, 10, 10) == nullptr);
}

// ============================================================================
// Drag lifecycle
// ============================================================================

TEST_CASE("DragHandler enter, hover and drop reach the target", "[unit][dragdrop]")
{
    Container root;
    Label target;
    RecordingDropTarget listener;

    root.setSize(200, 200);
    root.add(&target, 10, 10);
    target.setSize(50, 50);
    target.addDropTargetListener(&listener);

    Gui gui;
    setTopContainer(gui, root);

    DragHandler handler(&gui);
    Label source;
    auto payload = std::make_unique<DragPayload>();
    REQUIRE(handler.beginDrag(&source, std::move(payload), 20, 20));

    handler.update(20, 20);
    REQUIRE(handler.getHoveredWidget() == &target);
    REQUIRE(handler.getActiveDropTarget() == &target);

    handler.update(25, 25);
    REQUIRE(handler.getActiveDropTarget() == &target);

    DropResult const result = handler.drop(25, 25);
    REQUIRE(result == DropResult::Accepted);
    REQUIRE(handler.getState() == fcn::DragState::Idle);
    REQUIRE(handler.getPayload() == nullptr);
    REQUIRE(handler.getSourceWidget() == nullptr);

    REQUIRE(!listener.types.empty());
    REQUIRE(listener.types.front() == DragEvent::Type::Enter);
    REQUIRE(std::find(listener.types.begin(), listener.types.end(), DragEvent::Type::Hover) != listener.types.end());
    REQUIRE(listener.types[listener.types.size() - 2] == DragEvent::Type::Drop);
    REQUIRE(listener.types.back() == DragEvent::Type::Leave);
}

TEST_CASE("DragHandler drop is rejected when the target declines", "[unit][dragdrop]")
{
    Container root;
    Label target;
    RecordingDropTarget listener;
    listener.acceptDrop = false;

    root.setSize(200, 200);
    root.add(&target, 10, 10);
    target.setSize(50, 50);
    target.addDropTargetListener(&listener);

    Gui gui;
    setTopContainer(gui, root);

    DragHandler handler(&gui);
    auto payload = std::make_unique<DragPayload>();
    REQUIRE(handler.beginDrag(nullptr, std::move(payload), 20, 20));

    DropResult const result = handler.drop(20, 20);
    REQUIRE(result == DropResult::Rejected);
    REQUIRE(handler.getState() == fcn::DragState::Idle);
    REQUIRE(std::find(listener.types.begin(), listener.types.end(), DragEvent::Type::Drop) != listener.types.end());
    REQUIRE(listener.types.back() == DragEvent::Type::Leave);
}

TEST_CASE("DragHandler drop with no accepting target returns Cancelled", "[unit][dragdrop]")
{
    Container root;
    Label target;

    root.setSize(200, 200);
    root.add(&target, 10, 10);
    target.setSize(50, 50);

    Gui gui;
    setTopContainer(gui, root);

    DragHandler handler(&gui);
    auto payload = std::make_unique<DragPayload>();
    REQUIRE(handler.beginDrag(nullptr, std::move(payload), 20, 20));

    REQUIRE(handler.drop(20, 20) == DropResult::Cancelled);
}

TEST_CASE("DragHandler sends leave when the pointer leaves the target", "[unit][dragdrop]")
{
    Container root;
    Label target;
    RecordingDropTarget listener;

    root.setSize(200, 200);
    root.add(&target, 10, 10);
    target.setSize(50, 50);
    target.addDropTargetListener(&listener);

    Gui gui;
    setTopContainer(gui, root);

    DragHandler handler(&gui);
    auto payload = std::make_unique<DragPayload>();
    REQUIRE(handler.beginDrag(nullptr, std::move(payload), 20, 20));

    handler.update(20, 20);
    REQUIRE(handler.getActiveDropTarget() == &target);

    handler.update(150, 150);
    REQUIRE(handler.getActiveDropTarget() == nullptr);
    REQUIRE(listener.types.back() == DragEvent::Type::Leave);
}

TEST_CASE("DragHandler cancel sends leave to the active target", "[unit][dragdrop]")
{
    Container root;
    Label target;
    RecordingDropTarget listener;

    root.setSize(200, 200);
    root.add(&target, 10, 10);
    target.setSize(50, 50);
    target.addDropTargetListener(&listener);

    Gui gui;
    setTopContainer(gui, root);

    DragHandler handler(&gui);
    auto payload = std::make_unique<DragPayload>();
    REQUIRE(handler.beginDrag(nullptr, std::move(payload), 20, 20));
    handler.update(20, 20);

    handler.cancel();
    REQUIRE(handler.getActiveDropTarget() == nullptr);
    REQUIRE(listener.types.back() == DragEvent::Type::Leave);
}

TEST_CASE("DragHandler targets an ancestor when the child does not accept", "[unit][dragdrop]")
{
    Container root;
    Label child;
    RecordingDropTarget rootListener;

    root.setSize(200, 200);
    root.add(&child, 10, 10);
    child.setSize(50, 50);
    root.addDropTargetListener(&rootListener);

    Gui gui;
    setTopContainer(gui, root);

    DragHandler handler(&gui);
    auto payload = std::make_unique<DragPayload>();
    REQUIRE(handler.beginDrag(nullptr, std::move(payload), 20, 20));
    handler.update(20, 20);

    REQUIRE(handler.getHoveredWidget() == &child);
    REQUIRE(handler.getActiveDropTarget() == &root);
}

TEST_CASE("DragHandler confines targets to the modal widget", "[unit][dragdrop]")
{
    Container root;
    Label outside;
    Container modal;
    RecordingDropTarget modalListener;

    root.setSize(200, 200);
    root.add(&outside, 10, 10);
    outside.setSize(50, 50);
    root.add(&modal, 100, 100);
    modal.setSize(50, 50);
    modal.addDropTargetListener(&modalListener);

    Gui gui;
    setTopContainer(gui, root);

    DragHandler handler(&gui);
    handler.setModalWidget(&modal);

    auto payload = std::make_unique<DragPayload>();
    REQUIRE(handler.beginDrag(nullptr, std::move(payload), 20, 20));
    handler.update(20, 20);

    REQUIRE(handler.getHoveredWidget() == nullptr);
    REQUIRE(handler.getActiveDropTarget() == nullptr);

    handler.update(110, 110);
    REQUIRE(handler.getHoveredWidget() == &modal);
    REQUIRE(handler.getActiveDropTarget() == &modal);
}

// ============================================================================
// Rendering
// ============================================================================

TEST_CASE("DragRenderConfig defaultHighlight draws a rectangle", "[unit][dragdrop]")
{
    auto const highlight = DragRenderConfig::defaultHighlight();
    REQUIRE(highlight);

    Label target;
    target.setPosition(10, 20);
    target.setSize(30, 40);

    RecordingGraphics graphics;
    highlight(graphics, &target, true);

    REQUIRE(graphics.drawRectangleCalls == 1);
    REQUIRE(graphics.lastRectangle.x == 10);
    REQUIRE(graphics.lastRectangle.y == 20);
    REQUIRE(graphics.lastRectangle.width == 30);
    REQUIRE(graphics.lastRectangle.height == 40);
    REQUIRE(graphics.getColor().a == 220);
}

TEST_CASE("DragHandler render uses the configured ghost and highlight", "[unit][dragdrop]")
{
    Container root;
    Label target;
    RecordingDropTarget listener;

    root.setSize(200, 200);
    root.add(&target, 10, 10);
    target.setSize(50, 50);
    target.addDropTargetListener(&listener);

    Gui gui;
    setTopContainer(gui, root);

    DragHandler handler(&gui);
    auto payload = std::make_unique<DragPayload>();
    REQUIRE(handler.beginDrag(nullptr, std::move(payload), 20, 20));
    handler.update(20, 20);

    int ghostCalls     = 0;
    int highlightCalls = 0;
    DragRenderConfig config;
    config.ghost = [&ghostCalls](Graphics&, DragPayload const &, fcn::DragPoint const &) {
        ++ghostCalls;
    };
    config.highlight = [&highlightCalls](Graphics&, fcn::Widget*, bool) {
        ++highlightCalls;
    };
    handler.setRenderConfig(config);

    RecordingGraphics graphics;
    handler.render(graphics);

    REQUIRE(ghostCalls == 1);
    REQUIRE(highlightCalls == 1);
}

TEST_CASE("DragHandler render falls back to the payload ghost renderer", "[unit][dragdrop]")
{
    Gui gui;
    DragHandler handler(&gui);

    int ghostCalls = 0;
    auto payload   = std::make_unique<DragPayload>();
    payload->setGhostRenderer([&ghostCalls](Graphics&, fcn::DragPoint const &) {
        ++ghostCalls;
    });
    REQUIRE(handler.beginDrag(nullptr, std::move(payload), 0, 0));

    RecordingGraphics graphics;
    handler.render(graphics);

    REQUIRE(ghostCalls == 1);
}

TEST_CASE("DragHandler render does nothing while idle", "[unit][dragdrop]")
{
    Gui gui;
    DragHandler handler(&gui);

    int calls = 0;
    DragRenderConfig config;
    config.ghost = [&calls](Graphics&, DragPayload const &, fcn::DragPoint const &) {
        ++calls;
    };
    config.highlight = [&calls](Graphics&, fcn::Widget*, bool) {
        ++calls;
    };
    handler.setRenderConfig(config);

    RecordingGraphics graphics;
    handler.render(graphics);

    REQUIRE(calls == 0);
}

// ============================================================================
// Linkage guard
// ============================================================================

TEST_CASE("DragHandler public API is linkable", "[unit][dragdrop][link]")
{
    Container root;
    Label target;
    root.setSize(100, 100);
    root.add(&target, 0, 0);
    target.setSize(50, 50);

    Gui gui;
    setTopContainer(gui, root);

    DragHandler handler(&gui);
    handler.setModalWidget(nullptr);
    REQUIRE(handler.getModalWidget() == nullptr);
    REQUIRE(handler.getState() == fcn::DragState::Idle);
    REQUIRE(handler.getHoveredWidget() == nullptr);
    REQUIRE(handler.getActiveDropTarget() == nullptr);
    REQUIRE(handler.getPayload() == nullptr);
    REQUIRE(handler.getSourceWidget() == nullptr);

    auto payload = std::make_unique<DragPayload>();
    payload->setTooltip("tip");
    REQUIRE(payload->getTooltip() == "tip");
    payload->setGhostRenderer([](Graphics&, fcn::DragPoint const &) { });
    REQUIRE(static_cast<bool>(payload->getGhostRenderer()));

    REQUIRE(handler.beginDrag(&target, std::move(payload), 10, 10));
    handler.update(10, 10);
    handler.cancel();

    DragRenderConfig config;
    handler.setRenderConfig(config);
    REQUIRE(!handler.getRenderConfig().ghost);
    REQUIRE(static_cast<bool>(DragRenderConfig::defaultHighlight()));

    auto ghostPayload = std::make_unique<DragPayload>();
    REQUIRE(handler.beginDrag(nullptr, std::move(ghostPayload), 0, 0));
    RecordingGraphics graphics;
    handler.render(graphics);
    REQUIRE(handler.drop(0, 0) == DropResult::Cancelled);
}
