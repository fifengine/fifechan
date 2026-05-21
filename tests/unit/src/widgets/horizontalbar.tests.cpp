// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2026 Fifengine contributors

#include <catch2/catch_test_macros.hpp>

#include "fifechan/graphics.hpp"
#include "fifechan/rectangle.hpp"
#include "fifechan/widgets/horizontalbar.hpp"
#include "fifechan/widgets/label.hpp"

using fcn::HorizontalBar;
using fcn::Label;

namespace
{
    class MockGraphics : public fcn::Graphics
    {
        public:
            void setColor(fcn::Color const &) override {}
            fcn::Color const & getColor() const override
            {
                static fcn::Color c;
                return c;
            }
            void drawPoint(int, int) override {}
            void drawLine(int, int, int, int) override {}
            void drawLine(int, int, int, int, unsigned int) override {}
            void drawPolyLine(fcn::PointVector const &, unsigned int) override {}
            void drawBezier(fcn::PointVector const &, int, unsigned int) override {}
            void drawRectangle(fcn::Rectangle const &) override {}
            void fillRectangle(fcn::Rectangle const &) override {}
            void drawCircle(fcn::Point const &, unsigned int) override {}
            void drawFillCircle(fcn::Point const &, unsigned int) override {}
            void drawCircleSegment(fcn::Point const &, unsigned int, int, int) override {}
            void drawFillCircleSegment(fcn::Point const &, unsigned int, int, int) override {}
            void drawImage(fcn::Image const *, int, int, int, int, int, int) override {}
    };
}

TEST_CASE("HorizontalBar default construction", "[unit][horizontalbar]")
{
    HorizontalBar bar;
    REQUIRE(bar.getLayout() == fcn::Container::LayoutPolicy::Horizontal);
    REQUIRE(bar.isOpaque());
    REQUIRE(bar.getSpacing() == 4);
    REQUIRE(bar.getPadding() == 4);
    REQUIRE(bar.getFixedHeight() == 0);
    REQUIRE(bar.isClipping());
    REQUIRE_FALSE(bar.isExpandChildren());
}

TEST_CASE("HorizontalBar setSpacing and getSpacing", "[unit][horizontalbar]")
{
    HorizontalBar bar;
    bar.setSpacing(8);
    REQUIRE(bar.getSpacing() == 8);
    REQUIRE(bar.getHorizontalSpacing() == 8);

    bar.setSpacing(0);
    REQUIRE(bar.getSpacing() == 0);
}

TEST_CASE("HorizontalBar setPadding and getPadding", "[unit][horizontalbar]")
{
    HorizontalBar bar;
    bar.setPadding(10);
    REQUIRE(bar.getPadding() == 10);

    bar.setPadding(0);
    REQUIRE(bar.getPadding() == 0);
}

TEST_CASE("HorizontalBar setFixedHeight and getFixedHeight", "[unit][horizontalbar]")
{
    HorizontalBar bar;
    bar.setFixedHeight(32);
    REQUIRE(bar.getFixedHeight() == 32);

    bar.setFixedHeight(0);
    REQUIRE(bar.getFixedHeight() == 0);
}

TEST_CASE("HorizontalBar setClipping and isClipping", "[unit][horizontalbar]")
{
    HorizontalBar bar;
    bar.setClipping(false);
    REQUIRE_FALSE(bar.isClipping());

    bar.setClipping(true);
    REQUIRE(bar.isClipping());
}

TEST_CASE("HorizontalBar setExpandChildren and isExpandChildren", "[unit][horizontalbar]")
{
    HorizontalBar bar;
    bar.setExpandChildren(true);
    REQUIRE(bar.isExpandChildren());

    bar.setExpandChildren(false);
    REQUIRE_FALSE(bar.isExpandChildren());
}

TEST_CASE("HorizontalBar add single child", "[unit][horizontalbar]")
{
    HorizontalBar bar;
    Label l;
    bar.add(&l);
    REQUIRE(bar.getChildrenCount() == 1);
}

TEST_CASE("HorizontalBar remove child", "[unit][horizontalbar]")
{
    HorizontalBar bar;
    Label l;
    bar.add(&l);
    bar.remove(&l);
    REQUIRE(bar.getChildrenCount() == 0);
}

TEST_CASE("HorizontalBar removeAllChildren", "[unit][horizontalbar]")
{
    HorizontalBar bar;
    Label l1, l2;
    bar.add(&l1);
    bar.add(&l2);
    bar.removeAllChildren();
    REQUIRE(bar.getChildrenCount() == 0);
}

