// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2026 Fifengine contributors

// Corresponding header include
#include "fifechan/graphics.hpp"

// Standard library includes
#include <memory>
#include <string>
#include <vector>

// Third-party library includes
#include <catch2/catch_test_macros.hpp>

// Project headers (subdirs before local)
#include "fifechan/color.hpp"
#include "fifechan/exception.hpp"
#include "fifechan/font.hpp"
#include "fifechan/point.hpp"

using fcn::Graphics;
using fcn::Image;
using fcn::Color;
using fcn::Font;
using fcn::Point;
using fcn::PointVector;
using fcn::Rectangle;
using fcn::ClipRectangle;

// Mock font for testing Graphics::drawText
class MockFont : public Font
{
    public:
        bool drawStringCalled = false;
        explicit MockFont(int charWidth = 10, int height = 16) : m_charWidth(charWidth), m_height(height)
        {
        }

        int getWidth(std::string const & text) const override
        {
            return static_cast<int>(text.size()) * m_charWidth;
        }

        int getHeight() const override
        {
            return m_height;
        }

        void drawString(Graphics* /*graphics*/, std::string const & /*text*/, int /*x*/, int /*y*/) override
        {
            drawStringCalled = true;
        }

    private:
        int m_charWidth;
        int m_height;
};

// Mock Graphics implementation for testing base class functionality
class MockGraphics : public Graphics
{
    public:
        MockGraphics() : m_color(255, 255, 255, 255)
        {
        }

        // Pure virtual methods from Graphics - minimal implementations
        void drawImage(
            Image const * /*image*/,
            int /*srcX*/,
            int /*srcY*/,
            int /*dstX*/,
            int /*dstY*/,
            int /*width*/,
            int /*height*/) override
        {
            // Mock implementation - do nothing
        }

        void drawPoint(int /*x*/, int /*y*/) override
        {
            // Mock implementation - do nothing
        }

        void drawLine(int /*x1*/, int /*y1*/, int /*x2*/, int /*y2*/) override
        {
            // Mock implementation - do nothing
        }

        void drawLine(int /*x1*/, int /*y1*/, int /*x2*/, int /*y2*/, unsigned int /*width*/) override
        {
            // Mock implementation - do nothing
        }

        void drawPolyLine(PointVector const & /*points*/, unsigned int /*width*/) override
        {
            // Mock implementation - do nothing
        }

        void drawBezier(PointVector const & /*points*/, int /*steps*/, unsigned int /*width*/) override
        {
            // Mock implementation - do nothing
        }

        void drawRectangle(Rectangle const & /*rectangle*/) override
        {
            // Mock implementation - do nothing
        }

        void fillRectangle(Rectangle const & /*rectangle*/) override
        {
            // Mock implementation - do nothing
        }

        void drawCircle(Point const & /*p*/, unsigned int /*radius*/) override
        {
            // Mock implementation - do nothing
        }

        void drawFillCircle(Point const & /*p*/, unsigned int /*radius*/) override
        {
            // Mock implementation - do nothing
        }

        void drawCircleSegment(Point const & /*p*/, unsigned int /*radius*/, int /*sangle*/, int /*eangle*/) override
        {
            // Mock implementation - do nothing
        }

        void drawFillCircleSegment(
            Point const & /*p*/, unsigned int /*radius*/, int /*sangle*/, int /*eangle*/) override
        {
            // Mock implementation - do nothing
        }

        void setColor(Color const & color) override
        {
            m_color = color;
        }

        Color const & getColor() const override
        {
            return m_color;
        }

    private:
        Color m_color;
};

// ============================================
// Tests for pushClipArea
// ============================================

TEST_CASE("Graphics::pushClipArea with empty rect pushes empty clip rect", "[graphics][pushClipArea]")
{
    MockGraphics g;

    // Test with width=0
    Rectangle emptyWidthRect(10, 20, 0, 30);
    bool result = g.pushClipArea(emptyWidthRect);
    REQUIRE(result == true);
    ClipRectangle const & clip = g.getCurrentClipArea();
    REQUIRE(clip.isEmpty() == true);

    g.popClipArea();

    // Test with height=0
    Rectangle emptyHeightRect(10, 20, 30, 0);
    result = g.pushClipArea(emptyHeightRect);
    REQUIRE(result == true);
    ClipRectangle const & clip2 = g.getCurrentClipArea();
    REQUIRE(clip2.isEmpty() == true);
}

TEST_CASE("Graphics::pushClipArea with normal rect when stack empty sets xOffset/yOffset", "[graphics][pushClipArea]")
{
    MockGraphics g;

    Rectangle rect(10, 20, 100, 200);
    bool result = g.pushClipArea(rect);

    REQUIRE(result == true);
    ClipRectangle const & clip = g.getCurrentClipArea();
    REQUIRE(clip.x == 10);
    REQUIRE(clip.y == 20);
    REQUIRE(clip.width == 100);
    REQUIRE(clip.height == 200);
    REQUIRE(clip.xOffset == 10);
    REQUIRE(clip.yOffset == 20);
}

