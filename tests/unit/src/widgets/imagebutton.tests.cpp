// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2026 Fifengine contributors

// Third-party library includes
#include <catch2/catch_test_macros.hpp>

// Project headers
#include "fifechan/widgets/imagebutton.hpp"

using fcn::ImageButton;

TEST_CASE("ImageButton default constructor", "[unit][imagebutton]")
{
    ImageButton btn;
    REQUIRE(btn.getCaption().empty());
    REQUIRE(btn.getUpImage() == nullptr);
    REQUIRE(btn.getDownImage() == nullptr);
    REQUIRE(btn.getHoverImage() == nullptr);
    REQUIRE(btn.getInactiveUpImage() == nullptr);
    REQUIRE(btn.getInactiveDownImage() == nullptr);
    REQUIRE(btn.getInactiveHoverImage() == nullptr);
}

TEST_CASE("ImageButton resizeToContent", "[unit][imagebutton]")
{
    ImageButton btn;
    btn.resizeToContent();
    REQUIRE(true);
}

TEST_CASE("ImageButton adjustSize", "[unit][imagebutton]")
{
    ImageButton btn;
    btn.adjustSize();
    REQUIRE(true);
}

TEST_CASE("ImageButton getters return null initially", "[unit][imagebutton]")
{
    ImageButton btn;
    REQUIRE(btn.getUpImage() == nullptr);
    REQUIRE(btn.getDownImage() == nullptr);
    REQUIRE(btn.getHoverImage() == nullptr);
    REQUIRE(btn.getInactiveUpImage() == nullptr);
    REQUIRE(btn.getInactiveDownImage() == nullptr);
    REQUIRE(btn.getInactiveHoverImage() == nullptr);
}

TEST_CASE("ImageButton image type enum values", "[unit][imagebutton]")
{
    REQUIRE(static_cast<int>(ImageButton::ImageType::Up) == 0);
    REQUIRE(static_cast<int>(ImageButton::ImageType::Down) == 1);
    REQUIRE(static_cast<int>(ImageButton::ImageType::Hover) == 2);
    REQUIRE(static_cast<int>(ImageButton::ImageType::Up_Inactive) == 3);
    REQUIRE(static_cast<int>(ImageButton::ImageType::Down_Inactive) == 4);
    REQUIRE(static_cast<int>(ImageButton::ImageType::Hover_Inactive) == 5);
}

TEST_CASE("ImageButton default is focusable and has center alignment", "[unit][imagebutton]")
{
    ImageButton btn;
    REQUIRE(btn.isFocusable());
    REQUIRE(btn.getAlignment() == fcn::Graphics::Alignment::Center);
}

TEST_CASE("ImageButton setActive and isActive", "[unit][imagebutton]")
{
    ImageButton btn;
    REQUIRE(btn.isActive());

    btn.setActive(false);
    REQUIRE(!btn.isActive());

    btn.setActive(true);
    REQUIRE(btn.isActive());
}

TEST_CASE("ImageButton alignment get/set", "[unit][imagebutton]")
{
    ImageButton btn;
    btn.setAlignment(fcn::Graphics::Alignment::Left);
    REQUIRE(btn.getAlignment() == fcn::Graphics::Alignment::Left);

    btn.setAlignment(fcn::Graphics::Alignment::Right);
    REQUIRE(btn.getAlignment() == fcn::Graphics::Alignment::Right);
}
