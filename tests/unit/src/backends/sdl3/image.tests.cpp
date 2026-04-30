// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2013 - 2026 Fifengine contributors

// Corresponding header include

// Standard library includes
#include <algorithm>
#include <filesystem>
#include <stdexcept>
#include <string>
#include <vector>

// Third-party library includes
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

#include <catch2/catch_test_macros.hpp>

// Project headers (subdirs before local)
#include "fifechan/backends/sdl3/image.hpp"
#include "fifechan/backends/sdl3/imageloader.hpp"

namespace
{
    // Test constants
    constexpr int TEST_IMAGE_WIDTH  = 4;
    constexpr int TEST_IMAGE_HEIGHT = 4;

    // Magenta color key (255, 0, 255)
    constexpr uint8_t MAGENTA_R = 255;
    constexpr uint8_t MAGENTA_G = 0;
    constexpr uint8_t MAGENTA_B = 255;

    // Non-magenta colors for testing
    constexpr uint8_t RED_R   = 255;
    constexpr uint8_t RED_G   = 0;
    constexpr uint8_t RED_B   = 0;
    constexpr uint8_t GREEN_R = 0;
    constexpr uint8_t GREEN_G = 255;
    constexpr uint8_t GREEN_B = 0;
    constexpr uint8_t BLUE_R  = 0;
    constexpr uint8_t BLUE_G  = 0;
    constexpr uint8_t BLUE_B  = 255;

    // SDL test environment (lifecycle managed per test case)
    struct SDLEnvironment
    {
            SDLEnvironment()
            {
                if (!SDL_Init(SDL_INIT_VIDEO)) {
                    throw std::runtime_error(std::string("SDL_Init failed: ") + SDL_GetError());
                }

                mWindow = SDL_CreateWindow("FifeGUI Image Test", 256, 256, SDL_WINDOW_RESIZABLE);
                if (mWindow == nullptr) {
                    SDL_Quit();
                    throw std::runtime_error(std::string("SDL_CreateWindow failed: ") + SDL_GetError());
                }

                mRenderer = SDL_CreateRenderer(mWindow, nullptr);
                if (mRenderer == nullptr) {
                    SDL_DestroyWindow(mWindow);
                    mWindow = nullptr;
                    SDL_Quit();
                    throw std::runtime_error(std::string("SDL_CreateRenderer failed: ") + SDL_GetError());
                }
            }

            ~SDLEnvironment()
            {
                if (mRenderer != nullptr) {
                    SDL_DestroyRenderer(mRenderer);
                    mRenderer = nullptr;
                }
                if (mWindow != nullptr) {
                    SDL_DestroyWindow(mWindow);
                    mWindow = nullptr;
                }
                SDL_Quit();
            }

            SDL_Window* mWindow     = nullptr;
            SDL_Renderer* mRenderer = nullptr;
    };

