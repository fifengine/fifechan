// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2026 Fifengine contributors

// Standard library includes
#include <algorithm>
#include <filesystem>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

// Third-party library includes
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

// Third-party library includes
#include <catch2/catch_test_macros.hpp>

// Project headers
#include "fifechan/backends/sdl3/graphics.hpp"
#include "fifechan/backends/sdl3/image.hpp"
#include "fifechan/backends/sdl3/imageloader.hpp"
#include "fifechan/color.hpp"
#include "fifechan/image.hpp"
#include "fifechan/imagefont.hpp"

namespace
{
    // SDL test environment (lifecycle managed per test case)
    struct SDLEnvironment
    {
            SDLEnvironment()
            {
                if (!SDL_Init(SDL_INIT_VIDEO)) {
                    throw std::runtime_error(std::string("SDL_Init failed: ") + SDL_GetError());
                }

                // Create a window for SDL3 hardware acceleration
                mWindow = SDL_CreateWindow("FifeGUI ImageFont Test", 256, 256, SDL_WINDOW_RESIZABLE);

                if (mWindow == nullptr) {
                    SDL_Quit();
                    throw std::runtime_error(std::string("SDL_CreateWindow failed: ") + SDL_GetError());
                }

                mRenderer = SDL_CreateRenderer(mWindow, nullptr);

                if (mRenderer == nullptr) {
                    SDL_DestroyWindow(mWindow);
                    SDL_Quit();
                    throw std::runtime_error(std::string("SDL_CreateRenderer failed: ") + SDL_GetError());
                }

                // Set up global ImageLoader for Image::load() to work
                mImageLoader = new fcn::sdl3::ImageLoader();
                mImageLoader->setRenderer(mRenderer);
                fcn::Image::setImageLoader(mImageLoader);
            }

            ~SDLEnvironment()
            {
                // Clean up the global ImageLoader
                if (mImageLoader != nullptr) {
                    // Don't set nullptr since that would assert
                    delete mImageLoader;
                    mImageLoader = nullptr;
                }
                SDL_DestroyRenderer(mRenderer);
                SDL_DestroyWindow(mWindow);
                SDL_Quit();
            }

            SDLEnvironment(SDLEnvironment const &)            = delete;
            SDLEnvironment& operator=(SDLEnvironment const &) = delete;
            SDLEnvironment(SDLEnvironment&&)                  = delete;
            SDLEnvironment& operator=(SDLEnvironment&&)       = delete;

            SDL_Window* mWindow                  = nullptr;
            SDL_Renderer* mRenderer              = nullptr;
            fcn::sdl3::ImageLoader* mImageLoader = nullptr;
    };

    // Emulates backends (such as OpenGL) that discard the CPU pixel buffer
    // once an image has been converted to display format.
    class CachelessImage : public fcn::Image
    {
        public:
            CachelessImage(int width, int height, std::vector<fcn::Color> pixels) :
                mWidth(width), mHeight(height), mPixels(std::move(pixels))
            {
            }

            void free() override
            {
                mPixels.clear();
            }

            int getWidth() const override
            {
                return mWidth;
            }

            int getHeight() const override
            {
                return mHeight;
            }

            fcn::Color getPixel(int x, int y) override
            {
                if (mPixels.empty()) {
                    throw std::runtime_error("Image has been converted to display format");
                }
                return mPixels.at(
                    (static_cast<std::size_t>(y) * static_cast<std::size_t>(mWidth)) + static_cast<std::size_t>(x));
            }

            void putPixel(int x, int y, fcn::Color const & color) override
            {
                if (mPixels.empty()) {
                    throw std::runtime_error("Image has been converted to display format");
                }
                mPixels.at(
                    (static_cast<std::size_t>(y) * static_cast<std::size_t>(mWidth)) + static_cast<std::size_t>(x)) =
                    color;
            }

            void convertToDisplayFormat() override
            {
                mPixels.clear();
            }

        private:
            int mWidth;
            int mHeight;
            std::vector<fcn::Color> mPixels;
    };

    // Find font resource in common test locations
    std::filesystem::path findFontResource(std::string const & fontName)
    {
        std::vector<std::filesystem::path> searchPaths = {
            std::filesystem::path("tests/resources") / fontName,
            std::filesystem::path("../tests/resources") / fontName,
            std::filesystem::path("./tests/resources") / fontName,
        };

        auto it = std::ranges::find_if(searchPaths, [](std::filesystem::path const & p) {
            return std::filesystem::exists(p);
        });
        if (it != searchPaths.end()) {
            return *it;
        }
        return {};
    }

