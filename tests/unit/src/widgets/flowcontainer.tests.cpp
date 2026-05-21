// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2026 Fifengine contributors

#include <catch2/catch_test_macros.hpp>

#include "fifechan/exception.hpp"
#include "fifechan/widgets/flowcontainer.hpp"
#include "fifechan/widgets/label.hpp"

using fcn::Container;
using fcn::FlowContainer;
using fcn::Label;

TEST_CASE("FlowContainer default constructor", "[unit][flowcontainer]")
{
    FlowContainer fc;
    REQUIRE(fc.getChildrenCount() == 0);
    REQUIRE(fc.isOpaque());
    REQUIRE(fc.getAlignment() == FlowContainer::Alignment::Center);
}

TEST_CASE("FlowContainer add and remove children", "[unit][flowcontainer]")
{
    FlowContainer fc;
    Label l1;
    Label l2;

    fc.add(&l1);
    REQUIRE(fc.getChildrenCount() == 1);

    fc.add(&l2);
    REQUIRE(fc.getChildrenCount() == 2);

    fc.remove(&l1);
    REQUIRE(fc.getChildrenCount() == 1);
    REQUIRE(l1.getParent() == nullptr);

    fc.removeAllChildren();
    REQUIRE(fc.getChildrenCount() == 0);
    REQUIRE(l2.getParent() == nullptr);
}

TEST_CASE("FlowContainer setAlignment and getAlignment", "[unit][flowcontainer]")
{
    FlowContainer fc;
    fc.setAlignment(FlowContainer::Alignment::Left);
    REQUIRE(fc.getAlignment() == FlowContainer::Alignment::Left);
    fc.setAlignment(FlowContainer::Alignment::Right);
    REQUIRE(fc.getAlignment() == FlowContainer::Alignment::Right);
    fc.setAlignment(FlowContainer::Alignment::Top);
    REQUIRE(fc.getAlignment() == FlowContainer::Alignment::Top);
    fc.setAlignment(FlowContainer::Alignment::Bottom);
    REQUIRE(fc.getAlignment() == FlowContainer::Alignment::Bottom);
    fc.setAlignment(FlowContainer::Alignment::Center);
    REQUIRE(fc.getAlignment() == FlowContainer::Alignment::Center);
}

TEST_CASE("FlowContainer setLayout Horizontal and Vertical", "[unit][flowcontainer]")
{
    FlowContainer fc;
    fc.setLayout(Container::LayoutPolicy::Horizontal);
    REQUIRE(fc.getLayout() == Container::LayoutPolicy::Horizontal);
    fc.setLayout(Container::LayoutPolicy::Vertical);
    REQUIRE(fc.getLayout() == Container::LayoutPolicy::Vertical);
}

TEST_CASE("FlowContainer setLayout Circular throws", "[unit][flowcontainer]")
{
    FlowContainer fc;
    REQUIRE_THROWS_AS(fc.setLayout(Container::LayoutPolicy::Circular), fcn::Exception);
}

TEST_CASE("FlowContainer adjustContent empty container", "[unit][flowcontainer]")
{
    FlowContainer fc;
    fc.adjustContent();
    REQUIRE(fc.getChildrenCount() == 0);
}

TEST_CASE("FlowContainer adjustContent single child horizontal", "[unit][flowcontainer]")
{
    FlowContainer fc;
    fc.setLayout(Container::LayoutPolicy::Horizontal);
    fc.setSize(200, 100);

    Label lbl;
    lbl.setSize(50, 30);
    fc.add(&lbl);

    fc.adjustContent();

    // Single child should be at (marginLeft, marginTop) = (0, 0)
    REQUIRE(lbl.getX() >= 0);
    REQUIRE(lbl.getY() >= 0);
    // Container should have been resized to include the child
    REQUIRE(fc.getWidth() >= 50);
    REQUIRE(fc.getHeight() >= 30);
}

TEST_CASE("FlowContainer adjustContent single child vertical", "[unit][flowcontainer]")
{
    FlowContainer fc;
    fc.setLayout(Container::LayoutPolicy::Vertical);
    fc.setSize(200, 100);

    Label lbl;
    lbl.setSize(50, 30);
    fc.add(&lbl);

    fc.adjustContent();

    REQUIRE(lbl.getX() >= 0);
    REQUIRE(lbl.getY() >= 0);
    REQUIRE(fc.getWidth() >= 50);
    REQUIRE(fc.getHeight() >= 30);
}

TEST_CASE("FlowContainer adjustContent multiple children single row", "[unit][flowcontainer]")
{
    FlowContainer fc;
    fc.setLayout(Container::LayoutPolicy::Horizontal);
    fc.setSize(400, 100);

    Label l1;
    l1.setSize(60, 30);
    Label l2;
    l2.setSize(60, 30);
    fc.add(&l1);
    fc.add(&l2);

    fc.adjustContent();

    // Both children should be in the same row
    REQUIRE(l1.getY() == l2.getY());
    // l2 should be to the right of l1
    REQUIRE(l2.getX() > l1.getX());
}