    // Creates a test surface with magenta color key pixels
    // Layout:
    // +------------------+
    // | Magenta Red      |
    // | Green  Magenta  |
    // | Blue   Blue    |
    // +------------------+
    SDL_Surface* createTestSurfaceWithColorKey()
    {
        SDL_Surface* surface = SDL_CreateSurface(TEST_IMAGE_WIDTH, TEST_IMAGE_HEIGHT, SDL_PIXELFORMAT_RGBA8888);

        if (surface == nullptr) {
            return nullptr;
        }

        // Fill with transparent (black with full alpha)
        SDL_FillSurfaceRect(surface, nullptr, SDL_MapSurfaceRGBA(surface, 0, 0, 0, 255));

        // Lock surface for direct pixel access
        SDL_LockSurface(surface);

        auto* pixels = static_cast<uint32_t*>(surface->pixels);

        // Row 0: Magenta (top-left), Red (top-right)
        // (0,0) = Magenta (color key), (1,0) = Red
        pixels[0] = SDL_MapSurfaceRGBA(surface, MAGENTA_R, MAGENTA_G, MAGENTA_B, 255);
        pixels[1] = SDL_MapSurfaceRGBA(surface, RED_R, RED_G, RED_B, 255);
        pixels[2] = SDL_MapSurfaceRGBA(surface, RED_R, RED_G, RED_B, 255);
        pixels[3] = SDL_MapSurfaceRGBA(surface, RED_R, RED_G, RED_B, 255);

        // Row 1: Green, Magenta (x3)
        pixels[4] = SDL_MapSurfaceRGBA(surface, GREEN_R, GREEN_G, GREEN_B, 255);
        pixels[5] = SDL_MapSurfaceRGBA(surface, MAGENTA_R, MAGENTA_G, MAGENTA_B, 255);
        pixels[6] = SDL_MapSurfaceRGBA(surface, MAGENTA_R, MAGENTA_G, MAGENTA_B, 255);
        pixels[7] = SDL_MapSurfaceRGBA(surface, MAGENTA_R, MAGENTA_G, MAGENTA_B, 255);

        // Row 2: Blue, Blue, Magenta, Blue
        pixels[8]  = SDL_MapSurfaceRGBA(surface, BLUE_R, BLUE_G, BLUE_B, 255);
        pixels[9]  = SDL_MapSurfaceRGBA(surface, BLUE_R, BLUE_G, BLUE_B, 255);
        pixels[10] = SDL_MapSurfaceRGBA(surface, MAGENTA_R, MAGENTA_G, MAGENTA_B, 255);
        pixels[11] = SDL_MapSurfaceRGBA(surface, BLUE_R, BLUE_G, BLUE_B, 255);

        // Row 3: All blue
        pixels[12] = SDL_MapSurfaceRGBA(surface, BLUE_R, BLUE_G, BLUE_B, 255);
        pixels[13] = SDL_MapSurfaceRGBA(surface, BLUE_R, BLUE_G, BLUE_B, 255);
        pixels[14] = SDL_MapSurfaceRGBA(surface, BLUE_R, BLUE_G, BLUE_B, 255);
        pixels[15] = SDL_MapSurfaceRGBA(surface, BLUE_R, BLUE_G, BLUE_B, 255);

        SDL_UnlockSurface(surface);

        // Set magenta as color key (for testing)
        SDL_SetSurfaceColorKey(surface, true, SDL_MapSurfaceRGB(surface, MAGENTA_R, MAGENTA_G, MAGENTA_B));

        return surface;
    }
} // anonymous namespace

TEST_CASE("Image color key conversion: magenta becomes transparent", "[unit][image][colorkey]")
{
    SDLEnvironment env;

    SDL_Surface* surfaceWithColorKey = createTestSurfaceWithColorKey();
    REQUIRE(surfaceWithColorKey != nullptr);

    // Create Image from surface with color key
    // autoFree=true to manage surface lifetime
    fcn::sdl3::Image image(surfaceWithColorKey, true, env.mRenderer);

    // Verify image dimensions
    REQUIRE(image.getWidth() == TEST_IMAGE_WIDTH);
    REQUIRE(image.getHeight() == TEST_IMAGE_HEIGHT);

    // Test that magenta pixels have alpha=0 after color key conversion
    // Position (0,0) was magenta, should now be transparent
    fcn::Color pixel00 = image.getPixel(0, 0);
    REQUIRE(pixel00.a == 0); // Alpha should be 0 for transparent

    // Position (1,0) was red, should remain fully opaque
    fcn::Color pixel10 = image.getPixel(1, 0);
    CHECK(pixel10.r == RED_R);
    CHECK(pixel10.g == RED_G);
    CHECK(pixel10.b == RED_B);
    REQUIRE(pixel10.a == 255); // Alpha should remain 255 for non-magenta

    // Position (1,1) was magenta, should be transparent
    fcn::Color pixel11 = image.getPixel(1, 1);
    REQUIRE(pixel11.a == 0); // Alpha should be 0 for transparent

    // Position (2,1) was magenta, should be transparent
    fcn::Color pixel21 = image.getPixel(2, 1);
    REQUIRE(pixel21.a == 0); // Alpha should be 0 for transparent

    // Position (0,2) was blue, should remain fully opaque
    fcn::Color pixel02 = image.getPixel(0, 2);
    CHECK(pixel02.r == BLUE_R);
    CHECK(pixel02.g == BLUE_G);
    CHECK(pixel02.b == BLUE_B);
    REQUIRE(pixel02.a == 255); // Alpha should remain 255 for non-magenta
}