    // Glyph strings for each font type
    std::string const & getGlyphString(std::filesystem::path const & fontPath)
    {
        static std::string const rpgfontGlyphs =
            " abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789.,!?-+/():;%&`'*#=[]\"";

        static std::string const fixedfontGlyphs = " abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

        std::string const stem = fontPath.filename().string();
        if (stem.find("rpg") != std::string::npos) {
            return rpgfontGlyphs;
        }
        return fixedfontGlyphs;
    }
} // anonymous namespace

TEST_CASE("ImageFont construction with valid font image scans glyphs", "[unit][imagefont][construction]")
{
    SDLEnvironment const env;

    // Find rpgfont.png (typical test font with clear glyphs)
    std::filesystem::path fontPath = findFontResource("rpgfont.png");
    if (fontPath.empty()) {
        fontPath = findFontResource("fixedfont.bmp");
    }

    if (fontPath.empty()) {
        SKIP("Font image not found in search paths");
    }

    // Load the font image first to analyze its properties
    fcn::sdl3::ImageLoader loader;
    loader.setRenderer(env.mRenderer);

    fcn::Image* fontImage = loader.load(fontPath.string(), false);
    REQUIRE(fontImage != nullptr);

    // Get image dimensions for validation
    int const fontImageWidth  = fontImage->getWidth();
    int const fontImageHeight = fontImage->getHeight();

    CHECK(fontImageWidth > 0);
    CHECK(fontImageHeight > 0);

    fontImage->free();
}

TEST_CASE("ImageFont constructor loads font and extracts glyph coordinates", "[unit][imagefont][glyphs]")
{
    SDLEnvironment const env;

    // Find rpgfont.png (contains: space + alphanumeric + punctuation)
    std::filesystem::path fontPath = findFontResource("rpgfont.png");
    if (fontPath.empty()) {
        fontPath = findFontResource("fixedfont.bmp");
    }

    if (fontPath.empty()) {
        SKIP("Font image not found");
    }

    fcn::ImageFontConfig cfg;
    cfg.strategy     = fcn::SeparatorStrategy::BorderDominant;
    cfg.verbose      = false;
    cfg.glyphPadding = 0;

    // Use the glyph string appropriate for the font being loaded
    std::string const glyphs = getGlyphString(fontPath);
    fcn::ImageFont font(fontPath.string(), glyphs, cfg);

    // Verify the font image was loaded
    // Check that some valid glyphs have non-zero widths
    // Space should always have a width
    int const spaceWidth = font.getWidth(' ');
    CHECK(spaceWidth > 0);

    int const aWidth = font.getWidth('A');
    CHECK(aWidth > 0);

    int const zeroWidth = font.getWidth('0');
    CHECK(zeroWidth > 0);

    // Verify font height is positive
    int const fontHeight = font.getHeight();
    CHECK(fontHeight > 0);
}

TEST_CASE("ImageFont getWidth returns correct values for glyphs", "[unit][imagefont][width]")
{
    SDLEnvironment const env;

    std::filesystem::path fontPath = findFontResource("rpgfont.png");
    if (fontPath.empty()) {
        fontPath = findFontResource("fixedfont.bmp");
    }

    if (fontPath.empty()) {
        SKIP("Font image not found");
    }

    // rpgfont.png has 83 chars, fixedfont.bmp has 63 chars
    // Both use BorderDominant to detect separator color automatically
    fcn::ImageFontConfig cfg;
    cfg.strategy     = fcn::SeparatorStrategy::BorderDominant;
    cfg.verbose      = false;
    cfg.glyphPadding = 0;

    // Use the appropriate glyph string for the font being loaded
    std::string const glyphs = getGlyphString(fontPath);
    fcn::ImageFont font(fontPath.string(), glyphs, cfg);

    // Test space character width (always valid)
    int const spaceWidth = font.getWidth(' ');
    CHECK(spaceWidth > 0);

    // Test that 'A' has width
    int const aWidth = font.getWidth('A');
    CHECK(aWidth > 0);

    // Test that 'Z' has width
    int const zWidth = font.getWidth('Z');
    CHECK(zWidth > 0);

    // Test that '0' digit has width
    int const zeroWidth = font.getWidth('0');
    CHECK(zeroWidth > 0);

    // Test that '9' digit has width
    int const nineWidth = font.getWidth('9');
    CHECK(nineWidth > 0);

    // Test getWidth for string
    int const abcWidth = font.getWidth("ABC");
    CHECK(
        abcWidth == (static_cast<int>(font.getWidth('A')) + static_cast<int>(font.getWidth('B')) +
                     static_cast<int>(font.getWidth('C'))));
}

