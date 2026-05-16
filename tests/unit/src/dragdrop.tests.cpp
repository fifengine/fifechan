// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2026 Fifengine contributors

// Third-party library includes
#include <catch2/catch_test_macros.hpp>

// Project headers
#include "fifechan/dragdrop.hpp"
#include "fifechan/events/dragevent.hpp"
#include "fifechan/listeners/droptargetlistener.hpp"
#include "fifechan/widgets/container.hpp"
#include "fifechan/widgets/label.hpp"

using fcn::Container;
using fcn::DragEvent;
using fcn::DragHandler;
using fcn::DragPayload;
using fcn::DropResult;
using fcn::Label;

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
    DragPayload copy(payload);
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

TEST_CASE("DragHandler beginDragFromWidget returns false", "[unit][dragdrop]")
{
    DragHandler handler(nullptr);
    Label source;
    REQUIRE(!handler.beginDragFromWidget(&source, 0, 0));
}

TEST_CASE("DragHandler findWidgetAt with null root", "[unit][dragdrop]")
{
    DragHandler handler(nullptr);
    REQUIRE(handler.findWidgetAt(nullptr, 0, 0) == nullptr);
}

TEST_CASE("DragHandler update when idle does nothing", "[unit][dragdrop]")
{
    DragHandler handler(nullptr);
    REQUIRE(handler.getState() == fcn::DragState::Idle);
    handler.update(50, 50);
    REQUIRE(handler.getState() == fcn::DragState::Idle);
}
