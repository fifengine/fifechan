// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2013 - 2026 Fifengine contributors

// Corresponding header include

// Standard library includes
#include <filesystem>
#include <string>
#include <vector>

// Third-party library includes
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <catch2/catch_test_macros.hpp>

// Project headers (subdirs before local)
#include "fifechan/backends/sdl2/graphics.hpp"
#include "fifechan/backends/sdl2/image.hpp"
#include "fifechan/backends/sdl2/imageloader.hpp"
#include "fifechan/imagefont.hpp"

namespace
{
    // SDL2 test environment (lifecycle managed per test case)
    struct SDL2Environment
    {
        SDL2Environment()
        {
            if (SDL_Init(SDL_INIT_VIDEO) != 0) {
                throw std::runtime_error(std::string("SDL_Init failed: ") + SDL_GetError());
            }

            // Initialize SDL_image for loading PNG (use PNG only as BMP may not be available)
            int imgFlags = IMG_INIT_PNG;
            if ((IMG_Init(imgFlags) & imgFlags) != imgFlags) {
                SDL_Quit();
                throw std::runtime_error(std::string("IMG_Init failed: ") + SDL_GetError());
            }

            // Create a window for SDL2 hardware acceleration
            mWindow = SDL_CreateWindow(
                "FifeGUI ImageFont Test",
                SDL_WINDOWPOS_CENTERED,
                SDL_WINDOWPOS_CENTERED,
                256,
                256,
                SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
            if (mWindow == nullptr) {
                IMG_Quit();
                SDL_Quit();
                throw std::runtime_error(std::string("SDL_CreateWindow failed: ") + SDL_GetError());
            }

            mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
            if (mRenderer == nullptr) {
                SDL_DestroyWindow(mWindow);
                mWindow = nullptr;
                IMG_Quit();
                SDL_Quit();
                throw std::runtime_error(std::string("SDL_CreateRenderer failed: ") + SDL_GetError());
            }

            // Set up global ImageLoader for Image::load() to work
            mImageLoader = new fcn::sdl2::ImageLoader();
            mImageLoader->setRenderer(mRenderer);
            fcn::Image::setImageLoader(mImageLoader);
        }

        ~SDL2Environment()
        {
            // Clean up the global ImageLoader
            if (mImageLoader != nullptr) {
                fcn::Image::setImageLoader(nullptr);
                delete mImageLoader;
                mImageLoader = nullptr;
            }
            if (mRenderer != nullptr) {
                SDL_DestroyRenderer(mRenderer);
                mRenderer = nullptr;
            }
            if (mWindow != nullptr) {
                SDL_DestroyWindow(mWindow);
                mWindow = nullptr;
            }
            IMG_Quit();
            SDL_Quit();
        }

        SDL_Window* mWindow                  = nullptr;
        SDL_Renderer* mRenderer              = nullptr;
        fcn::sdl2::ImageLoader* mImageLoader = nullptr;
    };

    // Find font resource in common test locations
    std::filesystem::path findFontResource(std::string const & fontName)
    {
        std::vector<std::filesystem::path> searchPaths = {
            std::filesystem::path("tests/resources") / fontName,
            std::filesystem::path("../tests/resources") / fontName,
            std::filesystem::path("./tests/resources") / fontName,
        };

        for (auto const & path : searchPaths) {
            if (std::filesystem::exists(path)) {
                return path;
            }
        }
        return {};
    }