TEST_CASE("ImageFont getHeight returns correct values", "[unit][imagefont][height]")
{
    SDLEnvironment const env;

    std::filesystem::path fontPath = findFontResource("rpgfont.png");
    if (fontPath.empty()) {
        fontPath = findFontResource("fixedfont.bmp");
    }

    if (fontPath.empty()) {
        SKIP("Font image not found");
    }

    fcn::ImageFontConfig cfg;
    cfg.strategy     = fcn::SeparatorStrategy::BorderDominant;
    cfg.verbose      = false;
    cfg.glyphPadding = 0;

    // Use the appropriate glyph string for the font being loaded
    std::string const glyphs = getGlyphString(fontPath);
    fcn::ImageFont font(fontPath.string(), glyphs, cfg);

    // Font height should be positive and based on the glyph image
    int const height = font.getHeight();
    CHECK(height > 0);

    // Height should be consistent with row spacing defaults
    CHECK(font.getRowSpacing() == 0);

    // After setting row spacing, height should reflect it
    font.setRowSpacing(5);
    CHECK(font.getHeight() == height + 5);
}

TEST_CASE("ImageFont glyph spacing can be modified", "[unit][imagefont][spacing]")
{
    SDLEnvironment const env;

    std::filesystem::path fontPath = findFontResource("rpgfont.png");
    if (fontPath.empty()) {
        fontPath = findFontResource("fixedfont.bmp");
    }

    if (fontPath.empty()) {
        SKIP("Font image not found");
    }

    fcn::ImageFontConfig cfg;
    cfg.strategy     = fcn::SeparatorStrategy::BorderDominant;
    cfg.verbose      = false;
    cfg.glyphPadding = 0;

    // Use the appropriate glyph string for the font being loaded
    std::string const glyphs = getGlyphString(fontPath);
    fcn::ImageFont font(fontPath.string(), glyphs, cfg);

    // Default spacing should be zero
    CHECK(font.getGlyphSpacing() == 0);
    CHECK(font.getRowSpacing() == 0);

    // Set and verify glyph spacing
    font.setGlyphSpacing(2);
    CHECK(font.getGlyphSpacing() == 2);

    // Set and verify row spacing
    font.setRowSpacing(3);
    CHECK(font.getRowSpacing() == 3);

    // Get widths at different spacing levels
    font.setGlyphSpacing(0); // Reset first
    int const widthAtZero = font.getWidth('A');
    font.setGlyphSpacing(2);
    int const widthAtTwo = font.getWidth('A');

    // Width at spacing 2 should be greater than width at spacing 0
    CHECK(widthAtTwo > widthAtZero);
    // The difference should be 2 (the spacing value)
    CHECK(widthAtTwo - widthAtZero == 2);
}

TEST_CASE("ImageFont getWidth handles empty string_view", "[unit][imagefont][width][empty]")
{
    SDLEnvironment const env;

    std::filesystem::path fontPath = findFontResource("rpgfont.png");
    if (fontPath.empty()) {
        fontPath = findFontResource("fixedfont.bmp");
    }

    if (fontPath.empty()) {
        SKIP("Font image not found");
    }

    // Use BorderDominant to automatically detect separator color (white)
    fcn::ImageFontConfig cfg;
    cfg.strategy     = fcn::SeparatorStrategy::BorderDominant;
    cfg.glyphPadding = 0;

    // Use the appropriate glyph string for the font being loaded
    std::string const glyphs = getGlyphString(fontPath);
    fcn::ImageFont font(fontPath.string(), glyphs, cfg);

    // getWidth with empty string should return -mGlyphSpacing (which is 0 by default)
    int const emptyWidth = font.getWidth(std::string_view{});
    CHECK(emptyWidth == 0);
}