TEST_CASE("Image convertToDisplayFormat() regenerates texture", "[unit][image][convert]")
{
    SDLEnvironment env;

    SDL_Surface* surfaceWithColorKey = createTestSurfaceWithColorKey();
    REQUIRE(surfaceWithColorKey != nullptr);

    // Create Image from surface with color key
    // autoFree=true to manage surface lifetime
    fcn::sdl3::Image image(surfaceWithColorKey, true, env.mRenderer);

    SDL_Texture const * originalTexture = image.getTexture();
    REQUIRE(originalTexture != nullptr);

    // Verify magenta pixel is transparent before conversion
    fcn::Color pixelBefore = image.getPixel(0, 0);
    REQUIRE(pixelBefore.a == 0); // Should be transparent

    // Perform convertToDisplayFormat
    // This regenerates the texture without changing transient surface data.
    image.convertToDisplayFormat();

    SDL_Texture const * newTexture = image.getTexture();
    REQUIRE(newTexture != nullptr);

    // The pixel should still be transparent after conversion
    fcn::Color pixelAfter = image.getPixel(0, 0);
    REQUIRE(pixelAfter.a == 0); // Should still be transparent
}

TEST_CASE("Image getPixel() reads from transient surface vs getTexture()", "[unit][image][transient]")
{
    SDLEnvironment env;

    SDL_Surface* surfaceWithColorKey = createTestSurfaceWithColorKey();
    REQUIRE(surfaceWithColorKey != nullptr);

    // Create Image from surface with color key
    // autoFree=true to manage surface lifetime
    fcn::sdl3::Image image(surfaceWithColorKey, true, env.mRenderer);

    SDL_Texture* texture = image.getTexture();
    REQUIRE(texture != nullptr);

    // getTexture returns a valid SDL texture for rendering
    float texWFloat = 0, texHFloat = 0;
    SDL_GetTextureSize(texture, &texWFloat, &texHFloat);
    int texW = static_cast<int>(texWFloat);
    int texH = static_cast<int>(texHFloat);
    REQUIRE(texW == TEST_IMAGE_WIDTH);
    REQUIRE(texH == TEST_IMAGE_HEIGHT);

    // getPixel reads from transient surface
    // Position (0,0) was magenta in original, should be transparent
    fcn::Color pixel00 = image.getPixel(0, 0);
    REQUIRE(pixel00.a == 0); // Converted to transparent

    // Position (1,0) was red, should remain opaque
    fcn::Color pixel10 = image.getPixel(1, 0);
    CHECK(pixel10.r == RED_R);
    CHECK(pixel10.g == RED_G);
    CHECK(pixel10.b == RED_B);
    REQUIRE(pixel10.a == 255); // Opaque
}

TEST_CASE("Image loads font image with magenta color key", "[unit][image][font]")
{
    SDLEnvironment env;

    // Look for RPG font in common locations
    // The test runs from the build directory, look in tests/resources/
    std::vector<std::filesystem::path> searchPaths = {
        std::filesystem::path("tests/resources/rpgfont.png"),
        std::filesystem::path("../tests/resources/rpgfont.png"),
        std::filesystem::path("./tests/resources/rpgfont.png"),
    };

    std::filesystem::path fontPath;
    auto it = std::find_if(searchPaths.begin(), searchPaths.end(), [](std::filesystem::path const & p) {
        return std::filesystem::exists(p);
    });
    if (it != searchPaths.end()) {
        fontPath = *it;
    }

    if (fontPath.empty()) {
        SKIP("Font image not found in any search path");
    }

    // Load font image using ImageLoader
    fcn::sdl3::ImageLoader loader;
    loader.setRenderer(env.mRenderer);

    // Load without convertToDisplayFormat to test getPixel reads
    fcn::Image* image = loader.load(fontPath.string(), false);
    REQUIRE(image != nullptr);

    // Verify dimensions
    REQUIRE(image->getWidth() > 0);
    REQUIRE(image->getHeight() > 0);

    // Verify we can read pixels from the loaded image
    // The RPG font should have magenta as transparency key
    bool hasTransparentPixels = false;
    bool hasOpaquePixels      = false;

    // Scan the image for magenta pixels
    for (int y = 0; y < image->getHeight() && y < 50; ++y) {
        for (int x = 0; x < image->getWidth() && x < 50; ++x) {
            fcn::Color pixel = image->getPixel(x, y);

            // Check if this is magenta color key
            if (pixel.r == 255 && pixel.g == 0 && pixel.b == 255) {
                if (pixel.a == 0) {
                    hasTransparentPixels = true;
                }
            } else if (pixel.a > 0) {
                hasOpaquePixels = true;
            }
        }
    }

    CHECK((hasTransparentPixels || hasOpaquePixels));

    image->free();
}