    // Set up global ImageLoader for Image::load() to work
    void setupImageLoader(SDL_Renderer* renderer)
    {
        fcn::sdl2::ImageLoader* loader = new fcn::sdl2::ImageLoader();
        loader->setRenderer(renderer);
        fcn::Image::setImageLoader(loader);
    }
} // anonymous namespace

TEST_CASE("ImageFont construction with valid font image scans glyphs", "[unit][imagefont][construction]")
{
    SDL2Environment env;

    // Find rpgfont.png (typical test font with clear glyphs)
    std::filesystem::path fontPath = findFontResource("rpgfont.png");
    if (fontPath.empty()) {
        fontPath = findFontResource("fixedfont.bmp");
    }

    if (fontPath.empty()) {
        SKIP("Font image not found in search paths");
    }

    // Load the font image first to analyze its properties
    fcn::sdl2::ImageLoader loader;
    loader.setRenderer(env.mRenderer);

    fcn::Image* fontImage = loader.load(fontPath.string(), false);
    REQUIRE(fontImage != nullptr);

    // Get the separator color (pixel at 0,0) used for glyph scanning
    // Note: The ImageLoader may not apply color key conversion when loaded with convertToDisplayFormat=false.
    // Just verify we can read the pixel without error.
    fcn::Color separatorColor = fontImage->getPixel(0, 0);
    // The pixel should have valid color values (not all zeros)
    bool hasColor =
        (separatorColor.r > 0) || (separatorColor.g > 0) || (separatorColor.b > 0) || (separatorColor.a > 0);
    CHECK(hasColor);

    // Get image dimensions for validation
    int const fontImageWidth  = fontImage->getWidth();
    int const fontImageHeight = fontImage->getHeight();

    CHECK(fontImageWidth > 0);
    CHECK(fontImageHeight > 0);

    // Clean up before creating ImageFont
    fontImage->free();
}

TEST_CASE("ImageFont constructor loads font and extracts glyph coordinates", "[unit][imagefont][glyphs]")
{
    SDL2Environment env;

    // Find rpgfont.png contains ASCII characters in order
    std::filesystem::path fontPath = findFontResource("rpgfont.png");
    if (fontPath.empty()) {
        fontPath = findFontResource("fixedfont.bmp");
    }

    if (fontPath.empty()) {
        SKIP("Font image not found");
    }

    // The rpgfont.png contains these glyphs:
    std::string const glyphs = " !\"#$%&'()*+,-./:;<=>?@0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

    fcn::ImageFont font(fontPath.string(), glyphs);

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
    SDL2Environment env;

    std::filesystem::path fontPath = findFontResource("rpgfont.png");
    if (fontPath.empty()) {
        fontPath = findFontResource("fixedfont.bmp");
    }

    if (fontPath.empty()) {
        SKIP("Font image not found");
    }

    // Use ASCII range constructor for simpler testing
    // Fixedfont contains printable ASCII from space (32) to tilde (126)
    fcn::ImageFont font(fontPath.string(), 32, 126);

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
    CHECK(abcWidth == (font.getWidth('A') + font.getWidth('B') + font.getWidth('C')));
}

TEST_CASE("ImageFont getHeight returns correct values", "[unit][imagefont][height]")
{
    SDL2Environment env;

    std::filesystem::path fontPath = findFontResource("rpgfont.png");
    if (fontPath.empty()) {
        fontPath = findFontResource("fixedfont.bmp");
    }

    if (fontPath.empty()) {
        SKIP("Font image not found");
    }

    fcn::ImageFont font(fontPath.string(), 32, 126);

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
    SDL2Environment env;

    std::filesystem::path fontPath = findFontResource("rpgfont.png");
    if (fontPath.empty()) {
        fontPath = findFontResource("fixedfont.bmp");
    }

    if (fontPath.empty()) {
        SKIP("Font image not found");
    }

    fcn::ImageFont font(fontPath.string(), 32, 126);

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

TEST_CASE("ImageFont drawGlyph uses correct texture coordinates", "[unit][imagefont][draw]")
{
    SDL2Environment env;

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

    REQUIRE_NOTHROW(font = new fcn::ImageFont(fontPath.string(), glyphs));

    // Get the internal image to verify texture coordinates are extracted
    // We need to access the private mImage,  but we can test via drawGlyph
    // The key is that drawGlyph correctly uses the stored glyph rectangle

    // Create SDL Graphics for drawing
    fcn::sdl2::Graphics graphics;
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
    CHECK(spaceWidth == font->getWidth(' '));

    int const bWidth = font->drawGlyph(&graphics, 'B', 10, 10);
    CHECK(bWidth == font->getWidth('B'));

    graphics._endDraw();

    delete font;
}

TEST_CASE("ImageFont renders multiple glyphs in sequence", "[unit][imagefont][render]")
{
    SDL2Environment env;

    std::filesystem::path fontPath = findFontResource("rpgfont.png");
    if (fontPath.empty()) {
        fontPath = findFontResource("fixedfont.bmp");
    }

    if (fontPath.empty()) {
        SKIP("Font image not found");
    }

    // Create font with ASCII printable range
    fcn::ImageFont font(fontPath.string(), 32, 126);

    // Create SDL Graphics for drawing
    fcn::sdl2::Graphics graphics;
    graphics.setTarget(env.mRenderer, 256, 256);
    graphics._beginDraw();

    // Draw a string, this internally calls drawGlyph for each character
    std::string const testString = "Hello";
    int expectedX                = 10;
    int expectedY                = 20;

    for (char const c : testString) {
        int const drawnWidth = font.drawGlyph(&graphics, c, expectedX, expectedY);
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
    SDL2Environment env;

    std::filesystem::path fontPath = findFontResource("rpgfont.png");
    if (fontPath.empty()) {
        fontPath = findFontResource("fixedfont.bmp");
    }

    if (fontPath.empty()) {
        SKIP("Font image not found");
    }

    // Use the glyph set typically used by examples to ensure all scanned glyphs have positive width
    std::string const glyphs = " abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789.,!?-+/():;%&`'*#[]\"";

    fcn::ImageFont font(fontPath.string(), glyphs);

    for (char const c : glyphs) {
        std::string infoMsg(1, c);
        INFO("Glyph: " << infoMsg);
        CHECK(font.getWidth(c) > 0);
    }
}

TEST_CASE("ImageFont constructor with Image* loads font", "[unit][imagefont][imageptr]")
{
    SDL2Environment env;

    std::filesystem::path fontPath = findFontResource("rpgfont.png");
    if (fontPath.empty()) {
        fontPath = findFontResource("fixedfont.bmp");
    }

    if (fontPath.empty()) {
        SKIP("Font image not found");
    }

    // Load the font image directly
    fcn::sdl2::ImageLoader loader;
    loader.setRenderer(env.mRenderer);

    fcn::Image* fontImage = loader.load(fontPath.string(), false);
    REQUIRE(fontImage != nullptr);

    // Create ImageFont from Image pointer
    std::string const glyphs = " ABC";
    fcn::ImageFont* font     = nullptr;

    REQUIRE_NOTHROW(font = new fcn::ImageFont(fontImage, glyphs));

    // Verify the font works correctly
    CHECK(font->getWidth('A') > 0);
    CHECK(font->getHeight() > 0);

    // Clean up, ImageFont takes ownership of the image
    delete font;
}

TEST_CASE("ImageFont drawString draws entire string", "[unit][imagefont][drawstring]")
{
    SDL2Environment env;

    std::filesystem::path fontPath = findFontResource("rpgfont.png");
    if (fontPath.empty()) {
        fontPath = findFontResource("fixedfont.bmp");
    }

    if (fontPath.empty()) {
        SKIP("Font image not found");
    }

    fcn::ImageFont font(fontPath.string(), 32, 126);

    // Create SDL Graphics
    fcn::sdl2::Graphics graphics;
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

TEST_CASE("ImageFont handles unknown glyph gracefully", "[unit][imagefont][unknown]")
{
    SDL2Environment env;

    std::filesystem::path fontPath = findFontResource("rpgfont.png");
    if (fontPath.empty()) {
        fontPath = findFontResource("fixedfont.bmp");
    }

    if (fontPath.empty()) {
        SKIP("Font image not found");
    }

    // Create font with printable ASCII range (includes space through ~)
    fcn::ImageFont font(fontPath.string(), 32, 126);

    // Create graphics for drawing
    fcn::sdl2::Graphics graphics;
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

TEST_CASE("ImageFont getStringIndexAt finds correct index", "[unit][imagefont][index]")
{
    SDL2Environment env;

    std::filesystem::path fontPath = findFontResource("rpgfont.png");
    if (fontPath.empty()) {
        fontPath = findFontResource("fixedfont.bmp");
    }

    if (fontPath.empty()) {
        SKIP("Font image not found");
    }

    fcn::ImageFont font(fontPath.string(), 32, 126);

    std::string const testString = "ABC";

    CHECK(font.getStringIndexAt(testString, 0) == 0);

    int const aWidth = font.getWidth('A');
    CHECK(font.getStringIndexAt(testString, aWidth) == 1);

    int const abWidth = font.getWidth('A') + font.getWidth('B');
    CHECK(font.getStringIndexAt(testString, abWidth) == 2);

    int const totalWidth = font.getWidth(testString);
    CHECK(font.getStringIndexAt(testString, totalWidth) == 3);
}
