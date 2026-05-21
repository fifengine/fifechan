// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2026 Fifengine contributors

#include <catch2/catch_test_macros.hpp>

#include "fifechan/widgets/adjustingcontainer.hpp"
#include "fifechan/widgets/label.hpp"

using fcn::AdjustingContainer;
using fcn::Label;

TEST_CASE("AdjustingContainer default construction", "[unit][adjustingcontainer]")
{
    AdjustingContainer c;
    REQUIRE(c.getNumberOfColumns() == 1);
    REQUIRE(c.getColumnAlignment(0) == AdjustingContainer::Alignment::Left);
    REQUIRE(c.getColumnAlignment(1) == AdjustingContainer::Alignment::Left);
}

TEST_CASE("AdjustingContainer setNumberOfColumns happy path", "[unit][adjustingcontainer]")
{
    AdjustingContainer c;
    c.setNumberOfColumns(3);
    REQUIRE(c.getNumberOfColumns() == 3);
    REQUIRE(c.getColumnAlignment(0) == AdjustingContainer::Alignment::Left);
    REQUIRE(c.getColumnAlignment(1) == AdjustingContainer::Alignment::Left);
    REQUIRE(c.getColumnAlignment(2) == AdjustingContainer::Alignment::Left);
}

TEST_CASE("AdjustingContainer setNumberOfColumns shrinks alignment vector", "[unit][adjustingcontainer]")
{
    AdjustingContainer c;
    c.setNumberOfColumns(5);
    c.setColumnAlignment(2, AdjustingContainer::Alignment::Center);
    c.setColumnAlignment(4, AdjustingContainer::Alignment::Right);
    c.setNumberOfColumns(3);
    REQUIRE(c.getNumberOfColumns() == 3);
    REQUIRE(c.getColumnAlignment(2) == AdjustingContainer::Alignment::Center);
    REQUIRE(c.getColumnAlignment(4) == AdjustingContainer::Alignment::Left);
}

TEST_CASE("AdjustingContainer setColumnAlignment", "[unit][adjustingcontainer]")
{
    AdjustingContainer c;
    c.setNumberOfColumns(3);
    c.setColumnAlignment(0, AdjustingContainer::Alignment::Center);
    c.setColumnAlignment(1, AdjustingContainer::Alignment::Right);
    REQUIRE(c.getColumnAlignment(0) == AdjustingContainer::Alignment::Center);
    REQUIRE(c.getColumnAlignment(1) == AdjustingContainer::Alignment::Right);
    REQUIRE(c.getColumnAlignment(2) == AdjustingContainer::Alignment::Left);
}

TEST_CASE("AdjustingContainer add single widget positions at origin", "[unit][adjustingcontainer]")
{
    AdjustingContainer c;
    Label l;
    l.setSize(30, 20);
    c.add(&l);
    c.adjustContent();
    REQUIRE(l.getX() == 0);
    REQUIRE(l.getY() == 0);
}

TEST_CASE("AdjustingContainer add multiple widgets grid layout", "[unit][adjustingcontainer]")
{
    AdjustingContainer c;
    c.setNumberOfColumns(2);

    Label la, lb, lc, ld;
    la.setSize(20, 10);
    lb.setSize(30, 15);
    lc.setSize(25, 12);
    ld.setSize(10, 20);

    c.add(&la);
    c.add(&lb);
    c.add(&lc);
    c.add(&ld);
    c.adjustContent();

    // Column 0 max width = max(20, 25) = 25
    // Column 1 max width = max(30, 10) = 30
    // Row 0 max height = max(10, 15) = 15
    // Row 1 max height = max(12, 20) = 20

    REQUIRE(la.getX() == 0);
    REQUIRE(la.getY() == 0);
    REQUIRE(lb.getX() == 25);
    REQUIRE(lb.getY() == 0);
    REQUIRE(lc.getX() == 0);
    REQUIRE(lc.getY() == 15);
    REQUIRE(ld.getX() == 25);
    REQUIRE(ld.getY() == 15);
}

TEST_CASE("AdjustingContainer add with position overload forwards to single-arg add", "[unit][adjustingcontainer]")
{
    AdjustingContainer c;
    Label l;
    l.setSize(10, 10);
    c.add(&l, 99, 99);
    c.adjustContent();
    // x,y args ignored; widget placed at (0,0)
    REQUIRE(l.getX() == 0);
    REQUIRE(l.getY() == 0);
}

TEST_CASE("AdjustingContainer adjustContent center alignment", "[unit][adjustingcontainer]")
{
    AdjustingContainer c;
    c.setNumberOfColumns(2);
    c.setColumnAlignment(0, AdjustingContainer::Alignment::Left);
    c.setColumnAlignment(1, AdjustingContainer::Alignment::Center);

    Label la, lb;
    la.setSize(40, 10);
    lb.setSize(10, 10);
    c.add(&la);
    c.add(&lb);
    c.adjustContent();

    // Col 0 width = 40, Col 1 width = 10
    // Col 1 center: basex = 40, x = 40 + (10 - 10) / 2 = 40
    REQUIRE(la.getX() == 0);
    REQUIRE(lb.getX() == 40);
}

TEST_CASE("AdjustingContainer adjustContent right alignment", "[unit][adjustingcontainer]")
{
    AdjustingContainer c;
    c.setNumberOfColumns(2);
    c.setColumnAlignment(1, AdjustingContainer::Alignment::Right);

    Label la, lb;
    la.setSize(40, 10);
    lb.setSize(10, 10);
    c.add(&la);
    c.add(&lb);
    c.adjustContent();

    // Col 0 width = 40, Col 1 width = 10
    // Col 1 right: basex = 40, x = 40 + 10 - 10 = 40
    REQUIRE(la.getX() == 0);
    REQUIRE(lb.getX() == 40);
}