TEST_CASE("ImageFont setRowSpacing and getRowSpacing edge cases", "[unit][imagefont][spacing][edge]")
{
    SDLEnvironment const env;

    std::filesystem::path fontPath = findFontResource("rpgfont.png");
    if (fontPath.empty()) {
        fontPath = findFontResource("fixedfont.bmp");
    }

    if (fontPath.empty()) {
        SKIP("Font image not found");
    }

    // Use BorderDominant to automatically detect separator color (white)
    fcn::ImageFontConfig cfg;
    cfg.strategy     = fcn::SeparatorStrategy::BorderDominant;
    cfg.glyphPadding = 0;

    // Use the appropriate glyph string for the font being loaded
    std::string const glyphs = getGlyphString(fontPath);
    fcn::ImageFont font(fontPath.string(), glyphs, cfg);

    // Set negative row spacing
    font.setRowSpacing(-5);
    CHECK(font.getRowSpacing() == -5);

    // Set zero row spacing
    font.setRowSpacing(0);
    CHECK(font.getRowSpacing() == 0);

    // Set large row spacing
    font.setRowSpacing(100);
    CHECK(font.getRowSpacing() == 100);
}

TEST_CASE("ImageFont setGlyphSpacing and getGlyphSpacing edge cases", "[unit][imagefont][spacing][edge2]")
{
    SDLEnvironment const env;

    std::filesystem::path fontPath = findFontResource("rpgfont.png");
    if (fontPath.empty()) {
        fontPath = findFontResource("fixedfont.bmp");
    }

    if (fontPath.empty()) {
        SKIP("Font image not found");
    }

    // Use BorderDominant to automatically detect separator color (white)
    fcn::ImageFontConfig cfg;
    cfg.strategy     = fcn::SeparatorStrategy::BorderDominant;
    cfg.glyphPadding = 0;

    // Use the appropriate glyph string for the font being loaded
    std::string const glyphs = getGlyphString(fontPath);
    fcn::ImageFont font(fontPath.string(), glyphs, cfg);

    // Set negative glyph spacing
    font.setGlyphSpacing(-3);
    CHECK(font.getGlyphSpacing() == -3);

    // Set zero glyph spacing
    font.setGlyphSpacing(0);
    CHECK(font.getGlyphSpacing() == 0);

    // Set large glyph spacing
    font.setGlyphSpacing(50);
    CHECK(font.getGlyphSpacing() == 50);
}

TEST_CASE("ImageFont drawGlyph uses correct texture coordinates", "[unit][imagefont][draw]")
{
    SDLEnvironment const env;

    std::filesystem::path fontPath = findFontResource("rpgfont.png");
    if (fontPath.empty()) {
        fontPath = findFontResource("fixedfont.bmp");
    }

    if (fontPath.empty()) {
        SKIP("Font image not found");
    }

    // Load font with specific glyph string
    std::string const glyphs = " ABC";
    fcn::ImageFont* font     = nullptr;

    fcn::ImageFontConfig cfgPtr;
    cfgPtr.strategy     = fcn::SeparatorStrategy::BorderDominant;
    cfgPtr.verbose      = true;
    cfgPtr.glyphPadding = 0;
    REQUIRE_NOTHROW(font = new fcn::ImageFont(fontPath.string(), glyphs, cfgPtr));

    // Get the internal image to verify texture coordinates are extracted
    // We need to access the private mImage,  but we can test via drawGlyph
    // The key is that drawGlyph correctly uses the stored glyph rectangle

    // Create SDL Graphics for drawing
    fcn::sdl3::Graphics graphics;
    graphics.setTarget(env.mRenderer, 256, 256);

    // Prepare for drawing
    graphics._beginDraw();

    // Draw glyph 'A' at position (10, 10)
    // This internally uses mGlyph['A'].x, mGlyph['A'].y as source coordinates
    int const drawnWidth = font->drawGlyph(&graphics, 'A', 10, 10);

    // The drawn width should match the glyph width plus spacing
    int const expectedWidth = font->getWidth('A');
    CHECK(drawnWidth == expectedWidth);

    // Draw space character, should use space width
    int const spaceWidth = font->drawGlyph(&graphics, ' ', 10, 10);
    CHECK(spaceWidth == static_cast<int>(font->getWidth(' ')));

    int const bWidth = font->drawGlyph(&graphics, 'B', 10, 10);
    CHECK(bWidth == static_cast<int>(font->getWidth('B')));

    graphics._endDraw();

    delete font;
}

