// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#include <SDL2/SDL.h>

#include <fifechan/backends/opengl/openglgraphics.hpp>
#include <fifechan/backends/opengl/openglsdlimageloader.hpp>
#include <fifechan/backends/sdl2/sdlinput.hpp>

#include <fifechan.hpp>

#include <iostream>
#include <memory>
#include <sstream>

/**
 * Demonstrates the minimal SDL2 + OpenGL integration path for Fifechan.
 *
 * The example creates an SDL window, wires SDL input into a FifeGUI GUI,
 * uses the OpenGL graphics backend and SDL-based image loader, loads a bitmap
 * font, and renders a single TextBox while processing the SDL event loop.
 */
int main(int argc, char** argv)
{
    SDL_Window* window      = nullptr;
    SDL_GLContext glContext = nullptr;
    auto input              = std::unique_ptr<fcn::SDLInput>();
    auto graphics           = std::unique_ptr<fcn::OpenGLGraphics>();
    auto imageLoader        = std::unique_ptr<fcn::OpenGLSDLImageLoader>();
    auto font               = std::unique_ptr<fcn::ImageFont>();
    auto gui                = std::unique_ptr<fcn::Gui>();
    auto top                = std::unique_ptr<fcn::Container>();

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << "\n";
        return 1;
    }

    window = SDL_CreateWindow("Simple TextBox Demo", 0, 0, 800, 600, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << "\n";
        SDL_Quit();
        return 1;
    }

    glContext = SDL_GL_CreateContext(window);
    if (glContext == nullptr) {
        std::cerr << "SDL_GL_CreateContext Error: " << SDL_GetError() << "\n";
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    try {
        imageLoader = std::make_unique<fcn::OpenGLSDLImageLoader>();
        fcn::Image::setImageLoader(imageLoader.get());

        graphics = std::make_unique<fcn::OpenGLGraphics>(800, 600);
        input    = std::make_unique<fcn::SDLInput>();

        font = std::make_unique<fcn::ImageFont>(
            "rpgfont.png", " abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789.,!?-+/():;%&`'*#[]\"");
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
            while (SDL_PollEvent(&evt)) {
                if (evt.type == SDL_QUIT) {
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

    fcn::Widget::setGlobalFont(nullptr);

    fcn::Image::setImageLoader(nullptr);

    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