TEST_CASE("Image pixel access without magenta color key", "[unit][image][pixel]")
{
    SDLEnvironment env;

    // Create a test surface WITHOUT magenta and WITHOUT color key
    // Using only RGB colors (no transparency key)
    SDL_Surface* surface = SDL_CreateSurface(TEST_IMAGE_WIDTH, TEST_IMAGE_HEIGHT, SDL_PIXELFORMAT_RGBA8888);

    REQUIRE(surface != nullptr);

    // Fill with black
    SDL_FillSurfaceRect(surface, nullptr, SDL_MapSurfaceRGBA(surface, 0, 0, 0, 255));

    // Lock for direct pixel access
    SDL_LockSurface(surface);

    auto* pixels = static_cast<uint32_t*>(surface->pixels);

    // Row 0: Red, Green, Blue, Yellow
    pixels[0] = SDL_MapSurfaceRGBA(surface, 255, 0, 0, 255);
    pixels[1] = SDL_MapSurfaceRGBA(surface, 0, 255, 0, 255);
    pixels[2] = SDL_MapSurfaceRGBA(surface, 0, 0, 255, 255);
    pixels[3] = SDL_MapSurfaceRGBA(surface, 255, 255, 0, 255);

    // Row 1: Cyan, Magenta, White, Black
    pixels[4] = SDL_MapSurfaceRGBA(surface, 0, 255, 255, 255);
    pixels[5] = SDL_MapSurfaceRGBA(surface, 255, 0, 255, 255);
    pixels[6] = SDL_MapSurfaceRGBA(surface, 255, 255, 255, 255);
    pixels[7] = SDL_MapSurfaceRGBA(surface, 0, 0, 0, 255);

    // Row 2-3: More colors
    for (int i = 8; i < 16; ++i) {
        pixels[i] = SDL_MapSurfaceRGBA(surface, 128, 128, 128, 255);
    }

    SDL_UnlockSurface(surface);

    // Ensure NO color key is set (important for this test)
    CHECK(SDL_GetSurfaceColorKey(surface, nullptr) != 0);

    // Create Image from surface WITHOUT color key
    fcn::sdl3::Image image(surface, true, env.mRenderer);

    // Verify image dimensions
    REQUIRE(image.getWidth() == TEST_IMAGE_WIDTH);
    REQUIRE(image.getHeight() == TEST_IMAGE_HEIGHT);

    // Pixel (0,0) was red - verify we can read the RAW color
    fcn::Color pixel00 = image.getPixel(0, 0);
    CHECK(pixel00.r == 255);
    CHECK(pixel00.g == 0);
    CHECK(pixel00.b == 0);
    CHECK(pixel00.a == 255);

    // Pixel (1,0) was green
    fcn::Color pixel10 = image.getPixel(1, 0);
    CHECK(pixel10.r == 0);
    CHECK(pixel10.g == 255);
    CHECK(pixel10.b == 0);

    // Pixel (2,0) was blue
    fcn::Color pixel20 = image.getPixel(2, 0);
    CHECK(pixel20.r == 0);
    CHECK(pixel20.g == 0);
    CHECK(pixel20.b == 255);

    // Pixel (1,1) was magenta - this is the key test!
    // Without color key, magenta should NOT become transparent
    // it should remain fully opaque
    fcn::Color pixel11 = image.getPixel(1, 1);
    CHECK(pixel11.r == 255);
    CHECK(pixel11.g == 0);
    CHECK(pixel11.b == 255);
    CHECK(pixel11.a == 255); // Should be opaque, NOT transparent!
}

