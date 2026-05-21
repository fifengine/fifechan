// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2026 Fifengine contributors

// Corresponding header include
#include "fifechan/widgets/menupopup.hpp"

// Third-party library includes
#include <catch2/catch_test_macros.hpp>

using fcn::MenuPopup;

TEST_CASE("MenuPopup default construction", "[unit][menupopup]")
{
    MenuPopup popup;
    REQUIRE(popup.isOpaque());
    REQUIRE(popup.getBorderSize() == 1);
    REQUIRE(popup.getLayout() == fcn::Container::LayoutPolicy::AutoSize);
    REQUIRE_FALSE(popup.isVisible());
    REQUIRE(popup.getParentMenuItem() == nullptr);
    REQUIRE(popup.getParentMenu() == nullptr);
}

TEST_CASE("MenuPopup setParentMenuItem and getParentMenuItem", "[unit][menupopup]")
{
    MenuPopup popup;
    REQUIRE(popup.getParentMenuItem() == nullptr);
    popup.setParentMenuItem(nullptr);
    REQUIRE(popup.getParentMenuItem() == nullptr);
}

TEST_CASE("MenuPopup setParentMenu and getParentMenu", "[unit][menupopup]")
{
    MenuPopup popup;
    REQUIRE(popup.getParentMenu() == nullptr);
    popup.setParentMenu(nullptr);
    REQUIRE(popup.getParentMenu() == nullptr);
}

TEST_CASE("MenuPopup isVisible initially false", "[unit][menupopup]")
{
    MenuPopup popup;
    REQUIRE_FALSE(popup.isVisible());
}

TEST_CASE("MenuPopup addSeparator", "[unit][menupopup]")
{
    MenuPopup popup;
    popup.addSeparator();
    REQUIRE(popup.getChildrenCount() >= 1);
}