TEST_CASE("Graphics::pushClipArea with nested clips computes intersection", "[graphics][pushClipArea]")
{
    MockGraphics g;

    // Push first clip area
    Rectangle rect1(10, 20, 100, 200);
    g.pushClipArea(rect1);

    // Push second clip area that intersects
    Rectangle rect2(50, 60, 100, 100);
    bool result = g.pushClipArea(rect2);

    REQUIRE(result == true);
    ClipRectangle const & clip = g.getCurrentClipArea();
    // After first push: clip = (10,20,100,200) with offset (10,20)
    // After second push:
    //   xOffset = 10 + 50 = 60, yOffset = 20 + 60 = 80
    //   x = 50 + 10 = 60, y = 60 + 20 = 80
    //   Intersection with (10,20,100,200):
    //     x=max(10,60)=60, y=max(20,80)=80
    //     width=min(110,160)-60=50, height=min(220,180)-80=100
    REQUIRE(clip.x == 60);
    REQUIRE(clip.y == 80);
    REQUIRE(clip.width == 50);
    REQUIRE(clip.height == 100);
    REQUIRE(clip.xOffset == 60); // 10 + 50
    REQUIRE(clip.yOffset == 80); // 20 + 60
}

TEST_CASE("Graphics::pushClipArea returns false when intersection empty", "[graphics][pushClipArea]")
{
    MockGraphics g;

    // Push first clip area
    Rectangle rect1(10, 10, 50, 50);
    g.pushClipArea(rect1);

    // Push second clip area that does NOT intersect (outside to the right)
    Rectangle rect2(100, 10, 50, 50); // x=100+10=110, which is outside rect1 (10-60)
    bool result = g.pushClipArea(rect2);

    REQUIRE(result == false);
    ClipRectangle const & clip = g.getCurrentClipArea();
    REQUIRE(clip.isEmpty() == true);
}

TEST_CASE("Graphics::pushClipArea nested offsets accumulate correctly", "[graphics][pushClipArea]")
{
    MockGraphics g;

    // Push first clip area at (10, 20)
    g.pushClipArea(Rectangle(10, 20, 100, 100));

    // Push second clip area at (5, 5) relative to first
    g.pushClipArea(Rectangle(5, 5, 50, 50));

    // Push third clip area at (2, 2) relative to second
    g.pushClipArea(Rectangle(2, 2, 20, 20));

    ClipRectangle const & clip = g.getCurrentClipArea();
    // xOffset should be 10 (first) + 5 (second) + 2 (third) = 17... wait
    // Actually xOffset accumulates: first push sets xOffset=10
    // second push: xOffset = 10 + 5 = 15, then x is adjusted to 5+10=15
    // third push: xOffset = 15 + 2 = 17, then x is adjusted to 2+15=17
    REQUIRE(clip.xOffset == 17);
    REQUIRE(clip.yOffset == 27); // 20 + 5 + 2 = 27
    REQUIRE(clip.x == 17);
    REQUIRE(clip.y == 27);
}

// ============================================
// Tests for popClipArea
// ============================================

TEST_CASE("Graphics::popClipArea removes top when stack not empty", "[graphics][popClipArea]")
{
    MockGraphics g;

    g.pushClipArea(Rectangle(10, 10, 100, 100));
    g.pushClipArea(Rectangle(5, 5, 50, 50));

    // Before pop
    ClipRectangle const & clipBefore = g.getCurrentClipArea();
    REQUIRE(clipBefore.x == 15); // 10+5
    REQUIRE(clipBefore.y == 15); // 10+5

    // Pop
    g.popClipArea();

    // After pop, should be back to first clip
    ClipRectangle const & clipAfter = g.getCurrentClipArea();
    REQUIRE(clipAfter.x == 10);
    REQUIRE(clipAfter.y == 10);
}

TEST_CASE("Graphics::popClipArea throws when stack empty", "[graphics][popClipArea]")
{
    MockGraphics g;

    REQUIRE_THROWS_AS(g.popClipArea(), fcn::Exception);
}

// ============================================
// Tests for getCurrentClipArea
// ============================================

TEST_CASE("Graphics::getCurrentClipArea returns top when stack not empty", "[graphics][getCurrentClipArea]")
{
    MockGraphics g;

    g.pushClipArea(Rectangle(10, 20, 100, 200));
    g.pushClipArea(Rectangle(5, 5, 50, 50));

    ClipRectangle const & clip = g.getCurrentClipArea();
    REQUIRE(clip.x == 15); // 10+5
    REQUIRE(clip.y == 25); // 20+5
}

TEST_CASE("Graphics::getCurrentClipArea throws when stack empty", "[graphics][getCurrentClipArea]")
{
    MockGraphics g;

    REQUIRE_THROWS_AS(g.getCurrentClipArea(), fcn::Exception);
}

// ============================================
// Tests for setFont
// ============================================

TEST_CASE("Graphics::setFont with nullptr", "[graphics][setFont]")
{
    MockGraphics g;

    g.setFont(nullptr);

    // Access the font through drawText which should throw
    REQUIRE_THROWS_AS(g.drawText("test", 0, 0), fcn::Exception);
}