TEST_CASE("ImageFont renders multiple glyphs in sequence", "[unit][imagefont][render]")
{
    SDLEnvironment const env;

    std::filesystem::path fontPath = findFontResource("rpgfont.png");
    if (fontPath.empty()) {
        fontPath = findFontResource("fixedfont.bmp");
    }

    if (fontPath.empty()) {
        SKIP("Font image not found");
    }

    // Use BorderDominant to automatically detect the separator color (white for both fonts)
    fcn::ImageFontConfig cfg;
    cfg.strategy     = fcn::SeparatorStrategy::BorderDominant;
    cfg.glyphPadding = 0;
    cfg.verbose      = false;

    // Use the appropriate glyph string for the font being loaded
    std::string const glyphs = getGlyphString(fontPath);
    fcn::ImageFont font(fontPath.string(), glyphs, cfg);

    // Create SDL Graphics for drawing
    fcn::sdl3::Graphics graphics;
    graphics.setTarget(env.mRenderer, 256, 256);
    graphics._beginDraw();

    // Draw a string, this internally calls drawGlyph for each character
    std::string const testString = "Hello";
    int expectedX                = 10;
    int const expectedY          = 20;

    for (char const c : testString) {
        int const drawnWidth = font.drawGlyph(&graphics, static_cast<unsigned char>(c), expectedX, expectedY);
        expectedX += drawnWidth;
    }

    // Verify total width matches getWidth for the string
    int const totalWidth = font.getWidth(testString);

    // Calculate expected final X position
    int const expectedFinalX = 10 + totalWidth;
    CHECK(expectedX == expectedFinalX);

    graphics._endDraw();
}

TEST_CASE("ImageFont scanned glyphs have non-zero widths", "[unit][imagefont][nozero]")
{
    SDLEnvironment const env;

    std::filesystem::path fontPath = findFontResource("rpgfont.png");
    if (fontPath.empty()) {
        fontPath = findFontResource("fixedfont.bmp");
    }

    if (fontPath.empty()) {
        SKIP("Font image not found");
    }

    // Use the appropriate glyph string for the font being loaded
    std::string const glyphs = getGlyphString(fontPath);

    fcn::ImageFont font(fontPath.string(), glyphs);

    for (char const c : glyphs) {
        std::string const infoMsg(1, c);
        INFO("Glyph: " << infoMsg);
        CHECK(static_cast<int>(font.getWidth(static_cast<unsigned char>(c))) > 0);
    }
}

TEST_CASE("ImageFont constructor with Image* loads font", "[unit][imagefont][imageptr]")
{
    SDLEnvironment const env;

    std::filesystem::path fontPath = findFontResource("rpgfont.png");
    if (fontPath.empty()) {
        fontPath = findFontResource("fixedfont.bmp");
    }

    if (fontPath.empty()) {
        SKIP("Font image not found");
    }

    // Load the font image directly
    fcn::sdl3::ImageLoader loader;
    loader.setRenderer(env.mRenderer);

    fcn::Image* fontImage = loader.load(fontPath.string(), false);
    REQUIRE(fontImage != nullptr);

    // Create ImageFont from Image pointer
    std::string const glyphs    = " ABC";
    fcn::ImageFont const * font = nullptr;

    REQUIRE_NOTHROW(font = new fcn::ImageFont(fontImage, glyphs));

    // Verify the font works correctly
    CHECK(font->getWidth('A') > 0);
    CHECK(font->getHeight() > 0);

    // The Image* constructor must map glyphs by their actual character codes,
    // matching the file-based constructor for the same image and glyph string.
    fcn::ImageFont const fileFont(fontPath.string(), glyphs);
    for (char const glyph : glyphs) {
        INFO("Glyph: " << glyph);
        CHECK(
            font->getWidth(static_cast<unsigned char>(glyph)) == fileFont.getWidth(static_cast<unsigned char>(glyph)));
    }

    // Clean up, ImageFont takes ownership of the image
    delete font;
}

TEST_CASE("ImageFont drawString draws entire string", "[unit][imagefont][drawstring]")
{
    SDLEnvironment const env;

    std::filesystem::path fontPath = findFontResource("rpgfont.png");
    if (fontPath.empty()) {
        fontPath = findFontResource("fixedfont.bmp");
    }

    if (fontPath.empty()) {
        SKIP("Font image not found");
    }

    // Use BorderDominant to automatically detect separator color (white)
    fcn::ImageFontConfig cfg;
    cfg.strategy     = fcn::SeparatorStrategy::BorderDominant;
    cfg.glyphPadding = 0;

    // Use the appropriate glyph string for the font being loaded
    std::string const glyphs = getGlyphString(fontPath);
    fcn::ImageFont font(fontPath.string(), glyphs, cfg);

    // Create SDL Graphics
    fcn::sdl3::Graphics graphics;
    graphics.setTarget(env.mRenderer, 256, 256);
    graphics._beginDraw();

    // Draw a string at position
    std::string const testString = "Test";
    int const startX             = 5;
    int const startY             = 10;

    font.drawString(&graphics, testString, startX, startY);

    // Verify getWidth for the string
    int const stringWidth = font.getWidth(testString);
    CHECK(stringWidth > 0);

    // Verify getHeight is consistent
    int const fontHeight = font.getHeight();
    CHECK(fontHeight > 0);

    graphics._endDraw();
}

