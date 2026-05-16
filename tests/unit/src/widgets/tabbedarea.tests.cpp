// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2026 Fifengine contributors

// Third-party library includes
#include <catch2/catch_test_macros.hpp>

// Project headers
#include "fifechan/defaultfont.hpp"
#include "fifechan/widgets/container.hpp"
#include "fifechan/widgets/label.hpp"
#include "fifechan/widgets/tab.hpp"
#include "fifechan/widgets/tabbedarea.hpp"

using fcn::Container;
using fcn::DefaultFont;
using fcn::Label;
using fcn::Tab;
using fcn::TabbedArea;

TEST_CASE("TabbedArea default constructor", "[unit][tabbedarea]")
{
    TabbedArea area;
    REQUIRE(!area.isOpaque());
    REQUIRE(area.getNumberOfTabs() == 0);
    REQUIRE(area.getSelectedTab() == nullptr);
    REQUIRE(area.getSelectedTabIndex() == -1);
}

TEST_CASE("TabbedArea setOpaque and isOpaque", "[unit][tabbedarea]")
{
    TabbedArea area;
    area.setOpaque(true);
    REQUIRE(area.isOpaque());

    area.setOpaque(false);
    REQUIRE(!area.isOpaque());
}

TEST_CASE("TabbedArea addTab and getNumberOfTabs", "[unit][tabbedarea]")
{
    TabbedArea area;
    Tab t1;
    Tab t2;
    Label c1;
    Label c2;

    area.addTab(&t1, &c1);
    REQUIRE(area.getNumberOfTabs() == 1);

    area.addTab(&t2, &c2);
    REQUIRE(area.getNumberOfTabs() == 2);
}

TEST_CASE("TabbedArea addTab sets first tab selected", "[unit][tabbedarea]")
{
    TabbedArea area;
    Tab myTab;
    Label content;
    area.addTab(&myTab, &content);
    REQUIRE(area.isTabSelected(0u));
    REQUIRE(area.isTabSelected(&myTab));
}

TEST_CASE("TabbedArea setSelectedTab by index", "[unit][tabbedarea]")
{
    TabbedArea area;
    Tab ta;
    Tab tb;
    Tab tc;
    Label ca;
    Label cb;
    Label cc;

    area.addTab(&ta, &ca);
    area.addTab(&tb, &cb);
    area.addTab(&tc, &cc);

    REQUIRE(area.isTabSelected(0u));

    area.setSelectedTab(1);
    REQUIRE(area.isTabSelected(1u));
    REQUIRE(!area.isTabSelected(0u));

    area.setSelectedTab(2);
    REQUIRE(area.isTabSelected(2u));
}

TEST_CASE("TabbedArea setSelectedTab by pointer", "[unit][tabbedarea]")
{
    TabbedArea area;
    Tab ta;
    Tab tb;
    Label ca;
    Label cb;

    area.addTab(&ta, &ca);
    area.addTab(&tb, &cb);
    area.setSelectedTab(&tb);
    REQUIRE(area.isTabSelected(1u));
    REQUIRE(area.getSelectedTab() == &tb);
}

TEST_CASE("TabbedArea removeTabWithIndex", "[unit][tabbedarea]")
{
    TabbedArea area;
    Tab ta;
    Tab tb;
    Label ca;
    Label cb;

    area.addTab(&ta, &ca);
    area.addTab(&tb, &cb);
    REQUIRE(area.getNumberOfTabs() == 2);

    area.removeTabWithIndex(0);
    REQUIRE(area.getNumberOfTabs() == 1);
}

TEST_CASE("TabbedArea removeTab", "[unit][tabbedarea]")
{
    TabbedArea area;
    Tab ta;
    Tab tb;
    Label ca;
    Label cb;

    area.addTab(&ta, &ca);
    area.addTab(&tb, &cb);
    REQUIRE(area.getNumberOfTabs() == 2);

    area.removeTab(&ta);
    REQUIRE(area.getNumberOfTabs() == 1);
}

TEST_CASE("TabbedArea setLayout and getLayout", "[unit][tabbedarea]")
{
    TabbedArea area;
    REQUIRE(area.getLayout() == Container::LayoutPolicy::Horizontal);

    area.setLayout(Container::LayoutPolicy::Vertical);
    REQUIRE(area.getLayout() == Container::LayoutPolicy::Vertical);

    area.setLayout(Container::LayoutPolicy::Absolute);
    REQUIRE(area.getLayout() == Container::LayoutPolicy::Absolute);
}

TEST_CASE("TabbedArea setUniformSize and isUniformSize", "[unit][tabbedarea]")
{
    TabbedArea area;
    REQUIRE(!area.isUniformSize());

    area.setUniformSize(true);
    REQUIRE(area.isUniformSize());

    area.setUniformSize(false);
    REQUIRE(!area.isUniformSize());
}

TEST_CASE("TabbedArea setBackgroundWidget", "[unit][tabbedarea]")
{
    TabbedArea area;
    Label bg;
    area.setBackgroundWidget(&bg);
    REQUIRE(area.getBackgroundWidget() == &bg);

    area.setBackgroundWidget(nullptr);
    REQUIRE(area.getBackgroundWidget() == nullptr);
}

TEST_CASE("TabbedArea adjustSize", "[unit][tabbedarea]")
{
    TabbedArea area;
    area.adjustSize();
    REQUIRE(true);
}

TEST_CASE("TabbedArea setWidth/Height/Size/Dimension", "[unit][tabbedarea]")
{
    TabbedArea area;
    area.setWidth(300);
    REQUIRE(area.getWidth() == 300);

    area.setHeight(200);
    REQUIRE(area.getHeight() == 200);

    area.setSize(400, 300);
    REQUIRE(area.getWidth() == 400);
    REQUIRE(area.getHeight() == 300);
}

TEST_CASE("TabbedArea resizeToContent", "[unit][tabbedarea]")
{
    TabbedArea area;
    area.resizeToContent();
    REQUIRE(true);

    area.resizeToContent(true);
    REQUIRE(true);

    area.resizeToContent(false);
    REQUIRE(true);
}

TEST_CASE("TabbedArea expandContent", "[unit][tabbedarea]")
{
    TabbedArea area;
    area.expandContent();
    REQUIRE(true);
}

TEST_CASE("TabbedArea setBaseColor", "[unit][tabbedarea]")
{
    TabbedArea area;
    fcn::Color color(255, 0, 0);
    area.setBaseColor(color);
    REQUIRE(area.getBaseColor() == color);
}