TEST_CASE("Graphics::setFont with valid font pointer", "[graphics][setFont]")
{
    MockGraphics g;
    MockFont font(10, 16);

    g.setFont(&font);

    // drawText should not throw now
    REQUIRE_NOTHROW(g.drawText("test", 0, 0));
}

// ============================================
// Tests for drawText
// ============================================

TEST_CASE("Graphics::drawText with null font throws", "[graphics][drawText]")
{
    MockGraphics g;

    g.setFont(nullptr);

    REQUIRE_THROWS_AS(g.drawText("Hello", 0, 0), fcn::Exception);
    REQUIRE_THROWS_AS(g.drawText("Hello", 0, 0, Graphics::Alignment::Left), fcn::Exception);
    REQUIRE_THROWS_AS(g.drawText("Hello", 0, 0, Graphics::Alignment::Center), fcn::Exception);
    REQUIRE_THROWS_AS(g.drawText("Hello", 0, 0, Graphics::Alignment::Right), fcn::Exception);
}

TEST_CASE("Graphics::drawText Left alignment", "[graphics][drawText]")
{
    MockGraphics g;
    MockFont font(10, 16);
    g.setFont(&font);

    // Should not throw - Left alignment draws at x, y
    REQUIRE_NOTHROW(g.drawText("Hello", 100, 200, Graphics::Alignment::Left));
}

TEST_CASE("Graphics::drawText Center alignment", "[graphics][drawText]")
{
    MockGraphics g;
    MockFont font(10, 16);
    g.setFont(&font);

    // "Hello" = 5 chars * 10px = 50px width
    // Center: x - (width/2) = 100 - 25 = 75
    REQUIRE_NOTHROW(g.drawText("Hello", 100, 200, Graphics::Alignment::Center));
}

TEST_CASE("Graphics::drawText Right alignment", "[graphics][drawText]")
{
    MockGraphics g;
    MockFont font(10, 16);
    g.setFont(&font);

    // "Hello" = 5 chars * 10px = 50px width
    // Right: x - width = 100 - 50 = 50
    REQUIRE_NOTHROW(g.drawText("Hello", 100, 200, Graphics::Alignment::Right));
}

TEST_CASE("Graphics::drawText unknown alignment throws", "[graphics][drawText]")
{
    MockGraphics g;
    MockFont font(10, 16);
    g.setFont(&font);

    // Cast an invalid value to Alignment to test unknown alignment
    Graphics::Alignment invalidAlignment = static_cast<Graphics::Alignment>(255);
    REQUIRE_THROWS_AS(g.drawText("Hello", 100, 200, invalidAlignment), fcn::Exception);
}

TEST_CASE("Graphics::drawText default overload uses Left alignment", "[graphics][drawText]")
{
    MockGraphics g;
    MockFont font(10, 16);
    g.setFont(&font);

    // Default drawText calls drawText with Alignment::Left
    REQUIRE_NOTHROW(g.drawText("Hello", 100, 200));
}

// ============================================
// Tests for createFont
// ============================================

TEST_CASE("Graphics::createFont returns nullptr", "[graphics][createFont]")
{
    MockGraphics g;

    auto font = g.createFont("nonexistent.ttf", 12);
    REQUIRE(font == nullptr);
}

TEST_CASE("Graphics::createFont with empty filename returns nullptr", "[graphics][createFont]")
{
    MockGraphics g;

    auto font = g.createFont("", 0);
    REQUIRE(font == nullptr);
}

// ============================================
// Tests for color methods (via MockGraphics)
// ============================================

TEST_CASE("Graphics::setColor and getColor work correctly", "[graphics][color]")
{
    MockGraphics g;

    Color red(255, 0, 0, 255);
    g.setColor(red);
    REQUIRE(g.getColor() == red);

    Color blue(0, 0, 255, 128);
    g.setColor(blue);
    REQUIRE(g.getColor() == blue);
}

// ============================================
// Integration tests
// ============================================

TEST_CASE("Graphics clip stack operations sequence", "[graphics][integration]")
{
    MockGraphics g;

    // Start with empty stack
    REQUIRE_THROWS_AS(g.getCurrentClipArea(), fcn::Exception);

    // Push first area
    g.pushClipArea(Rectangle(0, 0, 100, 100));
    ClipRectangle const & clip1 = g.getCurrentClipArea();
    REQUIRE(clip1.x == 0);
    REQUIRE(clip1.y == 0);
    REQUIRE(clip1.width == 100);
    REQUIRE(clip1.height == 100);

    // Push nested area
    g.pushClipArea(Rectangle(10, 10, 50, 50));
    ClipRectangle const & clip2 = g.getCurrentClipArea();
    REQUIRE(clip2.x == 10);
    REQUIRE(clip2.y == 10);
    REQUIRE(clip2.width == 50);
    REQUIRE(clip2.height == 50);

    // Pop back to first
    g.popClipArea();
    ClipRectangle const & clip3 = g.getCurrentClipArea();
    REQUIRE(clip3.x == 0);
    REQUIRE(clip3.y == 0);

    // Pop to empty
    g.popClipArea();
    REQUIRE_THROWS_AS(g.getCurrentClipArea(), fcn::Exception);
}
