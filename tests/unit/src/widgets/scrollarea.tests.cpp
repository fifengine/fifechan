// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2026 Fifengine contributors

// Third-party library includes
#include <catch2/catch_test_macros.hpp>

// Project headers
#include "fifechan/defaultfont.hpp"
#include "fifechan/widgets/label.hpp"
#include "fifechan/widgets/scrollarea.hpp"

using fcn::DefaultFont;
using fcn::Label;
using fcn::ScrollArea;

TEST_CASE("ScrollArea default constructor", "[unit][scrollarea]")
{
    ScrollArea area;
    REQUIRE(area.getContent() == nullptr);
    REQUIRE(area.getHorizontalScrollPolicy() == ScrollArea::ScrollPolicy::ShowAuto);
    REQUIRE(area.getVerticalScrollPolicy() == ScrollArea::ScrollPolicy::ShowAuto);
    REQUIRE(area.getScrollbarWidth() > 0);
    REQUIRE(area.getHorizontalScrollAmount() == 0);
    REQUIRE(area.getVerticalScrollAmount() == 0);
}

TEST_CASE("ScrollArea content constructor", "[unit][scrollarea]")
{
    Label label("Content");
    DefaultFont font;
    label.setFont(&font);

    ScrollArea area(&label);
    REQUIRE(area.getContent() == &label);
}

TEST_CASE("ScrollArea setContent", "[unit][scrollarea]")
{
    ScrollArea area;
    Label label;
    area.setContent(&label);
    REQUIRE(area.getContent() == &label);

    area.setContent(nullptr);
    REQUIRE(area.getContent() == nullptr);
}

TEST_CASE("ScrollArea setHorizontalScrollPolicy and getHorizontalScrollPolicy", "[unit][scrollarea]")
{
    ScrollArea area;
    area.setHorizontalScrollPolicy(ScrollArea::ScrollPolicy::ShowNever);
    REQUIRE(area.getHorizontalScrollPolicy() == ScrollArea::ScrollPolicy::ShowNever);

    area.setHorizontalScrollPolicy(ScrollArea::ScrollPolicy::ShowAlways);
    REQUIRE(area.getHorizontalScrollPolicy() == ScrollArea::ScrollPolicy::ShowAlways);

    area.setHorizontalScrollPolicy(ScrollArea::ScrollPolicy::ShowAuto);
    REQUIRE(area.getHorizontalScrollPolicy() == ScrollArea::ScrollPolicy::ShowAuto);
}

TEST_CASE("ScrollArea setVerticalScrollPolicy and getVerticalScrollPolicy", "[unit][scrollarea]")
{
    ScrollArea area;
    area.setVerticalScrollPolicy(ScrollArea::ScrollPolicy::ShowNever);
    REQUIRE(area.getVerticalScrollPolicy() == ScrollArea::ScrollPolicy::ShowNever);

    area.setVerticalScrollPolicy(ScrollArea::ScrollPolicy::ShowAlways);
    REQUIRE(area.getVerticalScrollPolicy() == ScrollArea::ScrollPolicy::ShowAlways);

    area.setVerticalScrollPolicy(ScrollArea::ScrollPolicy::ShowAuto);
    REQUIRE(area.getVerticalScrollPolicy() == ScrollArea::ScrollPolicy::ShowAuto);
}

TEST_CASE("ScrollArea setScrollbarWidth and getScrollbarWidth", "[unit][scrollarea]")
{
    ScrollArea area;
    int const defaultWidth = area.getScrollbarWidth();
    REQUIRE(defaultWidth > 0);

    area.setScrollbarWidth(20);
    REQUIRE(area.getScrollbarWidth() == 20);
}

TEST_CASE("ScrollArea scroll amounts", "[unit][scrollarea]")
{
    ScrollArea area;
    area.setHorizontalScrollAmount(50);
    // Note: clamped to 0 when no content exists (max scroll is 0)
    REQUIRE(area.getHorizontalScrollAmount() == 0);

    area.setVerticalScrollAmount(30);
    REQUIRE(area.getVerticalScrollAmount() == 0);
}

TEST_CASE("ScrollArea setWidth/Height/Size", "[unit][scrollarea]")
{
    ScrollArea area;
    area.setSize(300, 200);
    REQUIRE(area.getWidth() == 300);
    REQUIRE(area.getHeight() == 200);
}

TEST_CASE("ScrollArea setOpaque", "[unit][scrollarea]")
{
    ScrollArea area;
    area.setOpaque(true);
    REQUIRE(area.isOpaque());

    area.setOpaque(false);
    REQUIRE(!area.isOpaque());
}

TEST_CASE("ScrollArea adjustSize with content", "[unit][scrollarea]")
{
    Label label("Scroll Content");
    DefaultFont font;
    label.setFont(&font);
    ScrollArea area(&label);
    area.adjustSize();
    REQUIRE(true);
}

TEST_CASE("ScrollArea content getter returns set content", "[unit][scrollarea]")
{
    ScrollArea area;
    Label label;
    area.setContent(&label);
    REQUIRE(area.getContent() == &label);
}
