// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2026 Fifengine contributors

// Corresponding header include
#include "fifechan/widgets/iconprogressbar.hpp"

// Third-party library includes
#include <catch2/catch_test_macros.hpp>

//
#include "fifechan/exception.hpp"
#include "fifechan/graphics.hpp"
#include "fifechan/image.hpp"

using fcn::IconProgressBar;

namespace
{
    class MockGraphics : public fcn::Graphics
    {
        public:
            void setColor(fcn::Color const & /*color*/) override
            {
            }
            fcn::Color const & getColor() const override
            {
                static fcn::Color const c;
                return c;
            }
            void drawPoint(int /*x*/, int /*y*/) override
            {
            }
            void drawLine(int /*x1*/, int /*y1*/, int /*x2*/, int /*y2*/) override
            {
            }
            void drawLine(int /*x1*/, int /*y1*/, int /*x2*/, int /*y2*/, unsigned int /*width*/) override
            {
            }
            void drawPolyLine(fcn::PointVector const & /*points*/, unsigned int /*width*/) override
            {
            }
            void drawBezier(fcn::PointVector const & /*points*/, int /*steps*/, unsigned int /*width*/) override
            {
            }
            void drawRectangle(fcn::Rectangle const & /*rectangle*/) override
            {
            }
            void fillRectangle(fcn::Rectangle const & /*rectangle*/) override
            {
            }
            void drawCircle(fcn::Point const & /*p*/, unsigned int /*radius*/) override
            {
            }
            void drawFillCircle(fcn::Point const & /*p*/, unsigned int /*radius*/) override
            {
            }
            void drawCircleSegment(
                fcn::Point const & /*p*/, unsigned int /*radius*/, int /*sangle*/, int /*eangle*/) override
            {
            }
            void drawFillCircleSegment(
                fcn::Point const & /*p*/, unsigned int /*radius*/, int /*sangle*/, int /*eangle*/) override
            {
            }
            void drawImage(fcn::Image const * /*image*/, int /*dstX*/, int /*dstY*/) override
            {
            }
            void drawImage(
                fcn::Image const * /*image*/,
                int /*srcX*/,
                int /*srcY*/,
                int /*dstX*/,
                int /*dstY*/,
                int /*width*/,
                int /*height*/) override
            {
            }

            void drawSurface(SDL_Surface* /*surface*/, int /*dstX*/, int /*dstY*/) override
            {
            }

            void fillTriangle(int /*x1*/, int /*y1*/, int /*x2*/, int /*y2*/, int /*x3*/, int /*y3*/) override
            {
            }
    };

    class MockImage : public fcn::Image
    {
            int mWidth;
            int mHeight;

        public:
            MockImage(int w, int h) : mWidth(w), mHeight(h)
            {
            }
            void free() override
            {
            }
            int getWidth() const override
            {
                return mWidth;
            }
            int getHeight() const override
            {
                return mHeight;
            }
            fcn::Color getPixel(int /*x*/, int /*y*/) override
            {
                return {};
            }
            void putPixel(int /*x*/, int /*y*/, fcn::Color const & /*color*/) override
            {
            }
            void convertToDisplayFormat() override
            {
            }
    };
} // namespace

TEST_CASE("IconProgressBar default construction", "[unit][iconprogressbar]")
{
    IconProgressBar bar;
    REQUIRE(bar.getImage() == nullptr);
    REQUIRE(bar.getMaxIcons() == 0);
    REQUIRE(bar.getIconCount() == 0);
    REQUIRE(bar.getOrientation() == IconProgressBar::Orientation::Horizontal);
    REQUIRE(bar.isOpaque());
}

TEST_CASE("IconProgressBar setMaxIcons and getMaxIcons", "[unit][iconprogressbar]")
{
    IconProgressBar bar;
    bar.setMaxIcons(5);
    REQUIRE(bar.getMaxIcons() == 5);

    bar.setMaxIcons(0);
    REQUIRE(bar.getMaxIcons() == 0);
}

TEST_CASE("IconProgressBar setOrientation and getOrientation", "[unit][iconprogressbar]")
{
    IconProgressBar bar;
    REQUIRE(bar.getOrientation() == IconProgressBar::Orientation::Horizontal);

    bar.setOrientation(IconProgressBar::Orientation::Vertical);
    REQUIRE(bar.getOrientation() == IconProgressBar::Orientation::Vertical);

    bar.setOrientation(IconProgressBar::Orientation::Horizontal);
    REQUIRE(bar.getOrientation() == IconProgressBar::Orientation::Horizontal);
}

TEST_CASE("IconProgressBar setOpaque and isOpaque", "[unit][iconprogressbar]")
{
    IconProgressBar bar;
    bar.setOpaque(false);
    REQUIRE_FALSE(bar.isOpaque());

    bar.setOpaque(true);
    REQUIRE(bar.isOpaque());
}

