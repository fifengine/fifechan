// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2026 Fifengine contributors

// Standard library includes
#include <algorithm>
#include <filesystem>
#include <string>
#include <vector>

// Third-party library includes
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

#include <catch2/catch_test_macros.hpp>

// Project headers (subdirs before local)
#include "fifechan/backends/sdl3/graphics.hpp"
#include "fifechan/backends/sdl3/truetypefont.hpp"
#include "fifechan/fontloader.hpp"

namespace
{
    // SDL + TTF test environment (lifecycle managed per test case)
    struct SDLTTFEnvironment
    {
            SDLTTFEnvironment()
            {
                if (!SDL_Init(SDL_INIT_VIDEO)) {
                    throw std::runtime_error(std::string("SDL_Init failed: ") + SDL_GetError());
                }

                mWindow = SDL_CreateWindow("FifeGUI TrueTypeFont Test", 256, 256, SDL_WINDOW_HIDDEN);

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

                if (!TTF_Init()) {
                    SDL_DestroyRenderer(mRenderer);
                    SDL_DestroyWindow(mWindow);
                    SDL_Quit();
                    throw std::runtime_error(std::string("TTF_Init failed: ") + SDL_GetError());
                }
            }

            ~SDLTTFEnvironment()
            {
                TTF_Quit();
                SDL_DestroyRenderer(mRenderer);
                SDL_DestroyWindow(mWindow);
                SDL_Quit();
            }

            SDLTTFEnvironment(SDLTTFEnvironment const &)            = delete;
            SDLTTFEnvironment& operator=(SDLTTFEnvironment const &) = delete;

            SDL_Window* mWindow     = nullptr;
            SDL_Renderer* mRenderer = nullptr;
    };
} // anonymous namespace

// ─── Font Loading Tests ─────────────────────────────────────────────────────

TEST_CASE("OpenMoji color font loads successfully", "[unit][truetypefont][emoji]")
{
    SDLTTFEnvironment env;

    std::filesystem::path fontPath = fcn::font::FontLoader::findFontFile("OpenMoji-color-colr0_svg.ttf");
    if (fontPath.empty()) {
        SKIP("OpenMoji-color-colr0_svg.ttf not found in search paths");
    }

    // Load via SDL_ttf directly to verify the font file is valid
    TTF_Font* rawFont = TTF_OpenFont(fontPath.string().c_str(), 22.0f);
    REQUIRE(rawFont != nullptr);

    TTF_CloseFont(rawFont);
}

TEST_CASE("OpenMoji font reports correct glyph sizes for emoji", "[unit][truetypefont][emoji]")
{
    SDLTTFEnvironment env;

    std::filesystem::path fontPath = fcn::font::FontLoader::findFontFile("OpenMoji-color-colr0_svg.ttf");
    if (fontPath.empty()) {
        SKIP("OpenMoji-color-colr0_svg.ttf not found");
    }

    TTF_Font* rawFont = TTF_OpenFont(fontPath.string().c_str(), 22.0f);
    REQUIRE(rawFont != nullptr);

    // 📁 (U+1F4C1) - folder emoji, 4 bytes in UTF-8
    std::string const folderEmoji = "\xf0\x9f\x93\x81";
    int w = 0, h = 0;
    bool sizeOk = TTF_GetStringSize(rawFont, folderEmoji.c_str(), 0, &w, &h);
    CHECK(sizeOk);
    CHECK(w > 0);
    CHECK(h > 0);

    // 🔍 (U+1F50D) - magnifying glass emoji
    std::string const searchEmoji = "\xf0\x9f\x94\x8d";
    int w2 = 0, h2 = 0;
    bool sizeOk2 = TTF_GetStringSize(rawFont, searchEmoji.c_str(), 0, &w2, &h2);
    CHECK(sizeOk2);
    CHECK(w2 > 0);
    CHECK(h2 > 0);

    TTF_CloseFont(rawFont);
}

// ─── TrueTypeFont Wrapper Tests ─────────────────────────────────────────────

TEST_CASE("TrueTypeFont wraps OpenMoji font correctly", "[unit][truetypefont][emoji]")
{
    SDLTTFEnvironment env;

    std::filesystem::path fontPath = fcn::font::FontLoader::findFontFile("OpenMoji-color-colr0_svg.ttf");
    if (fontPath.empty()) {
        SKIP("OpenMoji-color-colr0_svg.ttf not found");
    }

    fcn::sdl3::TrueTypeFont* font = nullptr;
    REQUIRE_NOTHROW(font = new fcn::sdl3::TrueTypeFont(fontPath.string(), 22));
    REQUIRE(font != nullptr);

    // 📁 folder emoji
    std::string const folderEmoji = "\xf0\x9f\x93\x81";
    int const width               = font->getWidth(folderEmoji);
    CHECK(width > 0);

    int const height = font->getHeight();
    CHECK(height > 0);

    delete font;
}