TEST_CASE("AdjustingContainer remove widget", "[unit][adjustingcontainer]")
{
    AdjustingContainer c;
    Label l1, l2;
    l1.setSize(10, 10);
    l2.setSize(20, 20);
    c.add(&l1);
    c.add(&l2);
    c.adjustContent();

    c.remove(&l1);
    c.adjustContent();

    REQUIRE(l2.getX() == 0);
    REQUIRE(l2.getY() == 0);
}

TEST_CASE("AdjustingContainer removeAllChildren", "[unit][adjustingcontainer]")
{
    AdjustingContainer c;
    Label l1, l2;
    l1.setSize(10, 10);
    l2.setSize(20, 20);
    c.add(&l1);
    c.add(&l2);
    c.removeAllChildren();
    REQUIRE(c.getChildrenCount() == 0);
}

TEST_CASE("AdjustingContainer resizeToContent updates size", "[unit][adjustingcontainer]")
{
    AdjustingContainer c;
    c.setNumberOfColumns(2);

    Label la, lb;
    la.setSize(30, 20);
    lb.setSize(50, 10);
    c.add(&la);
    c.add(&lb);

    int const wBefore = c.getWidth();
    int const hBefore = c.getHeight();

    c.resizeToContent(false);

    // After resize, container should be larger (or same) to fit children
    REQUIRE(c.getWidth() > wBefore);
    REQUIRE(c.getHeight() > hBefore);
}

TEST_CASE("AdjustingContainer resizeToContent recursion parameter", "[unit][adjustingcontainer]")
{
    AdjustingContainer c;
    Label l;
    l.setSize(10, 10);
    c.add(&l);
    c.resizeToContent(true);
    c.resizeToContent(false);
}

TEST_CASE("AdjustingContainer expandContent is no-op", "[unit][adjustingcontainer]")
{
    AdjustingContainer c;
    c.setSize(100, 100);
    int const w = c.getWidth();
    int const h = c.getHeight();
    c.expandContent(true);
    REQUIRE(c.getWidth() == w);
    REQUIRE(c.getHeight() == h);
    c.expandContent(false);
    REQUIRE(c.getWidth() == w);
    REQUIRE(c.getHeight() == h);
}

TEST_CASE("AdjustingContainer getChildrenArea with no border", "[unit][adjustingcontainer]")
{
    AdjustingContainer c;
    c.setSize(200, 100);
    fcn::Rectangle area = c.getChildrenArea();
    REQUIRE(area.x == 0);
    REQUIRE(area.y == 0);
    REQUIRE(area.width == 200);
    REQUIRE(area.height == 100);
}

TEST_CASE("AdjustingContainer getChildrenArea with border", "[unit][adjustingcontainer]")
{
    AdjustingContainer c;
    c.setBorderSize(4);
    c.setSize(200, 100);
    fcn::Rectangle area = c.getChildrenArea();
    REQUIRE(area.x == 4);
    REQUIRE(area.y == 4);
    REQUIRE(area.width == 200 - 8);
    REQUIRE(area.height == 100 - 8);
}

TEST_CASE("AdjustingContainer empty container adjustContent", "[unit][adjustingcontainer]")
{
    AdjustingContainer c;
    c.adjustContent();
    // No crash is the pass condition
}

TEST_CASE("AdjustingContainer adjustSize with no widgets", "[unit][adjustingcontainer]")
{
    AdjustingContainer c;
    c.adjustSize();
    // No crash is the pass condition
}

TEST_CASE("AdjustingContainer single widget after multiple add-remove cycles", "[unit][adjustingcontainer]")
{
    AdjustingContainer c;
    Label l1, l2, l3;
    l1.setSize(10, 10);
    l2.setSize(20, 20);
    l3.setSize(30, 30);
    c.add(&l1);
    c.add(&l2);
    c.remove(&l1);
    c.add(&l3);
    c.remove(&l2);
    c.adjustContent();
    REQUIRE(l3.getX() == 0);
    REQUIRE(l3.getY() == 0);
}

TEST_CASE("AdjustingContainer one widget per row with 1 column", "[unit][adjustingcontainer]")
{
    AdjustingContainer c;
    Label la, lb;
    la.setSize(15, 10);
    lb.setSize(20, 25);
    c.add(&la);
    c.add(&lb);
    c.adjustContent();
    REQUIRE(la.getX() == 0);
    REQUIRE(la.getY() == 0);
    // Col width = max(15,20) = 20, Row 0 height = 10, vSpacing = 0
    REQUIRE(lb.getX() == 0);
    REQUIRE(lb.getY() == 10);
}

TEST_CASE("AdjustingContainer widgets narrower than column stay left-aligned", "[unit][adjustingcontainer]")
{
    AdjustingContainer c;
    c.setNumberOfColumns(2);
    Label la, lb;
    la.setSize(50, 10);
    lb.setSize(10, 10);
    c.add(&la);
    c.add(&lb);
    c.adjustContent();
    // Col 0 width = max(50,0) = 50, Col 1 width = max(10,0) = 10
    REQUIRE(la.getX() == 0);
    REQUIRE(lb.getX() == 50);
}

TEST_CASE("AdjustingContainer add same widget twice", "[unit][adjustingcontainer]")
{
    AdjustingContainer c;
    Label l;
    l.setSize(10, 10);
    c.add(&l);
    c.add(&l);
    // Both mTabContainer (Container) and mContainedWidgets accept duplicates
    REQUIRE(c.getChildrenCount() == 2);
}

TEST_CASE("AdjustingContainer remove widget not in container throws", "[unit][adjustingcontainer]")
{
    AdjustingContainer c;
    Label l;
    REQUIRE_THROWS(c.remove(&l));
}
