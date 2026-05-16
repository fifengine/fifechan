// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2026 Fifengine contributors

// Third-party library includes
#include <catch2/catch_test_macros.hpp>

// Project headers
#include "fifechan/events/mouseevent.hpp"
#include "fifechan/widgets/menupopup.hpp"
#include "fifechan/widgets/modalbackdrop.hpp"

using fcn::MenuPopup;
using fcn::ModalBackdrop;
using fcn::MouseEvent;

TEST_CASE("ModalBackdrop default state", "[unit][modalbackdrop]")
{
    MenuPopup popup;
    ModalBackdrop backdrop(&popup);
    REQUIRE_FALSE(backdrop.isOpaque());
    REQUIRE_FALSE(backdrop.isFocusable());
    REQUIRE(backdrop.isEnabled());
    REQUIRE(backdrop.getChildrenCount() == 0);
}

TEST_CASE("ModalBackdrop draw does not crash", "[unit][modalbackdrop]")
{
    MenuPopup popup;
    ModalBackdrop backdrop(&popup);
    // draw with null graphics - just check no crash
    backdrop.draw(nullptr);
    REQUIRE(true);
}

TEST_CASE("ModalBackdrop mouseReleased does not crash", "[unit][modalbackdrop]")
{
    MenuPopup popup;
    ModalBackdrop backdrop(&popup);
    MouseEvent event(
        &backdrop,
        &backdrop,
        false,
        false,
        false,
        false,
        MouseEvent::Type::Released,
        MouseEvent::Button::Left,
        0,
        0,
        0);
    backdrop.mouseReleased(event);
    REQUIRE(true);
}

TEST_CASE("ModalBackdrop mouseEntered does not crash", "[unit][modalbackdrop]")
{
    MenuPopup popup;
    ModalBackdrop backdrop(&popup);
    MouseEvent event(
        &backdrop,
        &backdrop,
        false,
        false,
        false,
        false,
        MouseEvent::Type::Entered,
        MouseEvent::Button::Empty,
        0,
        0,
        0);
    backdrop.mouseEntered(event);
    REQUIRE(true);
}

TEST_CASE("ModalBackdrop mouseExited does not crash", "[unit][modalbackdrop]")
{
    MenuPopup popup;
    ModalBackdrop backdrop(&popup);
    MouseEvent event(
        &backdrop, &backdrop, false, false, false, false, MouseEvent::Type::Exited, MouseEvent::Button::Empty, 0, 0, 0);
    backdrop.mouseExited(event);
    REQUIRE(true);
}
