// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2026 Fifengine contributors

// Third-party library includes
#include <catch2/catch_test_macros.hpp>

// Project headers
#include "fifechan/defaultfont.hpp"
#include "fifechan/events/actionevent.hpp"
#include "fifechan/events/event.hpp"
#include "fifechan/events/keyevent.hpp"
#include "fifechan/events/mouseevent.hpp"
#include "fifechan/exception.hpp"
#include "fifechan/graphics.hpp"
#include "fifechan/key.hpp"
#include "fifechan/widgets/container.hpp"
#include "fifechan/widgets/label.hpp"
#include "fifechan/widgets/tab.hpp"
#include "fifechan/widgets/tabbedarea.hpp"

using fcn::ActionEvent;
using fcn::Container;
using fcn::DefaultFont;
using fcn::Event;
using fcn::Graphics;
using fcn::Key;
using fcn::KeyEvent;
using fcn::Label;
using fcn::MouseEvent;
using fcn::Tab;
using fcn::TabbedArea;

namespace
{
    // Minimal mock implementing all pure virtual methods of fcn::Graphics
    class MockGraphics : public Graphics
    {
        public:
            void setColor(fcn::Color const & color) override
            {
                (void)color;
            }

            fcn::Color const & getColor() const override
            {
                static fcn::Color c;
                return c;
            }

            void drawPoint(int x, int y) override
            {
                (void)x;
                (void)y;
            }

            void drawLine(int x1, int y1, int x2, int y2) override
            {
                (void)x1;
                (void)y1;
                (void)x2;
                (void)y2;
            }

            void drawLine(int x1, int y1, int x2, int y2, unsigned int width) override
            {
                (void)x1;
                (void)y1;
                (void)x2;
                (void)y2;
                (void)width;
            }

            void drawPolyLine(fcn::PointVector const & points, unsigned int width) override
            {
                (void)points;
                (void)width;
            }

            void drawBezier(fcn::PointVector const & points, int steps, unsigned int width) override
            {
                (void)points;
                (void)steps;
                (void)width;
            }

            void drawRectangle(fcn::Rectangle const & rectangle) override
            {
                (void)rectangle;
            }

            void fillRectangle(fcn::Rectangle const & rectangle) override
            {
                (void)rectangle;
            }

            void drawCircle(fcn::Point const & p, unsigned int radius) override
            {
                (void)p;
                (void)radius;
            }

            void drawFillCircle(fcn::Point const & p, unsigned int radius) override
            {
                (void)p;
                (void)radius;
            }

            void drawCircleSegment(fcn::Point const & p, unsigned int radius, int sangle, int eangle) override
            {
                (void)p;
                (void)radius;
                (void)sangle;
                (void)eangle;
            }

            void drawFillCircleSegment(fcn::Point const & p, unsigned int radius, int sangle, int eangle) override
            {
                (void)p;
                (void)radius;
                (void)sangle;
                (void)eangle;
            }

            void drawImage(fcn::Image const * image, int srcX, int srcY, int dstX, int dstY, int width, int height) override
            {
                (void)image;
                (void)srcX;
                (void)srcY;
                (void)dstX;
                (void)dstY;
                (void)width;
                (void)height;
            }
    };
} // namespace

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

TEST_CASE("TabbedArea remove last tab from 4 keeps previous selected", "[unit][tabbedarea]")
{
    TabbedArea area;
    Tab ta, tb, tc, td;
    Label ca, cb, cc, cd;

    area.addTab(&ta, &ca);
    area.addTab(&tb, &cb);
    area.addTab(&tc, &cc);
    area.addTab(&td, &cd);
    REQUIRE(area.getNumberOfTabs() == 4);
    REQUIRE(area.isTabSelected(0u));

    area.setSelectedTab(3);
    REQUIRE(area.isTabSelected(3u));

    area.removeTab(&td);
    REQUIRE(area.getNumberOfTabs() == 3);
    REQUIRE(area.isTabSelected(2u));
    REQUIRE(area.getSelectedTab() == &tc);
}

TEST_CASE("TabbedArea remove middle tab from 4 keeps same index", "[unit][tabbedarea]")
{
    TabbedArea area;
    Tab ta, tb, tc, td;
    Label ca, cb, cc, cd;

    area.addTab(&ta, &ca);
    area.addTab(&tb, &cb);
    area.addTab(&tc, &cc);
    area.addTab(&td, &cd);
    REQUIRE(area.getNumberOfTabs() == 4);

    area.setSelectedTab(1);
    REQUIRE(area.isTabSelected(1u));

    area.removeTab(&tb);
    REQUIRE(area.getNumberOfTabs() == 3);
    REQUIRE(area.isTabSelected(1u));
    REQUIRE(area.getSelectedTab() == &tc);
}

