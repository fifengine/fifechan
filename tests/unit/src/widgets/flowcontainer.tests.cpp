// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2026 Fifengine contributors

// Standard library includes
#include <memory>
#include <string>

// Third-party library includes
#include <catch2/catch_test_macros.hpp>

// Project headers
#include "fifechan/defaultfont.hpp"
#include "fifechan/graphics.hpp"
#include "fifechan/widgets/flowcontainer.hpp"
#include "fifechan/widgets/label.hpp"

using fcn::Container;
using fcn::DefaultFont;
using fcn::FlowContainer;
using fcn::Graphics;
using fcn::Label;

TEST_CASE("FlowContainer default constructor", "[unit][flowcontainer]")
{
    FlowContainer fc;
    REQUIRE(fc.getChildrenCount() == 0);
    REQUIRE(fc.isOpaque());
}

TEST_CASE("FlowContainer add and count children", "[unit][flowcontainer]")
{
    FlowContainer fc;
    Label l1;
    Label l2;

    fc.add(&l1);
    REQUIRE(fc.getChildrenCount() == 1);

    fc.add(&l2);
    REQUIRE(fc.getChildrenCount() == 2);
}

TEST_CASE("FlowContainer remove child", "[unit][flowcontainer]")
{
    FlowContainer fc;
    Label l1;
    Label l2;

    fc.add(&l1);
    fc.add(&l2);
    REQUIRE(fc.getChildrenCount() == 2);

    fc.remove(&l1);
    REQUIRE(fc.getChildrenCount() == 1);
    REQUIRE(l1.getParent() == nullptr);
}

TEST_CASE("FlowContainer removeAllChildren", "[unit][flowcontainer]")
{
    FlowContainer fc;
    Label l1;
    Label l2;

    fc.add(&l1);
    fc.add(&l2);
    fc.removeAllChildren();
    REQUIRE(fc.getChildrenCount() == 0);
    REQUIRE(l1.getParent() == nullptr);
    REQUIRE(l2.getParent() == nullptr);
}

TEST_CASE("FlowContainer setOpaque and isOpaque", "[unit][flowcontainer]")
{
    FlowContainer fc;
    REQUIRE(fc.isOpaque());

    fc.setOpaque(false);
    REQUIRE_FALSE(fc.isOpaque());

    fc.setOpaque(true);
    REQUIRE(fc.isOpaque());
}

TEST_CASE("FlowContainer setSize then getWidth/Height", "[unit][flowcontainer]")
{
    FlowContainer fc;
    fc.setSize(300, 200);
    REQUIRE(fc.getWidth() == 300);
    REQUIRE(fc.getHeight() == 200);
}

TEST_CASE("FlowContainer adjustSize does not crash", "[unit][flowcontainer]")
{
    FlowContainer fc;
    fc.adjustSize();
    REQUIRE(true);
}

TEST_CASE("FlowContainer getChildrenArea with border and padding", "[unit][flowcontainer]")
{
    FlowContainer fc;
    fc.setBorderSize(2);
    fc.setPadding(4);
    fc.setSize(200, 100);

    auto area = fc.getChildrenArea();
    REQUIRE(area.x >= 0);
    REQUIRE(area.y >= 0);
    REQUIRE(area.width <= fc.getWidth());
    REQUIRE(area.height <= fc.getHeight());
}
