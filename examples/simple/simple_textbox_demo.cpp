// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2026 Fifengine contributors

// Standard library includes
#include <format>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <utility>

// Platform config include
#include "fifechan/platform.hpp"

// Third-party library includes
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include <fifechan/backends/opengl/graphics.hpp>
#include <fifechan/backends/opengl/imageloader.hpp>
#include <fifechan/backends/sdl3/input.hpp>

#include <fifechan.hpp>

/**
 * Demonstrates the minimal SDL3 + OpenGL integration path for Fifechan.
 *
 * The example creates an SDL window, wires SDL input into a FifeGUI GUI,
 * uses the OpenGL graphics backend and SDL-based image loader, loads a bitmap
 * font, and renders a single TextBox while processing the SDL event loop.
 */
int main(int /*argc*/, char** /*argv*/)
{
    auto input       = std::unique_ptr<fcn::sdl3::Input>();
    auto graphics    = std::unique_ptr<fcn::opengl::Graphics>();
    auto imageLoader = std::unique_ptr<fcn::opengl::ImageLoader>();
    auto font        = std::unique_ptr<fcn::ImageFont>();
    auto gui         = std::unique_ptr<fcn::Gui>();
    auto top         = std::unique_ptr<fcn::Container>();

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << "\n";
        return 1;
    }

    // Append library version to window title
    std::string const fifeguiVersion = fcn::fifechanVersion();
    std::string const title          = std::format("FifeGUI v{} - Simple TextBox Demo", fifeguiVersion);

    SDL_Window* window = SDL_CreateWindow(title.c_str(), 800, 600, SDL_WINDOW_OPENGL);
    if (window == nullptr) {
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << "\n";
        SDL_Quit();
        return 1;
    }

    SDL_GLContext glContext = SDL_GL_CreateContext(window);
    if (glContext == nullptr) {
        std::cerr << "SDL_GL_CreateContext Error: " << SDL_GetError() << "\n";
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    try {
        imageLoader = std::make_unique<fcn::opengl::ImageLoader>();
        fcn::Image::setImageLoader(imageLoader.get());

        graphics = std::make_unique<fcn::opengl::Graphics>(800, 600);
        input    = std::make_unique<fcn::sdl3::Input>();

        {
            fcn::ImageFontConfig cfg;
            cfg.strategy          = fcn::SeparatorStrategy::ExplicitColor;
            cfg.explicitSeparator = fcn::Color{255, 255, 0, 255}; // Yellow separator
            // cfg.verbose           = true;

            font = std::make_unique<fcn::ImageFont>(
                "rpgfont.png",
                " abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789.,!?-+/():;%&`'*#=[]\"",
                cfg);
        }

        fcn::Widget::setGlobalFont(font.get());

        gui = std::make_unique<fcn::Gui>();
        gui->setGraphics(graphics.get());
        gui->setInput(input.get());

        top = std::make_unique<fcn::Container>();
        top->setDimension(fcn::Rectangle(0, 0, 800, 600));
        top->setOpaque(false);
        gui->setTop(top.get());

        auto txtBox = std::make_unique<fcn::TextBox>();
        txtBox->setDimension(fcn::Rectangle(100, 50, 200, 200));
        txtBox->setText("Hello from Fifechan!\nThis is a simple TextBox demo.");

        top->addWidget(std::move(txtBox));

        bool running = true;
        SDL_Event evt;
        while (running) {
            while (SDL_PollEvent(&evt) != 0) {
                if (evt.type == SDL_EVENT_QUIT) {
                    running = false;
                }

                input->pushInput(evt);
            }

            gui->logic();
            gui->draw();
            SDL_GL_SwapWindow(window);
        }
    } catch (fcn::Exception const & exc) {
        std::cerr << exc.getMessage() << " " << exc.getLine() << "\n";
    }

    if (gui != nullptr) {
        gui->setTop(nullptr);
    }

    fcn::Widget::resetGlobalFont();

    fcn::Image::resetImageLoader();

    SDL_GL_DestroyContext(glContext);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