TEST_CASE("TrueTypeFont draws emoji glyph without crashing", "[unit][truetypefont][emoji]")
{
    SDLTTFEnvironment env;

    std::filesystem::path fontPath = fcn::font::FontLoader::findFontFile("OpenMoji-color-colr0_svg.ttf");
    if (fontPath.empty()) {
        SKIP("OpenMoji-color-colr0_svg.ttf not found");
    }

    fcn::sdl3::TrueTypeFont* font = nullptr;
    REQUIRE_NOTHROW(font = new fcn::sdl3::TrueTypeFont(fontPath.string(), 22));
    REQUIRE(font != nullptr);

    fcn::sdl3::Graphics graphics;
    graphics.setTarget(env.mRenderer, 256, 256);
    graphics._beginDraw();

    std::string const folderEmoji = "\xf0\x9f\x93\x81"; // 📁
    graphics.setFont(font);
    graphics.setColor(fcn::Color(255, 255, 255, 255));

    // This should not throw or crash
    REQUIRE_NOTHROW(graphics.drawText(folderEmoji, 10, 10, fcn::Graphics::Alignment::Left));

    graphics._endDraw();

    delete font;
}

TEST_CASE("TrueTypeFont renders multiple emoji glyphs in sequence", "[unit][truetypefont][emoji]")
{
    SDLTTFEnvironment env;

    std::filesystem::path fontPath = fcn::font::FontLoader::findFontFile("OpenMoji-color-colr0_svg.ttf");
    if (fontPath.empty()) {
        SKIP("OpenMoji-color-colr0_svg.ttf not found");
    }

    fcn::sdl3::TrueTypeFont* font = nullptr;
    REQUIRE_NOTHROW(font = new fcn::sdl3::TrueTypeFont(fontPath.string(), 22));
    REQUIRE(font != nullptr);

    fcn::sdl3::Graphics graphics;
    graphics.setTarget(env.mRenderer, 512, 256);
    graphics._beginDraw();

    // Activity bar emoji set used in sdlmdedit
    std::vector<std::string> emojis = {
        "\xf0\x9f\x93\x81",        // 📁 folder
        "\xf0\x9f\x94\x8d",        // 🔍 search
        "\xe2\xad\x90",            // ⭐ star
        "\xe2\x9a\x99\xef\xb8\x8f" // ⚙️ gear
    };

    graphics.setFont(font);
    graphics.setColor(fcn::Color(255, 255, 255, 255));

    int xPos = 10;
    for (auto const & emoji : emojis) {
        int const w = font->getWidth(emoji);
        CHECK(w > 0);
        REQUIRE_NOTHROW(graphics.drawText(emoji, xPos, 10, fcn::Graphics::Alignment::Left));
        xPos += w + 8; // spacing
    }

    graphics._endDraw();

    delete font;
}

// ─── Color Font Surface Tests (direct SDL_ttf) ──────────────────────────────

TEST_CASE("TTF_RenderText_Blended produces valid surface for emoji", "[unit][truetypefont][emoji]")
{
    SDLTTFEnvironment env;

    std::filesystem::path fontPath = fcn::font::FontLoader::findFontFile("OpenMoji-color-colr0_svg.ttf");
    if (fontPath.empty()) {
        SKIP("OpenMoji-color-colr0_svg.ttf not found");
    }

    TTF_Font* rawFont = TTF_OpenFont(fontPath.string().c_str(), 22.0f);
    REQUIRE(rawFont != nullptr);

    std::string const folderEmoji = "\xf0\x9f\x93\x81"; // 📁
    SDL_Color white               = {255, 255, 255, 255};

    SDL_Surface* surface = TTF_RenderText_Blended(rawFont, folderEmoji.c_str(), 0, white);
    REQUIRE(surface != nullptr);

    // Color emoji fonts should produce ARGB surfaces with actual pixel data
    CHECK(surface->w > 0);
    CHECK(surface->h > 0);
    CHECK(surface->pitch > 0);

    // For color fonts, the surface should have color data (not just alpha)
    // ARGB8888 = 4 bytes per pixel
    CHECK((surface->format == SDL_PIXELFORMAT_ARGB8888 || surface->format == SDL_PIXELFORMAT_RGBA8888));

    SDL_DestroySurface(surface);
    TTF_CloseFont(rawFont);
}

