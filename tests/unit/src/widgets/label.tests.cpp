// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2013 - 2026 Fifengine contributors

// Standard library includes
#include <string>
#include <vector>

// Third-party library includes
#include <catch2/catch_test_macros.hpp>

// Project headers (subdirs before local)
#include "fifechan/color.hpp"
#include "fifechan/defaultfont.hpp"
#include "fifechan/font.hpp"
#include "fifechan/graphics.hpp"
#include "fifechan/image.hpp"
#include "fifechan/point.hpp"
#include "fifechan/rectangle.hpp"
#include "fifechan/widgets/label.hpp"

using fcn::Color;
using fcn::DefaultFont;
using fcn::Font;
using fcn::Graphics;
using fcn::Image;
using fcn::Label;
using fcn::Point;
using fcn::PointVector;
using fcn::Rectangle;
// `Alignment` is defined under `fcn::Graphics`
using Alignment = fcn::Graphics::Alignment;

namespace
{
    // Minimal mock implementing all pure virtual methods of fcn::Graphics
    class MockGraphics : public Graphics
    {
        public:
            // drawText override (virtual in base class)
            void drawText(std::string const & text, int x, int y, Alignment alignment) override
            {
                (void)text;
                (void)x;
                (void)y;
                (void)alignment;
            }

            // setFont override (virtual in base class)
            void setFont(Font* font) override
            {
                (void)font;
            }

            // setColor (pure virtual in base)
            void setColor(Color const & color) override
            {
                (void)color;
            }

            // getColor (pure virtual in base)
            Color const & getColor() const override
            {
                static Color c;
                return c;
            }

            // drawPoint (pure virtual)
            void drawPoint(int x, int y) override
            {
                (void)x;
                (void)y;
            }

            // drawLine single width (pure virtual)
            void drawLine(int x1, int y1, int x2, int y2) override
            {
                (void)x1;
                (void)y1;
                (void)x2;
                (void)y2;
            }

            // drawLine with width (pure virtual)
            void drawLine(int x1, int y1, int x2, int y2, unsigned int width) override
            {
                (void)x1;
                (void)y1;
                (void)x2;
                (void)y2;
                (void)width;
            }

            // drawPolyLine (pure virtual)
            void drawPolyLine(PointVector const & points, unsigned int width) override
            {
                (void)points;
                (void)width;
            }

            // drawBezier (pure virtual)
            void drawBezier(PointVector const & points, int steps, unsigned int width) override
            {
                (void)points;
                (void)steps;
                (void)width;
            }

            // drawRectangle (pure virtual)
            void drawRectangle(Rectangle const & rectangle) override
            {
                (void)rectangle;
            }

            // fillRectangle (pure virtual)
            void fillRectangle(Rectangle const & rectangle) override
            {
                (void)rectangle;
            }

            // drawCircle (pure virtual)
            void drawCircle(Point const & p, unsigned int radius) override
            {
                (void)p;
                (void)radius;
            }

            // drawFillCircle (pure virtual)
            void drawFillCircle(Point const & p, unsigned int radius) override
            {
                (void)p;
                (void)radius;
            }

            // drawCircleSegment (pure virtual)
            void drawCircleSegment(Point const & p, unsigned int radius, int sangle, int eangle) override
            {
                (void)p;
                (void)radius;
                (void)sangle;
                (void)eangle;
            }

            // drawFillCircleSegment (pure virtual)
            void drawFillCircleSegment(Point const & p, unsigned int radius, int sangle, int eangle) override
            {
                (void)p;
                (void)radius;
                (void)sangle;
                (void)eangle;
            }

            // drawImage full version (pure virtual)
            void drawImage(Image const * image, int srcX, int srcY, int dstX, int dstY, int width, int height) override
            {
                (void)image;
                (void)srcX;
                (void)srcY;
                (void)dstX;
                (void)dstY;
                (void)width;
                (void)height;
            }

            // pushClipArea (virtual, returns bool)
            bool pushClipArea(Rectangle area) override
            {
                (void)area;
                return true;
            }

            // popClipArea (virtual)
            void popClipArea() override
            {
            }

            // getCurrentClipArea (virtual, returns ClipRectangle const&)
            fcn::ClipRectangle const & getCurrentClipArea() override
            {
                static fcn::ClipRectangle r;
                return r;
            }
    };
} // namespace