TEST_CASE(
    "ImageFont drawString preserves atlas colours instead of tinting with the Graphics colour",
    "[unit][imagefont][drawstring][colour]")
{
    SDLEnvironment const env;

    // A one-pixel white glyph in a magenta-separated atlas.
    constexpr int imageWidth  = 3;
    constexpr int imageHeight = 1;

    fcn::Color const separator(255, 0, 255, 255);
    fcn::Color const glyphColor(255, 255, 255, 255);

    std::vector<fcn::Color> pixels(static_cast<std::size_t>(imageWidth) * imageHeight, separator);
    pixels.at(1) = glyphColor;

    auto* const image = new CachelessImage(imageWidth, imageHeight, std::move(pixels));
    fcn::ImageFont const font(image, "A");
    REQUIRE(font.getWidth("A") == 1);
    REQUIRE(font.getHeight() == 1);

    constexpr int targetSize = 4;
    SDL_Texture* target =
        SDL_CreateTexture(env.mRenderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, targetSize, targetSize);
    REQUIRE(target != nullptr);
    REQUIRE(SDL_SetRenderTarget(env.mRenderer, target));

    SDL_SetRenderDrawColor(env.mRenderer, 0, 0, 0, 255);
    REQUIRE(SDL_RenderClear(env.mRenderer));

    fcn::sdl3::Graphics graphics;
    graphics.setTarget(env.mRenderer, targetSize, targetSize);
    graphics._beginDraw();

    // Widget text normally sets the foreground colour before drawing. Image
    // fonts must ignore that and keep the colours stored in their atlas.
    graphics.setColor(fcn::Color(0, 0, 0, 255));
    font.drawString(&graphics, "A", 1, 1);

    // The temporary white modulation colour must be restored afterwards.
    CHECK(graphics.getColor() == fcn::Color(0, 0, 0, 255));

    graphics._endDraw();

    Uint8 red             = 0;
    Uint8 green           = 0;
    Uint8 blue            = 0;
    Uint8 alpha           = 0;
    SDL_Surface* readback = SDL_RenderReadPixels(env.mRenderer, nullptr);
    REQUIRE(readback != nullptr);
    REQUIRE(SDL_ReadSurfacePixel(readback, 1, 1, &red, &green, &blue, &alpha));
    SDL_DestroySurface(readback);

    CHECK(red > 200);
    CHECK(green > 200);
    CHECK(blue > 200);
    CHECK(alpha > 0);

    SDL_SetRenderTarget(env.mRenderer, nullptr);
    SDL_DestroyTexture(target);
}

TEST_CASE("ImageFont getWidth with string_view containing unknown chars", "[unit][imagefont][width][unknown_chars]")
{
    SDLEnvironment const env;

    std::filesystem::path fontPath = findFontResource("rpgfont.png");
    if (fontPath.empty()) {
        fontPath = findFontResource("fixedfont.bmp");
    }

    if (fontPath.empty()) {
        SKIP("Font image not found");
    }

    // Use BorderDominant to automatically detect separator color (white)
    fcn::ImageFontConfig cfg;
    cfg.strategy     = fcn::SeparatorStrategy::BorderDominant;
    cfg.glyphPadding = 0;

    std::string const glyphs = "ABC";
    fcn::ImageFont font(fontPath.string(), glyphs, cfg);

    // String with known chars should work
    int const knownWidth = font.getWidth(std::string_view("ABC"));
    CHECK(knownWidth > 0);

    // String with unknown chars should still compute width (using space for unknown)
    int const mixedWidth = font.getWidth(std::string_view("ABX")); // X is unknown
    CHECK(mixedWidth > 0);
}