TEST_CASE("TTF_RenderText_Blended produces non-empty pixel data for emoji", "[unit][truetypefont][emoji]")
{
    SDLTTFEnvironment env;

    std::filesystem::path fontPath = fcn::font::FontLoader::findFontFile("OpenMoji-color-colr0_svg.ttf");
    if (fontPath.empty()) {
        SKIP("OpenMoji-color-colr0_svg.ttf not found");
    }

    TTF_Font* rawFont = TTF_OpenFont(fontPath.string().c_str(), 22.0f);
    REQUIRE(rawFont != nullptr);

    std::string const folderEmoji = "\xf0\x9f\x93\x81"; // 📁
    SDL_Color white               = {255, 255, 255, 255};

    SDL_Surface* surface = TTF_RenderText_Blended(rawFont, folderEmoji.c_str(), 0, white);
    REQUIRE(surface != nullptr);

    // Check that the surface has non-zero pixel data
    // For a color emoji, we expect actual color values, not just alpha
    bool hasColorData     = false;
    Uint8* pixels         = static_cast<Uint8*>(surface->pixels);
    int const totalPixels = surface->w * surface->h;

    for (int i = 0; i < totalPixels; ++i) {
        int const offset = i * 4;              // 4 bytes per pixel (ARGB)
        Uint8 const r    = pixels[offset + 1]; // ARGB layout
        Uint8 const g    = pixels[offset + 2];
        Uint8 const b    = pixels[offset + 3];
        if (r > 0 || g > 0 || b > 0) {
            hasColorData = true;
            break;
        }
    }

    CHECK(hasColorData);

    SDL_DestroySurface(surface);
    TTF_CloseFont(rawFont);
}

TEST_CASE("TTF_RenderText_Blended creates valid texture from emoji surface", "[unit][truetypefont][emoji]")
{
    SDLTTFEnvironment env;

    std::filesystem::path fontPath = fcn::font::FontLoader::findFontFile("OpenMoji-color-colr0_svg.ttf");
    if (fontPath.empty()) {
        SKIP("OpenMoji-color-colr0_svg.ttf not found");
    }

    TTF_Font* rawFont = TTF_OpenFont(fontPath.string().c_str(), 22.0f);
    REQUIRE(rawFont != nullptr);

    std::string const folderEmoji = "\xf0\x9f\x93\x81"; // 📁
    SDL_Color white               = {255, 255, 255, 255};

    SDL_Surface* surface = TTF_RenderText_Blended(rawFont, folderEmoji.c_str(), 0, white);
    REQUIRE(surface != nullptr);

    SDL_Texture* texture = SDL_CreateTextureFromSurface(env.mRenderer, surface);
    REQUIRE(texture != nullptr);

    float texW = 0, texH = 0;
    bool queryOk = SDL_GetTextureSize(texture, &texW, &texH);
    CHECK(queryOk);
    CHECK(static_cast<int>(texW) == surface->w);
    CHECK(static_cast<int>(texH) == surface->h);

    SDL_DestroyTexture(texture);
    SDL_DestroySurface(surface);
    TTF_CloseFont(rawFont);
}

// ─── Regular Font Baseline Tests ────────────────────────────────────────────

TEST_CASE("Regular TTF font loads and renders as baseline", "[unit][truetypefont][baseline]")
{
    SDLTTFEnvironment env;

    std::filesystem::path fontPath = fcn::font::FontLoader::findFontFile("ArchitectsDaughter.ttf");
    if (fontPath.empty()) {
        SKIP("ArchitectsDaughter.ttf not found");
    }

    TTF_Font* rawFont = TTF_OpenFont(fontPath.string().c_str(), 22.0f);
    REQUIRE(rawFont != nullptr);

    std::string const text = "Hello World";
    int w = 0, h = 0;
    bool sizeOk = TTF_GetStringSize(rawFont, text.c_str(), 0, &w, &h);
    CHECK(sizeOk);
    CHECK(w > 0);
    CHECK(h > 0);

    SDL_Color white      = {255, 255, 255, 255};
    SDL_Surface* surface = TTF_RenderText_Blended(rawFont, text.c_str(), 0, white);
    REQUIRE(surface != nullptr);
    CHECK(surface->w > 0);
    CHECK(surface->h > 0);

    SDL_DestroySurface(surface);
    TTF_CloseFont(rawFont);
}

TEST_CASE("TrueTypeFont wrapper works with regular TTF font", "[unit][truetypefont][baseline]")
{
    SDLTTFEnvironment env;

    std::filesystem::path fontPath = fcn::font::FontLoader::findFontFile("ArchitectsDaughter.ttf");
    if (fontPath.empty()) {
        SKIP("ArchitectsDaughter.ttf not found");
    }

    fcn::sdl3::TrueTypeFont* font = nullptr;
    REQUIRE_NOTHROW(font = new fcn::sdl3::TrueTypeFont(fontPath.string(), 22));
    REQUIRE(font != nullptr);

    CHECK(font->getWidth("Hello") > 0);
    CHECK(font->getHeight() > 0);

    fcn::sdl3::Graphics graphics;
    graphics.setTarget(env.mRenderer, 256, 256);
    graphics._beginDraw();

    graphics.setFont(font);
    graphics.setColor(fcn::Color(255, 255, 255, 255));
    REQUIRE_NOTHROW(graphics.drawText("Hello World", 10, 10, fcn::Graphics::Alignment::Left));

    graphics._endDraw();

    delete font;
}