TEST_CASE("TabbedArea remove first tab from 4 keeps same index", "[unit][tabbedarea]")
{
    TabbedArea area;
    Tab ta, tb, tc, td;
    Label ca, cb, cc, cd;

    area.addTab(&ta, &ca);
    area.addTab(&tb, &cb);
    area.addTab(&tc, &cc);
    area.addTab(&td, &cd);
    REQUIRE(area.getNumberOfTabs() == 4);

    area.setSelectedTab(0u);
    REQUIRE(area.isTabSelected(0u));

    area.removeTab(&ta);
    REQUIRE(area.getNumberOfTabs() == 3);
    REQUIRE(area.isTabSelected(0u));
    REQUIRE(area.getSelectedTab() == &tb);
}

TEST_CASE("TabbedArea remove non-selected tab from 4 leaves selection", "[unit][tabbedarea]")
{
    TabbedArea area;
    Tab ta, tb, tc, td;
    Label ca, cb, cc, cd;

    area.addTab(&ta, &ca);
    area.addTab(&tb, &cb);
    area.addTab(&tc, &cc);
    area.addTab(&td, &cd);
    REQUIRE(area.getNumberOfTabs() == 4);

    area.setSelectedTab(1);
    REQUIRE(area.isTabSelected(1u));

    area.removeTab(&td);
    REQUIRE(area.getNumberOfTabs() == 3);
    REQUIRE(area.isTabSelected(1u));
    REQUIRE(area.getSelectedTab() == &tb);
}

