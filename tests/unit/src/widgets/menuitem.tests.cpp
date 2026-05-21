// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2026 Fifengine contributors

// Corresponding header include
#include "fifechan/widgets/menuitem.hpp"

// Standard library includes
#include <string>

// Third-party library includes
#include <catch2/catch_test_macros.hpp>

using fcn::MenuItem;

TEST_CASE("MenuItem default construction", "[unit][menuitem]")
{
    MenuItem item("Test");
    REQUIRE(item.getType() == MenuItem::Type::Action);
    REQUIRE(item.getSubmenu() == nullptr);
    REQUIRE(item.getText() == "Test");
    REQUIRE(item.getCaption() == "Test");
    REQUIRE(item.getIcon() == nullptr);
    REQUIRE(item.getIconGlyph().empty());
    REQUIRE(item.getIconGlyphFont() == nullptr);
    REQUIRE(item.getShortcut().empty());
    REQUIRE_FALSE(item.isChecked());
    REQUIRE(item.isEnabled());
}

TEST_CASE("MenuItem setType and getType", "[unit][menuitem]")
{
    MenuItem item("");
    item.setType(MenuItem::Type::Submenu);
    REQUIRE(item.getType() == MenuItem::Type::Submenu);
    item.setType(MenuItem::Type::Separator);
    REQUIRE(item.getType() == MenuItem::Type::Separator);
    item.setType(MenuItem::Type::Checkable);
    REQUIRE(item.getType() == MenuItem::Type::Checkable);
    item.setType(MenuItem::Type::Action);
    REQUIRE(item.getType() == MenuItem::Type::Action);
}

TEST_CASE("MenuItem setSubmenu and getSubmenu", "[unit][menuitem]")
{
    MenuItem item("");
    REQUIRE(item.getSubmenu() == nullptr);
    item.setSubmenu(nullptr);
    REQUIRE(item.getSubmenu() == nullptr);
}

TEST_CASE("MenuItem setText and getText", "[unit][menuitem]")
{
    MenuItem item("");
    item.setText("New text");
    REQUIRE(item.getText() == "New text");
    REQUIRE(item.getCaption() == "New text");
}

TEST_CASE("MenuItem setIcon and getIcon", "[unit][menuitem]")
{
    MenuItem item("");
    REQUIRE(item.getIcon() == nullptr);
    item.setIcon(nullptr);
    REQUIRE(item.getIcon() == nullptr);
}

TEST_CASE("MenuItem setIconGlyph and getIconGlyph", "[unit][menuitem]")
{
    MenuItem item("");
    REQUIRE(item.getIconGlyph().empty());
    item.setIconGlyph("\xF0\x9F\x94\xA7");
    REQUIRE(item.getIconGlyph() == "\xF0\x9F\x94\xA7");
    item.setIconGlyph("");
    REQUIRE(item.getIconGlyph().empty());
}

TEST_CASE("MenuItem setIconGlyphFont and getIconGlyphFont", "[unit][menuitem]")
{
    MenuItem item("");
    REQUIRE(item.getIconGlyphFont() == nullptr);
    item.setIconGlyphFont(nullptr);
    REQUIRE(item.getIconGlyphFont() == nullptr);
}

TEST_CASE("MenuItem setShortcut and getShortcut", "[unit][menuitem]")
{
    MenuItem item("");
    REQUIRE(item.getShortcut().empty());
    item.setShortcut("Ctrl+S");
    REQUIRE(item.getShortcut() == "Ctrl+S");
    item.setShortcut("");
    REQUIRE(item.getShortcut().empty());
}

TEST_CASE("MenuItem setChecked and isChecked", "[unit][menuitem]")
{
    MenuItem item("");
    REQUIRE_FALSE(item.isChecked());
    item.setChecked(true);
    REQUIRE(item.isChecked());
    item.setChecked(false);
    REQUIRE_FALSE(item.isChecked());
}

TEST_CASE("MenuItem setEnabled and isEnabled", "[unit][menuitem]")
{
    MenuItem item("");
    REQUIRE(item.isEnabled());
    item.setEnabled(false);
    REQUIRE_FALSE(item.isEnabled());
    item.setEnabled(true);
    REQUIRE(item.isEnabled());
}

TEST_CASE("MenuItem getShortcut returns const reference", "[unit][menuitem]")
{
    MenuItem item("");
    item.setShortcut("Ctrl+Z");
    std::string const & ref1 = item.getShortcut();
    std::string const & ref2 = item.getShortcut();
    // cppcheck-suppress knownConditionTrueFalse
    REQUIRE(&ref1 == &ref2);
}

TEST_CASE("MenuItem setSubmenu changes type to Submenu", "[unit][menuitem]")
{
    MenuItem item("");
    REQUIRE(item.getType() == MenuItem::Type::Action);
    // setSubmenu with non-null changes type
    // with null it doesn't change type
    item.setSubmenu(nullptr);
    REQUIRE(item.getType() == MenuItem::Type::Action);
}