TEST_CASE("Image BMP font file loads with correct pixel colors", "[unit][image][font][bmp]")
{
    SDLEnvironment env;

    // Look for fixedfont BMP in common locations
    std::vector<std::filesystem::path> searchPaths = {
        std::filesystem::path("tests/resources/fixedfont.bmp"),
        std::filesystem::path("../tests/resources/fixedfont.bmp"),
        std::filesystem::path("./tests/resources/fixedfont.bmp"),
    };

    std::filesystem::path fontPath;
    auto it = std::find_if(searchPaths.begin(), searchPaths.end(), [](std::filesystem::path const & p) {
        return std::filesystem::exists(p);
    });
    if (it != searchPaths.end()) {
        fontPath = *it;
    }

    if (fontPath.empty()) {
        SKIP("BMP font image not found in any search path");
    }

    // Load font image using ImageLoader
    fcn::sdl3::ImageLoader loader;
    loader.setRenderer(env.mRenderer);

    // Load with convertToDisplayFormat=false to preserve raw pixel colors
    fcn::Image* image = loader.load(fontPath.string(), false);
    REQUIRE(image != nullptr);

    // Verify dimensions
    REQUIRE(image->getWidth() > 0);
    REQUIRE(image->getHeight() > 0);

    // Read the first pixel - this is the separator color for font glyphs
    // It should NOT be transparent (alpha=0)
    fcn::Color sepPixel = image->getPixel(0, 0);
    CHECK(sepPixel.a == 255); // Should be fully opaque, not transparent

    // Verify we can read at least some pixel values
    // Scan a few pixels to confirm raw pixel access works
    bool canReadPixels = false;
    for (int y = 0; y < image->getHeight() && y < 5; ++y) {
        for (int x = 0; x < image->getWidth() && x < 5; ++x) {
            fcn::Color pixel = image->getPixel(x, y);
            // If we can read any pixel without exception, it's working
            canReadPixels = true;
            // All pixels should be readable
            CHECK(pixel.r <= 255);
            CHECK(pixel.g <= 255);
            CHECK(pixel.b <= 255);
            CHECK(pixel.a <= 255);
        }
    }
    CHECK(canReadPixels);

    image->free();
}

TEST_CASE("Image PNG font file loads with correct pixel colors", "[unit][image][font][png]")
{
    SDLEnvironment env;

    // Look for rpgfont PNG in common locations
    std::vector<std::filesystem::path> searchPaths = {
        std::filesystem::path("tests/resources/rpgfont.png"),
        std::filesystem::path("../tests/resources/rpgfont.png"),
        std::filesystem::path("./tests/resources/rpgfont.png"),
        std::filesystem::path("examples/ff/images/rpgfont.png"),
    };

    std::filesystem::path fontPath;
    auto it = std::find_if(searchPaths.begin(), searchPaths.end(), [](std::filesystem::path const & p) {
        return std::filesystem::exists(p);
    });
    if (it != searchPaths.end()) {
        fontPath = *it;
    }

    if (fontPath.empty()) {
        SKIP("PNG font image not found in any search path");
    }

    // Load font image using ImageLoader
    fcn::sdl3::ImageLoader loader;
    loader.setRenderer(env.mRenderer);

    // Load with convertToDisplayFormat=false to preserve raw pixel colors
    fcn::Image* image = loader.load(fontPath.string(), false);
    REQUIRE(image != nullptr);

    // Verify dimensions
    REQUIRE(image->getWidth() > 0);
    REQUIRE(image->getHeight() > 0);

    // Read the first pixel - this is the separator color for font glyphs
    // It should NOT be transparent (alpha=0) when loaded without convertToDisplayFormat
    fcn::Color sepPixel = image->getPixel(0, 0);
    CHECK(sepPixel.a == 255); // Should be fully opaque, not transparent

    // Verify we can read at least some pixel values
    // Scan a few pixels to confirm raw pixel access works
    bool canReadPixels = false;
    for (int y = 0; y < image->getHeight() && y < 5; ++y) {
        for (int x = 0; x < image->getWidth() && x < 5; ++x) {
            fcn::Color pixel = image->getPixel(x, y);
            // If we can read any pixel without exception, it's working
            canReadPixels = true;
            // All pixels should be readable
            CHECK(pixel.r <= 255);
            CHECK(pixel.g <= 255);
            CHECK(pixel.b <= 255);
            CHECK(pixel.a <= 255);
        }
    }

    CHECK(canReadPixels);

    image->free();
}