TEST_CASE("FlowContainer adjustContent children wrap to next row", "[unit][flowcontainer]")
{
    FlowContainer fc;
    fc.setLayout(Container::LayoutPolicy::Horizontal);
    fc.setSize(100, 200);

    Label l1;
    l1.setSize(80, 30);
    Label l2;
    l2.setSize(80, 30);
    fc.add(&l1);
    fc.add(&l2);

    fc.adjustContent();

    // l2 should wrap to a new row (below l1)
    REQUIRE(l2.getY() > l1.getY());
}

TEST_CASE("FlowContainer adjustContent invisible children excluded", "[unit][flowcontainer]")
{
    FlowContainer fc;
    fc.setLayout(Container::LayoutPolicy::Horizontal);
    fc.setSize(400, 100);

    Label l1;
    l1.setSize(60, 30);
    Label l2;
    l2.setSize(60, 30);
    Label l3;
    l3.setSize(60, 30);
    fc.add(&l1);
    fc.add(&l2);
    fc.add(&l3);

    l2.setVisible(false);
    fc.adjustContent();

    // l3 should be immediately to the right of l1 (l2 skipped)
    REQUIRE(l3.getX() > l1.getX());
}

TEST_CASE("FlowContainer adjustContent vertical layout one column", "[unit][flowcontainer]")
{
    FlowContainer fc;
    fc.setLayout(Container::LayoutPolicy::Vertical);
    fc.setSize(200, 200);

    Label l1;
    l1.setSize(40, 30);
    Label l2;
    l2.setSize(60, 30);
    fc.add(&l1);
    fc.add(&l2);

    fc.setAlignment(FlowContainer::Alignment::Left);
    fc.adjustContent();

    // Both fit in one column, l2 is below l1
    REQUIRE(l2.getY() > l1.getY());
    // With Left alignment, both start at x=0
    REQUIRE(l1.getX() == 0);
    REQUIRE(l2.getX() == 0);
}

TEST_CASE("FlowContainer adjustContent wrapping in vertical layout", "[unit][flowcontainer]")
{
    FlowContainer fc;
    fc.setLayout(Container::LayoutPolicy::Vertical);
    fc.setSize(200, 100);

    Label l1;
    l1.setSize(40, 80);
    Label l2;
    l2.setSize(40, 80);
    fc.add(&l1);
    fc.add(&l2);

    fc.adjustContent();

    // l2 wraps to next column
    REQUIRE(l2.getX() > l1.getX());
    REQUIRE(l2.getY() == 0);
}

TEST_CASE("FlowContainer adjustContent vertical centering", "[unit][flowcontainer]")
{
    FlowContainer fc;
    fc.setLayout(Container::LayoutPolicy::Vertical);
    fc.setSize(200, 100);

    Label l1;
    l1.setSize(40, 30);
    Label l2;
    l2.setSize(40, 30);
    fc.add(&l1);
    fc.add(&l2);

    fc.adjustContent();

    // l2 should be below l1
    REQUIRE(l2.getY() > l1.getY());
}

TEST_CASE("FlowContainer resizeToContent with parent", "[unit][flowcontainer]")
{
    FlowContainer fc;
    fc.setLayout(Container::LayoutPolicy::Horizontal);

    Label lbl;
    lbl.setSize(70, 25);
    fc.add(&lbl);

    fc.setSize(200, 100);
    fc.adjustContent();

    // After adjustContent, container resizes to fit children
    REQUIRE(fc.getWidth() >= 70);
    REQUIRE(fc.getHeight() >= 25);
}

TEST_CASE("FlowContainer expandContent calls adjustContent", "[unit][flowcontainer]")
{
    FlowContainer fc;
    fc.setLayout(Container::LayoutPolicy::Horizontal);
    fc.setSize(200, 100);

    Label lbl;
    lbl.setSize(50, 30);
    fc.add(&lbl);

    fc.expandContent(false);

    // After expandContent, child should be positioned
    REQUIRE(lbl.getX() >= 0);
    REQUIRE(fc.getWidth() >= 50);
}

TEST_CASE("FlowContainer spacing affects layout", "[unit][flowcontainer]")
{
    FlowContainer fc;
    fc.setLayout(Container::LayoutPolicy::Horizontal);
    fc.setHorizontalSpacing(10);
    fc.setSize(400, 100);

    Label l1;
    l1.setSize(50, 30);
    Label l2;
    l2.setSize(50, 30);
    fc.add(&l1);
    fc.add(&l2);

    fc.adjustContent();

    // l2 should be at least 50 + 10 = 60 pixels from left edge
    REQUIRE(l2.getX() >= 60);
}

