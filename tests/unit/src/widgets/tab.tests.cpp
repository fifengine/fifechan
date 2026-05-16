// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2026 Fifengine contributors

// Third-party library includes
#include <catch2/catch_test_macros.hpp>

// Project headers
#include "fifechan/events/mouseevent.hpp"
#include "fifechan/widgets/tab.hpp"
#include "fifechan/widgets/tabbedarea.hpp"

using fcn::MouseEvent;
using fcn::Tab;
using fcn::TabbedArea;

TEST_CASE("Tab default constructor", "[unit][tab]")
{
    Tab tab;
    REQUIRE(tab.getTabbedArea() == nullptr);
    REQUIRE(!tab.isFocusable());
}

TEST_CASE("Tab setTabbedArea and getTabbedArea", "[unit][tab]")
{
    Tab tab;
    TabbedArea area;
    tab.setTabbedArea(&area);
    REQUIRE(tab.getTabbedArea() == &area);

    tab.setTabbedArea(nullptr);
    REQUIRE(tab.getTabbedArea() == nullptr);
}

TEST_CASE("Tab adjustSize", "[unit][tab]")
{
    Tab tab;
    tab.adjustSize();
    REQUIRE(true);
}

TEST_CASE("Tab getChildrenArea", "[unit][tab]")
{
    Tab tab;
    tab.setSize(100, 50);
    auto area = tab.getChildrenArea();
    REQUIRE((area.width > 0 && area.height > 0));
}

TEST_CASE("Tab mouseEntered and mouseExited", "[unit][tab]")
{
    Tab tab;
    MouseEvent enterEvent(
        &tab, &tab, false, false, false, false, MouseEvent::Type::Entered, MouseEvent::Button::Empty, 0, 0, 0);
    tab.mouseEntered(enterEvent);
    // No crash is sufficient

    MouseEvent exitEvent(
        &tab, &tab, false, false, false, false, MouseEvent::Type::Exited, MouseEvent::Button::Empty, 0, 0, 0);
    tab.mouseExited(exitEvent);
}
