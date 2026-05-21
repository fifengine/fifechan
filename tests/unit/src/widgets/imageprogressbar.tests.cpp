// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2026 Fifengine contributors

// Corresponding header include
#include "fifechan/widgets/imageprogressbar.hpp"

// Third-party library includes
#include <catch2/catch_test_macros.hpp>

// Project headers (subdirs before local)
#include "fifechan/exception.hpp"

using fcn::ImageProgressBar;

TEST_CASE("ImageProgressBar default construction", "[unit][imageprogressbar]")
{
    ImageProgressBar bar;
    REQUIRE(bar.getBarImage() == nullptr);
    REQUIRE(bar.getForegroundImage() == nullptr);
    REQUIRE(bar.getMaxValue() == 100);
    REQUIRE(bar.getValue() == 0);
    REQUIRE(bar.getOrientation() == ImageProgressBar::Orientation::Horizontal);
    REQUIRE(bar.isOpaque());
}

TEST_CASE("ImageProgressBar setOpaque and isOpaque", "[unit][imageprogressbar]")
{
    ImageProgressBar bar;
    REQUIRE(bar.isOpaque());
    bar.setOpaque(false);
    REQUIRE_FALSE(bar.isOpaque());
    bar.setOpaque(true);
    REQUIRE(bar.isOpaque());
}

TEST_CASE("ImageProgressBar setValue and getValue", "[unit][imageprogressbar]")
{
    ImageProgressBar bar;
    bar.setValue(50);
    REQUIRE(bar.getValue() == 50);
    bar.setValue(0);
    REQUIRE(bar.getValue() == 0);
    bar.setValue(100);
    REQUIRE(bar.getValue() == 100);
}

TEST_CASE("ImageProgressBar setValue clamps to max", "[unit][imageprogressbar]")
{
    ImageProgressBar bar;
    bar.setValue(200);
    REQUIRE(bar.getValue() == bar.getMaxValue());
}

TEST_CASE("ImageProgressBar setValue clamps to zero", "[unit][imageprogressbar]")
{
    ImageProgressBar bar;
    bar.setValue(-1);
    REQUIRE(bar.getValue() == 0);
    bar.setValue(-100);
    REQUIRE(bar.getValue() == 0);
}

TEST_CASE("ImageProgressBar setMaxValue and getMaxValue", "[unit][imageprogressbar]")
{
    ImageProgressBar bar;
    bar.setMaxValue(200);
    REQUIRE(bar.getMaxValue() == 200);
    bar.setMaxValue(1);
    REQUIRE(bar.getMaxValue() == 1);
}

TEST_CASE("ImageProgressBar setMaxValue positive assert only in debug", "[unit][imageprogressbar]")
{
    ImageProgressBar bar;
    REQUIRE(bar.getMaxValue() == 100);
}

TEST_CASE("ImageProgressBar getBarImage defaults to null", "[unit][imageprogressbar]")
{
    ImageProgressBar bar;
    REQUIRE(bar.getBarImage() == nullptr);
}

TEST_CASE("ImageProgressBar setBarImage with null", "[unit][imageprogressbar]")
{
    ImageProgressBar bar;
    bar.setBarImage(nullptr);
    REQUIRE(bar.getBarImage() == nullptr);
}

TEST_CASE("ImageProgressBar getForegroundImage defaults to null", "[unit][imageprogressbar]")
{
    ImageProgressBar bar;
    REQUIRE(bar.getForegroundImage() == nullptr);
}

TEST_CASE("ImageProgressBar setForegroundImage with null", "[unit][imageprogressbar]")
{
    ImageProgressBar bar;
    bar.setForegroundImage(nullptr);
    REQUIRE(bar.getForegroundImage() == nullptr);
}

TEST_CASE("ImageProgressBar setOrientation Horizontal", "[unit][imageprogressbar]")
{
    ImageProgressBar bar;
    REQUIRE(bar.getOrientation() == ImageProgressBar::Orientation::Horizontal);
    bar.setOrientation(ImageProgressBar::Orientation::Vertical);
    REQUIRE(bar.getOrientation() == ImageProgressBar::Orientation::Vertical);
    bar.setOrientation(ImageProgressBar::Orientation::Horizontal);
    REQUIRE(bar.getOrientation() == ImageProgressBar::Orientation::Horizontal);
}

TEST_CASE("ImageProgressBar setOrientation throws on invalid value", "[unit][imageprogressbar]")
{
    ImageProgressBar bar;
    auto invalid = static_cast<ImageProgressBar::Orientation>(99);
    REQUIRE_THROWS_AS(bar.setOrientation(invalid), fcn::Exception);
    REQUIRE(bar.getOrientation() == ImageProgressBar::Orientation::Horizontal);
}

TEST_CASE("ImageProgressBar resizeToContent with no image", "[unit][imageprogressbar]")
{
    ImageProgressBar bar;
    bar.resizeToContent();
    int const expected = (2 * bar.getBorderSize()) + bar.getPaddingLeft() + bar.getPaddingRight();
    REQUIRE(bar.getWidth() == expected);
    REQUIRE(bar.getHeight() == expected);
}

TEST_CASE("ImageProgressBar adjustSize with no image", "[unit][imageprogressbar]")
{
    ImageProgressBar bar;
    bar.adjustSize();
    int const expected = (2 * bar.getBorderSize()) + bar.getPaddingLeft() + bar.getPaddingRight();
    REQUIRE(bar.getWidth() == expected);
    REQUIRE(bar.getHeight() == expected);
}

TEST_CASE("ImageProgressBar value clamped after maxValue changes", "[unit][imageprogressbar]")
{
    ImageProgressBar bar;
    bar.setValue(50);
    REQUIRE(bar.getValue() == 50);
    bar.setMaxValue(25);
    // value is not clamped automatically on maxValue change
    REQUIRE(bar.getValue() == 50);
    // clamp happens on next setValue
    bar.setValue(bar.getValue());
    REQUIRE(bar.getValue() == 25);
}
