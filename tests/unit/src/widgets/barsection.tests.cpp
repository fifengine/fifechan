// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2026 Fifengine contributors

// Third-party library includes
#include <catch2/catch_test_macros.hpp>

// Project headers
#include "fifechan/widgets/barsection.hpp"

using namespace fcn;

// ============================================================================
// BarSection constructor
// ============================================================================

TEST_CASE("BarSection constructor sets horizontal layout", "[unit][barsection]")
{
    BarSection section;

    // Default alignment should be Start
    REQUIRE(section.getAlignment() == BarSection::Alignment::Start);

    // Default expand should be false
    REQUIRE(section.isExpand() == false);
}

// ============================================================================
// BarSection alignment
// ============================================================================

TEST_CASE("BarSection setAlignment and getAlignment", "[unit][barsection]")
{
    BarSection section;

    SECTION("set to Start")
    {
        section.setAlignment(BarSection::Alignment::Start);
        REQUIRE(section.getAlignment() == BarSection::Alignment::Start);
    }

    SECTION("set to Center")
    {
        section.setAlignment(BarSection::Alignment::Center);
        REQUIRE(section.getAlignment() == BarSection::Alignment::Center);
    }

    SECTION("set to End")
    {
        section.setAlignment(BarSection::Alignment::End);
        REQUIRE(section.getAlignment() == BarSection::Alignment::End);
    }
}

TEST_CASE("BarSection alignment can be changed multiple times", "[unit][barsection]")
{
    BarSection section;

    section.setAlignment(BarSection::Alignment::Center);
    REQUIRE(section.getAlignment() == BarSection::Alignment::Center);

    section.setAlignment(BarSection::Alignment::End);
    REQUIRE(section.getAlignment() == BarSection::Alignment::End);

    section.setAlignment(BarSection::Alignment::Start);
    REQUIRE(section.getAlignment() == BarSection::Alignment::Start);
}

// ============================================================================
// BarSection expand
// ============================================================================

TEST_CASE("BarSection setExpand and isExpand", "[unit][barsection]")
{
    BarSection section;

    SECTION("set expand to true")
    {
        section.setExpand(true);
        REQUIRE(section.isExpand() == true);
    }

    SECTION("set expand to false")
    {
        section.setExpand(false);
        REQUIRE(section.isExpand() == false);
    }
}

TEST_CASE("BarSection expand can be toggled", "[unit][barsection]")
{
    BarSection section;

    section.setExpand(true);
    REQUIRE(section.isExpand() == true);

    section.setExpand(false);
    REQUIRE(section.isExpand() == false);

    section.setExpand(true);
    REQUIRE(section.isExpand() == true);
}