// ============================================================================
// TEST CASE 1: Label constructors
// ============================================================================
TEST_CASE("Label constructors", "[unit][label]")
{
    SECTION("Default constructor: caption empty, alignment Left, vertical Center")
    {
        // Note: Label constructor calls adjustSizeImpl() which requires a font.
        // We set a font immediately after construction to avoid UB from nullptr font in constructor.
        Label label;
        DefaultFont font;
        label.setFont(&font);

        REQUIRE(label.getCaption().empty());
        REQUIRE(label.getAlignment() == Graphics::Alignment::Left);
        REQUIRE(label.getVerticalAlignment() == Graphics::VerticalAlignment::Center);
    }

    SECTION("Constructor with text: caption set, alignment Left, vertical Center")
    {
        Label label("Test Caption");
        DefaultFont font;
        label.setFont(&font);

        REQUIRE(label.getCaption() == "Test Caption");
        REQUIRE(label.getAlignment() == Graphics::Alignment::Left);
        REQUIRE(label.getVerticalAlignment() == Graphics::VerticalAlignment::Center);
    }
}

// ============================================================================
// TEST CASE 2: Label caption get/set
// ============================================================================
TEST_CASE("Label caption get/set", "[unit][label]")
{
    Label label;
    DefaultFont font;
    label.setFont(&font);

    SECTION("getCaption returns empty for default")
    {
        REQUIRE(label.getCaption().empty());
    }

    SECTION("setCaption then getCaption returns new value")
    {
        label.setCaption("Hello World");
        REQUIRE(label.getCaption() == "Hello World");
    }

    SECTION("setCaption with empty string")
    {
        label.setCaption("Temporary");
        label.setCaption("");
        REQUIRE(label.getCaption().empty());
    }

    SECTION("setCaption with multi-line (\\n separated)")
    {
        label.setCaption("Line1\nLine2\nLine3");
        REQUIRE(label.getCaption() == "Line1\nLine2\nLine3");
    }

    SECTION("setCaption calls adjustSizeImpl (verify size changes if font set)")
    {
        label.setCaption("Short");
        int const shortWidth  = label.getWidth();
        int const shortHeight = label.getHeight();

        label.setCaption("Much longer text");
        int const longWidth  = label.getWidth();
        int const longHeight = label.getHeight();

        REQUIRE(longWidth > shortWidth);
        REQUIRE(longHeight == shortHeight); // Same number of lines
    }
}

// ============================================================================
// TEST CASE 3: Label horizontal alignment
// ============================================================================
TEST_CASE("Label horizontal alignment", "[unit][label]")
{
    Label label;
    DefaultFont font;
    label.setFont(&font);

    SECTION("Default is Left")
    {
        REQUIRE(label.getAlignment() == Graphics::Alignment::Left);
    }

    SECTION("Set and get Center")
    {
        label.setAlignment(Graphics::Alignment::Center);
        REQUIRE(label.getAlignment() == Graphics::Alignment::Center);
    }

    SECTION("Set and get Right")
    {
        label.setAlignment(Graphics::Alignment::Right);
        REQUIRE(label.getAlignment() == Graphics::Alignment::Right);
    }

    SECTION("Set and get Left again")
    {
        label.setAlignment(Graphics::Alignment::Right);
        label.setAlignment(Graphics::Alignment::Left);
        REQUIRE(label.getAlignment() == Graphics::Alignment::Left);
    }
}

// ============================================================================
// TEST CASE 4: Label vertical alignment
// ============================================================================
TEST_CASE("Label vertical alignment", "[unit][label]")
{
    Label label;
    DefaultFont font;
    label.setFont(&font);

    SECTION("Default is Center")
    {
        REQUIRE(label.getVerticalAlignment() == Graphics::VerticalAlignment::Center);
    }

    SECTION("Set and get Top")
    {
        label.setVerticalAlignment(Graphics::VerticalAlignment::Top);
        REQUIRE(label.getVerticalAlignment() == Graphics::VerticalAlignment::Top);
    }

    SECTION("Set and get Bottom")
    {
        label.setVerticalAlignment(Graphics::VerticalAlignment::Bottom);
        REQUIRE(label.getVerticalAlignment() == Graphics::VerticalAlignment::Bottom);
    }

    SECTION("Set and get Center again")
    {
        label.setVerticalAlignment(Graphics::VerticalAlignment::Bottom);
        label.setVerticalAlignment(Graphics::VerticalAlignment::Center);
        REQUIRE(label.getVerticalAlignment() == Graphics::VerticalAlignment::Center);
    }
}