TEST_CASE("IconProgressBar setImage and getImage", "[unit][iconprogressbar]")
{
    IconProgressBar bar;
    REQUIRE(bar.getImage() == nullptr);

    MockImage img(16, 16);
    bar.setImage(&img);
    REQUIRE(bar.getImage() == &img);

    bar.setImage(nullptr);
    REQUIRE(bar.getImage() == nullptr);
}

TEST_CASE("IconProgressBar advance increments counter", "[unit][iconprogressbar]")
{
    IconProgressBar bar;
    bar.setMaxIcons(3);
    REQUIRE(bar.getIconCount() == 0);

    bar.advance();
    REQUIRE(bar.getIconCount() == 1);

    bar.advance();
    REQUIRE(bar.getIconCount() == 2);

    bar.advance();
    REQUIRE(bar.getIconCount() == 3);

    bar.advance();
    REQUIRE(bar.getIconCount() == 0);
}

TEST_CASE("IconProgressBar advance with maxIcons 0 stays at 0", "[unit][iconprogressbar]")
{
    IconProgressBar bar;
    bar.setMaxIcons(0);
    bar.advance();
    REQUIRE(bar.getIconCount() == 0);
    bar.advance();
    REQUIRE(bar.getIconCount() == 0);
}

TEST_CASE("IconProgressBar reset sets counter to 0", "[unit][iconprogressbar]")
{
    IconProgressBar bar;
    bar.setMaxIcons(5);
    bar.advance();
    bar.advance();
    REQUIRE(bar.getIconCount() == 2);

    bar.reset();
    REQUIRE(bar.getIconCount() == 0);
}

TEST_CASE("IconProgressBar setIconCount", "[unit][iconprogressbar]")
{
    IconProgressBar bar;
    bar.setMaxIcons(5);

    bar.setIconCount(3);
    REQUIRE(bar.getIconCount() == 3);

    bar.setIconCount(0);
    REQUIRE(bar.getIconCount() == 0);
}

TEST_CASE("IconProgressBar setIconCount wraps around maxIcons", "[unit][iconprogressbar]")
{
    IconProgressBar bar;
    bar.setMaxIcons(3);

    bar.setIconCount(4);
    REQUIRE(bar.getIconCount() == 0);

    bar.setIconCount(5);
    REQUIRE(bar.getIconCount() == 1);

    bar.setIconCount(7);
    REQUIRE(bar.getIconCount() == 3);
}

TEST_CASE("IconProgressBar setIconCount with maxIcons 0", "[unit][iconprogressbar]")
{
    IconProgressBar bar;
    bar.setMaxIcons(0);

    bar.setIconCount(0);
    REQUIRE(bar.getIconCount() == 0);
}

TEST_CASE("IconProgressBar resizeToContent with no image", "[unit][iconprogressbar]")
{
    IconProgressBar bar;
    bar.resizeToContent(false);
    // Size should be borders(0) + padding(0) = 0
    REQUIRE(bar.getWidth() == 0);
    REQUIRE(bar.getHeight() == 0);
}

TEST_CASE("IconProgressBar adjustSize with no image", "[unit][iconprogressbar]")
{
    IconProgressBar bar;
    bar.adjustSize();
    REQUIRE(bar.getWidth() == 0);
    REQUIRE(bar.getHeight() == 0);
}

TEST_CASE("IconProgressBar adjustSizeImpl with image sizes correctly", "[unit][iconprogressbar]")
{
    IconProgressBar bar;
    MockImage img(10, 20);
    bar.setImage(&img);
    bar.setMaxIcons(3);

    // Horizontal: w = 10*3 + borders(0) + padding(0) = 30, h = 20 + 0 = 20
    REQUIRE(bar.getWidth() == 30);
    REQUIRE(bar.getHeight() == 20);
}

TEST_CASE("IconProgressBar adjustSizeImpl with vertical orientation", "[unit][iconprogressbar]")
{
    IconProgressBar bar;
    MockImage img(10, 20);
    bar.setImage(&img);
    bar.setMaxIcons(3);
    bar.setOrientation(IconProgressBar::Orientation::Vertical);

    // Vertical: w = 10 + 0 = 10, h = 20*3 + 0 = 60
    REQUIRE(bar.getWidth() == 10);
    REQUIRE(bar.getHeight() == 60);
}