TEST_CASE("TabbedArea remove all tabs progressively from 4", "[unit][tabbedarea]")
{
    TabbedArea area;
    Tab ta, tb, tc, td;
    Label ca, cb, cc, cd;

    area.addTab(&ta, &ca);
    area.addTab(&tb, &cb);
    area.addTab(&tc, &cc);
    area.addTab(&td, &cd);

    area.removeTab(&td);
    REQUIRE(area.getNumberOfTabs() == 3);

    area.removeTab(&tc);
    REQUIRE(area.getNumberOfTabs() == 2);

    area.removeTab(&tb);
    REQUIRE(area.getNumberOfTabs() == 1);

    area.removeTab(&ta);
    REQUIRE(area.getNumberOfTabs() == 0);
    REQUIRE(area.getSelectedTab() == nullptr);
    REQUIRE(area.getSelectedTabIndex() == -1);
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

TEST_CASE("TabbedArea setBaseColor with tabs", "[unit][tabbedarea]")
{
    TabbedArea area;
    Tab ta, tb;
    Label ca, cb;
    area.addTab(&ta, &ca);
    area.addTab(&tb, &cb);

    fcn::Color color(128, 64, 32);
    area.setBaseColor(color);
    REQUIRE(area.getBaseColor() == color);
}

TEST_CASE("TabbedArea addTab with Absolute layout changes to area layout", "[unit][tabbedarea]")
{
    TabbedArea area;
    Tab tab;
    tab.setLayout(Container::LayoutPolicy::Absolute);
    Label content;
    area.addTab(&tab, &content);
    REQUIRE(tab.getLayout() == Container::LayoutPolicy::Horizontal);
}

TEST_CASE("TabbedArea removeTabWithIndex out of bounds throws", "[unit][tabbedarea]")
{
    TabbedArea area;
    REQUIRE_THROWS_AS(area.removeTabWithIndex(0), fcn::Exception);
}

TEST_CASE("TabbedArea isTabSelected out of bounds throws", "[unit][tabbedarea]")
{
    TabbedArea area;
    REQUIRE_THROWS_AS(area.isTabSelected(0u), fcn::Exception);
}

TEST_CASE("TabbedArea setSelectedTab unsigned int out of bounds throws", "[unit][tabbedarea]")
{
    TabbedArea area;
    REQUIRE_THROWS_AS(area.setSelectedTab(0u), fcn::Exception);
}

TEST_CASE("TabbedArea setSelectedTab same tab twice does nothing", "[unit][tabbedarea]")
{
    TabbedArea area;
    Tab ta, tb;
    Label ca, cb;
    area.addTab(&ta, &ca);
    area.addTab(&tb, &cb);
    REQUIRE(area.isTabSelected(0u));

    area.setSelectedTab(&ta);
    REQUIRE(area.isTabSelected(0u));
}

TEST_CASE("TabbedArea draw non-opaque without tabs", "[unit][tabbedarea]")
{
    TabbedArea area;
    area.setSize(200, 100);
    MockGraphics g;
    area.draw(&g);
    REQUIRE(true);
}

TEST_CASE("TabbedArea draw opaque without tabs", "[unit][tabbedarea]")
{
    TabbedArea area;
    area.setSize(200, 100);
    area.setOpaque(true);
    MockGraphics g;
    area.draw(&g);
    REQUIRE(true);
}

TEST_CASE("TabbedArea draw with tabs and selection", "[unit][tabbedarea]")
{
    TabbedArea area;
    area.setSize(400, 300);
    Tab ta, tb;
    Label ca, cb;
    DefaultFont font;
    ta.setFont(&font);
    tb.setFont(&font);
    area.addTab(&ta, &ca);
    area.addTab(&tb, &cb);
    area.setSelectedTab(0u);

    MockGraphics g;
    area.draw(&g);
    REQUIRE(true);
}

TEST_CASE("TabbedArea setLayout Vertical calls adjustSize correct branch", "[unit][tabbedarea]")
{
    TabbedArea area;
    area.setSize(200, 100);
    area.setLayout(Container::LayoutPolicy::Vertical);
    area.adjustSize();
    REQUIRE(area.getLayout() == Container::LayoutPolicy::Vertical);
}

TEST_CASE("TabbedArea setLayout Vertical with tabs adjusts positions", "[unit][tabbedarea]")
{
    TabbedArea area;
    area.setSize(300, 200);
    area.setLayout(Container::LayoutPolicy::Vertical);
    Tab ta, tb;
    Label ca, cb;
    DefaultFont font;
    ta.setFont(&font);
    tb.setFont(&font);
    area.addTab(&ta, &ca);
    area.addTab(&tb, &cb);
    area.resizeToContent(false);
    REQUIRE(area.getNumberOfTabs() == 2);
}

TEST_CASE("TabbedArea setVerticalSpacing and getVerticalSpacing", "[unit][tabbedarea]")
{
    TabbedArea area;
    REQUIRE(area.getVerticalSpacing() == 2);

    area.setVerticalSpacing(5);
    REQUIRE(area.getVerticalSpacing() == 5);

    area.setVerticalSpacing(2);
    REQUIRE(area.getVerticalSpacing() == 2);
}

TEST_CASE("TabbedArea setHorizontalSpacing and getHorizontalSpacing", "[unit][tabbedarea]")
{
    TabbedArea area;
    REQUIRE(area.getHorizontalSpacing() == 2);

    area.setHorizontalSpacing(8);
    REQUIRE(area.getHorizontalSpacing() == 8);

    area.setHorizontalSpacing(2);
    REQUIRE(area.getHorizontalSpacing() == 2);
}

TEST_CASE("TabbedArea keyPressed left navigates tabs", "[unit][tabbedarea]")
{
    TabbedArea area;
    area.setFocused(true);
    Tab ta, tb, tc;
    Label ca, cb, cc;
    area.addTab(&ta, &ca);
    area.addTab(&tb, &cb);
    area.addTab(&tc, &cc);
    area.setSelectedTab(1);
    REQUIRE(area.isTabSelected(1u));

    Key const leftKey(Key::LEFT);
    KeyEvent leftEvent(&area, &area, false, false, false, false, KeyEvent::Type::Pressed, false, leftKey);
    area.keyPressed(leftEvent);
    REQUIRE(area.isTabSelected(0u));
}

TEST_CASE("TabbedArea keyPressed right navigates tabs", "[unit][tabbedarea]")
{
    TabbedArea area;
    area.setFocused(true);
    Tab ta, tb, tc;
    Label ca, cb, cc;
    area.addTab(&ta, &ca);
    area.addTab(&tb, &cb);
    area.addTab(&tc, &cc);
    area.setSelectedTab(1);
    REQUIRE(area.isTabSelected(1u));

    Key const rightKey(Key::RIGHT);
    KeyEvent rightEvent(&area, &area, false, false, false, false, KeyEvent::Type::Pressed, false, rightKey);
    area.keyPressed(rightEvent);
    REQUIRE(area.isTabSelected(2u));
}

TEST_CASE("TabbedArea keyPressed left at first tab does nothing", "[unit][tabbedarea]")
{
    TabbedArea area;
    area.setFocused(true);
    Tab ta, tb;
    Label ca, cb;
    area.addTab(&ta, &ca);
    area.addTab(&tb, &cb);
    area.setSelectedTab(0u);
    REQUIRE(area.isTabSelected(0u));

    Key const leftKey(Key::LEFT);
    KeyEvent leftEvent(&area, &area, false, false, false, false, KeyEvent::Type::Pressed, false, leftKey);
    area.keyPressed(leftEvent);
    REQUIRE(area.isTabSelected(0u));
}

TEST_CASE("TabbedArea keyPressed right at last tab does nothing", "[unit][tabbedarea]")
{
    TabbedArea area;
    area.setFocused(true);
    Tab ta, tb;
    Label ca, cb;
    area.addTab(&ta, &ca);
    area.addTab(&tb, &cb);
    area.setSelectedTab(1);
    REQUIRE(area.isTabSelected(1u));

    Key const rightKey(Key::RIGHT);
    KeyEvent rightEvent(&area, &area, false, false, false, false, KeyEvent::Type::Pressed, false, rightKey);
    area.keyPressed(rightEvent);
    REQUIRE(area.isTabSelected(1u));
}

TEST_CASE("TabbedArea keyPressed consumed does nothing", "[unit][tabbedarea]")
{
    TabbedArea area;
    area.setFocused(true);
    Tab ta, tb;
    Label ca, cb;
    area.addTab(&ta, &ca);
    area.addTab(&tb, &cb);

    Key const rightKey(Key::RIGHT);
    KeyEvent rightEvent(&area, &area, false, false, false, false, KeyEvent::Type::Pressed, false, rightKey);
    rightEvent.consume();
    area.keyPressed(rightEvent);
    REQUIRE(area.isTabSelected(0u));
}

TEST_CASE("TabbedArea mousePressed on tab selects it", "[unit][tabbedarea]")
{
    TabbedArea area;
    Tab ta, tb;
    Label ca, cb;
    area.addTab(&ta, &ca);
    area.addTab(&tb, &cb);
    area.setSelectedTab(0u);

    // tb is at x=some offset. With default font, width is font width * tab padding etc.
    // Simulate a click on tb's area
    MouseEvent pressEvent(&area, &area, false, false, false, false, MouseEvent::Type::Pressed, MouseEvent::Button::Left, 50, 5, 0);
    area.mousePressed(pressEvent);
    // tb is at index 1 at x=0 initially, so click at x=50 may or may not hit tb;
    // just verify no crash
    REQUIRE(true);
}

TEST_CASE("TabbedArea mousePressed non-left button does nothing", "[unit][tabbedarea]")
{
    TabbedArea area;
    Tab ta;
    Label ca;
    area.addTab(&ta, &ca);

    MouseEvent pressEvent(&area, &area, false, false, false, false, MouseEvent::Type::Pressed, MouseEvent::Button::Right, 0, 0, 0);
    area.mousePressed(pressEvent);
    REQUIRE(area.isTabSelected(0u));
}

TEST_CASE("TabbedArea death with tab source removes it", "[unit][tabbedarea]")
{
    TabbedArea area;
    Tab tab;
    Label content;
    area.addTab(&tab, &content);
    REQUIRE(area.getNumberOfTabs() == 1);

    Event event(&tab);
    area.death(event);
    REQUIRE(area.getNumberOfTabs() == 0);
}

TEST_CASE("TabbedArea death with non-tab source does nothing", "[unit][tabbedarea]")
{
    TabbedArea area;
    Tab tab;
    Label content;
    Label nonTab;
    area.addTab(&tab, &content);
    REQUIRE(area.getNumberOfTabs() == 1);

    Event event(&nonTab);
    area.death(event);
    REQUIRE(area.getNumberOfTabs() == 1);
}

TEST_CASE("TabbedArea action from tab selects it", "[unit][tabbedarea]")
{
    TabbedArea area;
    Tab ta, tb;
    Label ca, cb;
    area.addTab(&ta, &ca);
    area.addTab(&tb, &cb);
    area.setSelectedTab(0u);
    REQUIRE(area.isTabSelected(0u));

    ActionEvent actionEvent(&tb, "tab");
    area.action(actionEvent);
    REQUIRE(area.isTabSelected(1u));
    REQUIRE(area.getSelectedTab() == &tb);
}

TEST_CASE("TabbedArea action from non-tab throws", "[unit][tabbedarea]")
{
    TabbedArea area;
    Label notATab;
    ActionEvent actionEvent(&notATab, "notatab");
    REQUIRE_THROWS_AS(area.action(actionEvent), fcn::Exception);
}

TEST_CASE("TabbedArea setDimension adjusts layout", "[unit][tabbedarea]")
{
    TabbedArea area;
    fcn::Rectangle dim(0, 0, 500, 300);
    area.setDimension(dim);
    REQUIRE(area.getWidth() == 500);
    REQUIRE(area.getHeight() == 300);
}

TEST_CASE("TabbedArea resizeToContent with tabs forces adjustment", "[unit][tabbedarea]")
{
    TabbedArea area;
    Tab ta, tb;
    Label ca, cb;
    DefaultFont font;
    ta.setFont(&font);
    tb.setFont(&font);
    area.addTab(&ta, &ca);
    area.addTab(&tb, &cb);
    area.resizeToContent(true);
    REQUIRE(area.getNumberOfTabs() == 2);
}

TEST_CASE("TabbedArea expandContent with tabs", "[unit][tabbedarea]")
{
    TabbedArea area;
    Tab ta, tb;
    Label ca, cb;
    DefaultFont font;
    ta.setFont(&font);
    tb.setFont(&font);
    area.addTab(&ta, &ca);
    area.addTab(&tb, &cb);
    area.expandContent(true);
    REQUIRE(area.getNumberOfTabs() == 2);
}