// ============================================================================
// TEST CASE 5: Label adjustSize calculates correct dimensions
// ============================================================================
TEST_CASE("Label adjustSize calculates correct dimensions", "[unit][label]")
{
    Label label;
    DefaultFont font;
    label.setFont(&font);
    label.setBorderSize(0);
    label.setPaddingLeft(0);
    label.setPaddingRight(0);
    label.setPaddingTop(0);
    label.setPaddingBottom(0);

    SECTION("Empty caption: width = 2*border + paddingLeft + paddingRight, height similar")
    {
        label.setCaption("");
        // Empty line: maxWidth = 0, lineCount = 1, height = 8
        // Width: 2*0 + 0 + 0 + 0 = 0
        // Height: 2*0 + 0 + 0 + 8 = 8
        REQUIRE(label.getWidth() == 0);
        REQUIRE(label.getHeight() == 8);
    }

    SECTION("Single line: width includes text width")
    {
        label.setCaption("Hello"); // 5 chars * 8 = 40
        REQUIRE(label.getWidth() == 40);
        REQUIRE(label.getHeight() == 8);
    }

    SECTION("Multi-line: height includes multiple lines")
    {
        label.setCaption("Hello\nWorld"); // 2 lines, each 40 width
        REQUIRE(label.getWidth() == 40);
        REQUIRE(label.getHeight() == 16); // 2 * 8
    }

    SECTION("After setCaption, size should update")
    {
        label.setCaption("A");
        int const widthA = label.getWidth(); // 8
        label.setCaption("BB");
        int const widthBB = label.getWidth(); // 16
        REQUIRE(widthBB > widthA);
    }

    SECTION("With non-zero border size")
    {
        label.setBorderSize(2);
        label.setCaption("Hi"); // 2 chars *8=16
        // Width: 2*2 +0 +0 +16 = 20
        // Height: 2*2 +0 +0 +8 = 12
        REQUIRE(label.getWidth() == 20);
        REQUIRE(label.getHeight() == 12);
    }

    SECTION("With non-zero padding")
    {
        label.setBorderSize(0);
        label.setPaddingLeft(3);
        label.setPaddingRight(5);
        label.setPaddingTop(2);
        label.setPaddingBottom(4);
        label.setCaption("A"); // width 8
        // Width: 0 +3 +5 +8 =16
        // Height:0 +2 +4 +8=14
        REQUIRE(label.getWidth() == 16);
        REQUIRE(label.getHeight() == 14);
    }
}

// ============================================================================
// TEST CASE 6: Label resizeToContent works
// ============================================================================
TEST_CASE("Label resizeToContent works", "[unit][label]")
{
    Label label;
    DefaultFont font;
    label.setFont(&font);

    SECTION("Same as adjustSize essentially")
    {
        label.setCaption("Test");
        label.adjustSize();
        int const adjustWidth  = label.getWidth();
        int const adjustHeight = label.getHeight();

        label.setCaption("Test");
        label.resizeToContent();
        REQUIRE(label.getWidth() == adjustWidth);
        REQUIRE(label.getHeight() == adjustHeight);
    }

    SECTION("Call with recursion=true")
    {
        label.setCaption("Recursion True"); // 14 chars
        label.resizeToContent(true);
        REQUIRE(label.getWidth() == 14 * 8); // 14 chars * 8
        REQUIRE(label.getHeight() == 8);
    }

    SECTION("Call with recursion=false")
    {
        label.setCaption("Recursion False"); // 15 chars
        label.resizeToContent(false);
        REQUIRE(label.getWidth() == 15 * 8); // 15 chars * 8
        REQUIRE(label.getHeight() == 8);
    }
}

// ============================================================================
// TEST CASE 7: Label draw does not crash
// ============================================================================
TEST_CASE("Label draw does not crash", "[unit][label]")
{
    Label label;
    DefaultFont font;
    label.setFont(&font);
    MockGraphics mockGraphics;

    SECTION("Basic draw call")
    {
        label.setCaption("Draw Test");
        label.draw(&mockGraphics);
    }

    SECTION("Draw with multi-line text")
    {
        label.setCaption("Line1\nLine2\nLine3");
        label.draw(&mockGraphics);
    }

    SECTION("Draw with different horizontal alignments")
    {
        label.setCaption("Aligned Text");
        label.setAlignment(Graphics::Alignment::Left);
        label.draw(&mockGraphics);

        label.setAlignment(Graphics::Alignment::Center);
        label.draw(&mockGraphics);

        label.setAlignment(Graphics::Alignment::Right);
        label.draw(&mockGraphics);
    }

    SECTION("Draw with different vertical alignments")
    {
        label.setCaption("Vert Text");
        label.setVerticalAlignment(Graphics::VerticalAlignment::Top);
        label.draw(&mockGraphics);

        label.setVerticalAlignment(Graphics::VerticalAlignment::Center);
        label.draw(&mockGraphics);

        label.setVerticalAlignment(Graphics::VerticalAlignment::Bottom);
        label.draw(&mockGraphics);
    }

    SECTION("Draw with border enabled")
    {
        label.setBorderSize(2);
        label.setCaption("Bordered");
        label.draw(&mockGraphics);
    }
}