TEST_CASE("IconProgressBar adjustSizeImpl with border and padding", "[unit][iconprogressbar]")
{
    IconProgressBar bar;
    MockImage img(8, 8);
    bar.setImage(&img);
    bar.setMaxIcons(2);
    bar.setBorderSize(2);
    bar.setPaddingLeft(3);
    bar.setPaddingRight(4);
    bar.setPaddingTop(5);
    bar.setPaddingBottom(6);

    bar.adjustSize();
    // Horizontal: w = 8*2 + 2*2 + 3+4 = 16+4+7 = 27
    // h = 8 + 2*2 + 5+6 = 8+4+11 = 23
    REQUIRE(bar.getWidth() == 27);
    REQUIRE(bar.getHeight() == 23);
}

TEST_CASE("IconProgressBar draw with opaque", "[unit][iconprogressbar]")
{
    IconProgressBar bar;
    bar.setSize(100, 30);
    MockGraphics g;
    bar.draw(&g);
}

TEST_CASE("IconProgressBar draw with non-opaque", "[unit][iconprogressbar]")
{
    IconProgressBar bar;
    bar.setOpaque(false);
    bar.setSize(100, 30);
    MockGraphics g;
    bar.draw(&g);
}

TEST_CASE("IconProgressBar draw with image", "[unit][iconprogressbar]")
{
    IconProgressBar bar;
    MockImage img(8, 8);
    bar.setImage(&img);
    bar.setMaxIcons(3);
    bar.setIconCount(2);
    bar.setBorderSize(0);
    MockGraphics g;
    bar.draw(&g);
}

TEST_CASE("IconProgressBar resizeToContent with image", "[unit][iconprogressbar]")
{
    IconProgressBar bar;
    MockImage img(12, 12);
    bar.setImage(&img);
    bar.setMaxIcons(4);
    bar.resizeToContent(false);
    REQUIRE(bar.getWidth() == 12 * 4);
    REQUIRE(bar.getHeight() == 12);
}

TEST_CASE("IconProgressBar draw focused with selection background", "[unit][iconprogressbar]")
{
    IconProgressBar bar;
    bar.setFocused(true);
    bar.setSelectionMode(fcn::Widget::SelectionMode::Background);
    bar.setSize(100, 30);
    MockGraphics g;
    bar.draw(&g);
}

TEST_CASE("IconProgressBar draw with border and selection", "[unit][iconprogressbar]")
{
    IconProgressBar bar;
    bar.setFocused(true);
    bar.setSelectionMode(fcn::Widget::SelectionMode::Border);
    bar.setBorderSize(2);
    bar.setSize(100, 30);
    MockGraphics g;
    bar.draw(&g);
}

TEST_CASE("IconProgressBar draw vertical with icons", "[unit][iconprogressbar]")
{
    IconProgressBar bar;
    MockImage img(8, 8);
    bar.setImage(&img);
    bar.setMaxIcons(3);
    bar.setIconCount(2);
    bar.setOrientation(IconProgressBar::Orientation::Vertical);
    MockGraphics g;
    bar.draw(&g);
}

TEST_CASE("IconProgressBar draw with border and no selection", "[unit][iconprogressbar]")
{
    IconProgressBar bar;
    bar.setFocused(false);
    bar.setBorderSize(2);
    bar.setSize(100, 30);
    MockGraphics g;
    bar.draw(&g);
}

TEST_CASE("IconProgressBar setOrientation same value does nothing", "[unit][iconprogressbar]")
{
    IconProgressBar bar;
    bar.setOrientation(IconProgressBar::Orientation::Horizontal);
    REQUIRE(bar.getOrientation() == IconProgressBar::Orientation::Horizontal);
    // Size unchanged
    REQUIRE(bar.getWidth() == 0);
}

TEST_CASE("IconProgressBar constructor with image and maxIcons", "[unit][iconprogressbar]")
{
    MockImage img(16, 24);
    IconProgressBar bar(&img, 5);
    REQUIRE(bar.getImage() == &img);
    REQUIRE(bar.getMaxIcons() == 5);
    REQUIRE(bar.getIconCount() == 0);
    REQUIRE(bar.getOrientation() == IconProgressBar::Orientation::Horizontal);
    REQUIRE(bar.getWidth() == 16 * 5);
    REQUIRE(bar.getHeight() == 24);
}

TEST_CASE("IconProgressBar setImage to null after non-null", "[unit][iconprogressbar]")
{
    IconProgressBar bar;
    MockImage img(8, 8);
    bar.setImage(&img);
    REQUIRE(bar.getImage() == &img);

    bar.setImage(nullptr);
    REQUIRE(bar.getImage() == nullptr);
}

TEST_CASE("IconProgressBar advance does not wrap when maxIcons is 1", "[unit][iconprogressbar]")
{
    IconProgressBar bar;
    bar.setMaxIcons(1);
    REQUIRE(bar.getIconCount() == 0);

    bar.advance();
    REQUIRE(bar.getIconCount() == 1);

    bar.advance();
    REQUIRE(bar.getIconCount() == 0);
}