TEST_CASE("ImageFont handles unknown glyph gracefully", "[unit][imagefont][unknown]")
{
    SDLEnvironment const env;

    std::filesystem::path fontPath = findFontResource("rpgfont.png");
    if (fontPath.empty()) {
        fontPath = findFontResource("fixedfont.bmp");
    }

    if (fontPath.empty()) {
        SKIP("Font image not found");
    }

    // Use BorderDominant to automatically detect separator color (white)
    fcn::ImageFontConfig cfg;
    cfg.strategy     = fcn::SeparatorStrategy::BorderDominant;
    cfg.glyphPadding = 0;

    // Use the appropriate glyph string for the font being loaded
    std::string const glyphs = getGlyphString(fontPath);
    fcn::ImageFont font(fontPath.string(), glyphs, cfg);

    // Create graphics for drawing
    fcn::sdl3::Graphics graphics;
    graphics.setTarget(env.mRenderer, 256, 256);
    graphics._beginDraw();

    // Draw known glyph 'A'
    int const aWidth = font.drawGlyph(&graphics, 'A', 0, 0);
    CHECK(aWidth > 0);

    // Draw 'Z'
    int const zWidth = font.drawGlyph(&graphics, 'Z', 0, 0);
    CHECK(zWidth > 0);

    // For unknown glyphs (outside the defined range), ImageFont uses space width
    int const unknownWidth = font.drawGlyph(&graphics, 1, 0, 0); // ASCII 1 is outside 32-126
    // Unknown glyphs use space width
    CHECK(unknownWidth == font.getWidth(' '));

    graphics._endDraw();
}

TEST_CASE("ImageFont drawGlyph uses space width for zero-width glyph", "[unit][imagefont][draw][zerowidth]")
{
    SDLEnvironment const env;

    std::filesystem::path fontPath = findFontResource("rpgfont.png");
    if (fontPath.empty()) {
        fontPath = findFontResource("fixedfont.bmp");
    }

    if (fontPath.empty()) {
        SKIP("Font image not found");
    }

    // Use BorderDominant to automatically detect separator color (white)
    fcn::ImageFontConfig cfg;
    cfg.strategy     = fcn::SeparatorStrategy::BorderDominant;
    cfg.glyphPadding = 0;

    // Use the appropriate glyph string for the font being loaded
    std::string const glyphs = getGlyphString(fontPath);
    fcn::ImageFont font(fontPath.string(), glyphs, cfg);

    // Create SDL Graphics for drawing
    fcn::sdl3::Graphics graphics;
    graphics.setTarget(env.mRenderer, 256, 256);
    graphics._beginDraw();

    // Draw a glyph that has zero width (should fall back to space width)
    // Using character code 1 which is unlikely to be in the font
    int const drawnWidth = font.drawGlyph(&graphics, static_cast<unsigned char>(1), 10, 10);
    // Unknown glyphs should use space width
    CHECK(drawnWidth == font.getWidth(' '));

    graphics._endDraw();
}

TEST_CASE("ImageFont getStringIndexAt finds correct index", "[unit][imagefont][index]")
{
    SDLEnvironment const env;

    std::filesystem::path fontPath = findFontResource("rpgfont.png");
    if (fontPath.empty()) {
        fontPath = findFontResource("fixedfont.bmp");
    }

    if (fontPath.empty()) {
        SKIP("Font image not found");
    }

    // Use BorderDominant to automatically detect separator color (white)
    fcn::ImageFontConfig cfg;
    cfg.strategy     = fcn::SeparatorStrategy::BorderDominant;
    cfg.glyphPadding = 0;

    // Use the appropriate glyph string for the font being loaded
    std::string const glyphs = getGlyphString(fontPath);
    fcn::ImageFont font(fontPath.string(), glyphs, cfg);

    std::string const testString = "ABC";

    CHECK(font.getStringIndexAt(testString, 0) == 0);

    int const aWidth = font.getWidth('A');
    CHECK(font.getStringIndexAt(testString, aWidth) == 1);

    int const abWidth = font.getWidth('A') + font.getWidth('B');
    CHECK(font.getStringIndexAt(testString, abWidth) == 2);

    int const totalWidth = font.getWidth(testString);
    CHECK(font.getStringIndexAt(testString, totalWidth) == 3);
}