// ============================================================================
// TEST CASE 8: Label border and padding affect size
// ============================================================================
TEST_CASE("Label border and padding affect size", "[unit][label]")
{
    Label label;
    DefaultFont font;
    label.setFont(&font);
    label.setCaption("Size Test"); // 9 chars *8=72 width, 8 height

    SECTION("Set border size, verify adjustSize includes it")
    {
        label.setBorderSize(0);
        label.adjustSize();
        int const noBorderWidth  = label.getWidth();
        int const noBorderHeight = label.getHeight();

        label.setBorderSize(3);
        label.adjustSize();
        // Width increases by 2*3=6, height increases by 2*3=6
        REQUIRE(label.getWidth() == noBorderWidth + 6);
        REQUIRE(label.getHeight() == noBorderHeight + 6);
    }

    SECTION("Set padding, verify adjustSize includes it")
    {
        label.setBorderSize(0);
        label.setPaddingLeft(0);
        label.setPaddingRight(0);
        label.setPaddingTop(0);
        label.setPaddingBottom(0);
        label.adjustSize();
        int const noPadWidth  = label.getWidth();
        int const noPadHeight = label.getHeight();

        label.setPaddingLeft(2);
        label.setPaddingRight(4);
        label.setPaddingTop(1);
        label.setPaddingBottom(3);
        label.adjustSize();
        // Width increases by 2+4=6, height increases by1+3=4
        REQUIRE(label.getWidth() == noPadWidth + 6);
        REQUIRE(label.getHeight() == noPadHeight + 4);
    }

    SECTION("Combined border and padding")
    {
        label.setBorderSize(2);
        label.setPaddingLeft(1);
        label.setPaddingRight(1);
        label.adjustSize();
        // Width: 2*2 +1+1 +72 =4+2+72=78
        REQUIRE(label.getWidth() == 78);
    }
}

// ============================================================================
// TEST CASE 9: Label multi-line caption handling
// ============================================================================
TEST_CASE("Label multi-line caption handling", "[unit][label]")
{
    Label label;
    DefaultFont font;
    label.setFont(&font);

    SECTION("Caption with \\n\\n (empty line)")
    {
        label.setCaption("A\n\nB"); // 3 lines: A, empty, B
        // maxWidth: max(8, 0, 8) =8
        // height: 3*8=24
        REQUIRE(label.getWidth() == 8);
        REQUIRE(label.getHeight() == 24);
    }

    SECTION("Verify line count in draw (indirectly via size)")
    {
        label.setCaption("1\n2\n3\n4\n5");   // 5 lines
        REQUIRE(label.getHeight() == 5 * 8); // 40
    }

    SECTION("Very long text")
    {
        std::string longText(100, 'x'); // 100 chars *8=800 width
        label.setCaption(longText);
        REQUIRE(label.getWidth() == 800);
        REQUIRE(label.getHeight() == 8);
    }
}

// ============================================================================
// TEST CASE 10: Label font inheritance
// ============================================================================
TEST_CASE("Label font inheritance", "[unit][label]")
{
    Label label;
    DefaultFont font1;
    DefaultFont font2;

    SECTION("setFont and getFont")
    {
        label.setFont(&font1);
        REQUIRE(label.getFont() == &font1);

        label.setFont(&font2);
        REQUIRE(label.getFont() == &font2);

        // setFont(nullptr) clears custom font; getFont() returns default font, not nullptr
        label.setFont(nullptr);
        REQUIRE(label.getFont() != nullptr); // Returns default font
    }

    SECTION("draw uses the set font")
    {
        label.setFont(&font1);
        MockGraphics mockGraphics;
        label.setCaption("Font Test");
        // Should not crash, uses font1
        label.draw(&mockGraphics);
    }

    SECTION("Font affects text size calculations")
    {
        // DefaultFont: 8x8 per char
        label.setFont(&font1);
        label.setCaption("ABC");
        int const widthFont1 = label.getWidth(); // 24

        // If we had another font with different size, width would change
        // Since we only have DefaultFont, verify 8 per char
        REQUIRE(widthFont1 == 24);
    }
}