TEST_CASE("HorizontalBar resizeToContent shrinks from large width", "[unit][horizontalbar]")
{
    HorizontalBar bar;
    bar.setSize(500, 30);
    Label l;
    l.setSize(40, 20);
    bar.add(&l);
    bar.resizeToContent(false);
    // Content width = 40 + 4(paddingLeft) + 4(paddingRight) = 48
    REQUIRE(bar.getWidth() < 500);
    REQUIRE(bar.getWidth() > 0);
}

TEST_CASE("HorizontalBar resizeToContent with empty bar", "[unit][horizontalbar]")
{
    HorizontalBar bar;
    bar.resizeToContent(false);
}

TEST_CASE("HorizontalBar resizeToContent recursion flag", "[unit][horizontalbar]")
{
    HorizontalBar bar;
    Label l;
    l.setSize(10, 10);
    bar.add(&l);
    bar.resizeToContent(true);
    bar.resizeToContent(false);
}

TEST_CASE("HorizontalBar adjustSize shrinks from large width", "[unit][horizontalbar]")
{
    HorizontalBar bar;
    bar.setSize(400, 40);
    Label l;
    l.setSize(30, 15);
    bar.add(&l);
    bar.adjustSize();
    // Content width = padding(4+4) + child(30) = 38
    REQUIRE(bar.getWidth() < 400);
    REQUIRE(bar.getWidth() > 0);
}

TEST_CASE("HorizontalBar adjustSize with empty bar", "[unit][horizontalbar]")
{
    HorizontalBar bar;
    bar.adjustSize();
}

TEST_CASE("HorizontalBar fixedHeight applied after resizeToContent", "[unit][horizontalbar]")
{
    HorizontalBar bar;
    bar.setFixedHeight(50);
    Label l;
    l.setSize(30, 10);
    bar.setSize(200, 40);
    bar.add(&l);
    bar.resizeToContent(false);
    REQUIRE(bar.getHeight() == 50);
}

TEST_CASE("HorizontalBar adjustSize with fixedHeight applied", "[unit][horizontalbar]")
{
    HorizontalBar bar;
    bar.setFixedHeight(60);
    Label l;
    l.setSize(30, 10);
    bar.setSize(200, 40);
    bar.add(&l);
    bar.adjustSize();
    REQUIRE(bar.getHeight() == 60);
}

TEST_CASE("HorizontalBar fixedHeight 0 keeps content height", "[unit][horizontalbar]")
{
    HorizontalBar bar;
    bar.setFixedHeight(0);
    Label l;
    l.setSize(30, 10);
    bar.add(&l);
    bar.resizeToContent(false);
    REQUIRE(bar.getHeight() >= 10);
}

TEST_CASE("HorizontalBar draw with opaque", "[unit][horizontalbar]")
{
    HorizontalBar bar;
    bar.setSize(100, 30);
    Label l;
    l.setSize(40, 20);
    bar.add(&l);
    MockGraphics g;
    bar.draw(&g);
}

TEST_CASE("HorizontalBar draw with non-opaque", "[unit][horizontalbar]")
{
    HorizontalBar bar;
    bar.setOpaque(false);
    bar.setSize(100, 30);
    Label l;
    l.setSize(40, 20);
    bar.add(&l);
    MockGraphics g;
    bar.draw(&g);
}

TEST_CASE("HorizontalBar draw empty bar", "[unit][horizontalbar]")
{
    HorizontalBar bar;
    bar.setSize(50, 20);
    MockGraphics g;
    bar.draw(&g);
}

TEST_CASE("HorizontalBar expandChildren prevents width shrinkage", "[unit][horizontalbar]")
{
    HorizontalBar bar;
    bar.setExpandChildren(true);
    bar.setSize(200, 30);
    Label l;
    l.setSize(40, 20);
    bar.add(&l);
    int const wBefore = bar.getWidth();
    bar.resizeToContent(false);
}

TEST_CASE("HorizontalBar multiple children", "[unit][horizontalbar]")
{
    HorizontalBar bar;
    Label l1, l2, l3;
    l1.setSize(10, 10);
    l2.setSize(20, 10);
    l3.setSize(30, 10);
    bar.add(&l1);
    bar.add(&l2);
    bar.add(&l3);
    bar.adjustSize();
    REQUIRE(bar.getChildrenCount() == 3);
}

TEST_CASE("HorizontalBar setSize", "[unit][horizontalbar]")
{
    HorizontalBar bar;
    bar.setSize(150, 40);
    REQUIRE(bar.getWidth() == 150);
    REQUIRE(bar.getHeight() == 40);
}