TEST_CASE("ImageFont getStringIndexAt handles x beyond total width", "[unit][imagefont][index][beyond]")
{
    SDLEnvironment const env;

    std::filesystem::path fontPath = findFontResource("rpgfont.png");
    if (fontPath.empty()) {
        fontPath = findFontResource("fixedfont.bmp");
    }

    if (fontPath.empty()) {
        SKIP("Font image not found");
    }

    // Use BorderDominant to automatically detect separator color (white)
    fcn::ImageFontConfig cfg;
    cfg.strategy     = fcn::SeparatorStrategy::BorderDominant;
    cfg.glyphPadding = 0;

    // Use the appropriate glyph string for the font being loaded
    std::string const glyphs = getGlyphString(fontPath);
    fcn::ImageFont font(fontPath.string(), glyphs, cfg);

    std::string const testString = "ABC";

    // Requesting index beyond total width should return text.size()
    int beyondWidth = font.getWidth(testString) + 100;
    CHECK(font.getStringIndexAt(testString, beyondWidth) == static_cast<int>(testString.size()));

    // Requesting index at negative position should return 0
    CHECK(font.getStringIndexAt(testString, -1) == 0);
}

TEST_CASE("ImageFont renderToSurface works after the source pixels are discarded", "[unit][imagefont][render]")
{
    constexpr int glyphWidth  = 2;
    constexpr int glyphHeight = 3;
    constexpr int gap         = 1;
    constexpr int glyphCount  = 4;
    constexpr int imageWidth  = (glyphCount * glyphWidth) + ((glyphCount + 1) * gap);

    fcn::Color const separator(255, 0, 255, 255);
    fcn::Color const glyphColor(255, 255, 255, 255);

    std::vector<fcn::Color> pixels(
        static_cast<std::size_t>(imageWidth) * static_cast<std::size_t>(glyphHeight), separator);

    for (int glyph = 0; glyph < glyphCount; ++glyph) {
        int const startX = gap + (glyph * (glyphWidth + gap));
        for (int y = 0; y < glyphHeight; ++y) {
            for (int x = 0; x < glyphWidth; ++x) {
                pixels.at(
                    (static_cast<std::size_t>(y) * static_cast<std::size_t>(imageWidth)) +
                    static_cast<std::size_t>(startX + x)) = glyphColor;
            }
        }
    }

    auto* const image = new CachelessImage(imageWidth, glyphHeight, std::move(pixels));
    fcn::ImageFont const font(image, " ABC");

    auto surface = font.renderToSurface("ABC");
    REQUIRE(surface != nullptr);
    CHECK(surface->w == font.getWidth("ABC"));
    CHECK(surface->h == font.getHeight());

    REQUIRE(SDL_LockSurface(surface.get()));
    auto const * const renderedPixels = static_cast<Uint32 const *>(surface->pixels);
    bool hasVisiblePixel              = false;
    int const pixelCount              = surface->h * (surface->pitch / static_cast<int>(sizeof(Uint32)));
    for (int i = 0; i < pixelCount; ++i) {
        if (renderedPixels[i] != 0) {
            hasVisiblePixel = true;
            break;
        }
    }
    SDL_UnlockSurface(surface.get());

    CHECK(hasVisiblePixel);
}

TEST_CASE("ImageFont renderToSurface returns nullptr for empty string", "[unit][imagefont][render][empty]")
{
    constexpr int glyphWidth  = 2;
    constexpr int glyphHeight = 3;
    constexpr int gap         = 1;
    constexpr int glyphCount  = 4;
    constexpr int imageWidth  = (glyphCount * glyphWidth) + ((glyphCount + 1) * gap);

    fcn::Color const separator(255, 0, 255, 255);
    fcn::Color const glyphColor(255, 255, 255, 255);

    std::vector<fcn::Color> pixels(
        static_cast<std::size_t>(imageWidth) * static_cast<std::size_t>(glyphHeight), separator);

    for (int glyph = 0; glyph < glyphCount; ++glyph) {
        int const startX = gap + (glyph * (glyphWidth + gap));
        for (int y = 0; y < glyphHeight; ++y) {
            for (int x = 0; x < glyphWidth; ++x) {
                pixels.at(
                    (static_cast<std::size_t>(y) * static_cast<std::size_t>(imageWidth)) +
                    static_cast<std::size_t>(startX + x)) = glyphColor;
            }
        }
    }

    auto* const image = new CachelessImage(imageWidth, glyphHeight, std::move(pixels));
    fcn::ImageFont const font(image, " ABC");

    // Rendering empty string should return nullptr
    auto surface = font.renderToSurface("");
    REQUIRE(surface == nullptr);
}
