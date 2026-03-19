// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2026 Fifengine contributors

#ifndef TESTS_INTEGRATION_OPENGL_SDL_OPENGLSDLHELLOWORLD_HPP_
#define TESTS_INTEGRATION_OPENGL_SDL_OPENGLSDLHELLOWORLD_HPP_

#include <fifechan/backends/opengl/imageloader.hpp>
#include <fifechan/backends/opengl/opengl.hpp>
#include <fifechan/backends/sdl2/sdl.hpp>
#include <fifechan/exception.hpp>
#include <fifechan/gui.hpp>

#include <fifechan.hpp>

#include <cstdio>
#include <memory>

namespace openglsdl
{
    inline bool running = true;

    inline SDL_Window* window = nullptr;

    inline std::unique_ptr<fcn::opengl::Graphics> graphics;
    inline std::unique_ptr<fcn::sdl2::Input> input;
    inline std::unique_ptr<fcn::opengl::ImageLoader> imageLoader;

    inline std::unique_ptr<fcn::Gui> gui;

    inline void init()
    {
        if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0) {
            printf("SDL_Init Error: %s\n", SDL_GetError());
            return;
        }

        window = SDL_CreateWindow(
            "Hello World",
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            640,
            480,
            SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
        if (window == nullptr) {
            printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
            SDL_Quit();
            return;
        }

        SDL_GLContext glContext = SDL_GL_CreateContext(window);
        if (glContext == nullptr) {
            printf("SDL_GL_CreateContext Error: %s\n", SDL_GetError());
            SDL_DestroyWindow(window);
            SDL_Quit();
            return;
        }

        glViewport(0, 0, 640, 480);
        glClearColor(0.0F, 0.0F, 0.0F, 0.0F);

        SDL_StartTextInput();

        imageLoader = std::make_unique<fcn::opengl::ImageLoader>();
        fcn::Image::setImageLoader(imageLoader.get());
        graphics = std::make_unique<fcn::opengl::Graphics>();
        graphics->setTargetPlane(640, 480);
        input = std::make_unique<fcn::sdl2::Input>();

        gui = std::make_unique<fcn::Gui>();
        gui->setGraphics(graphics.get());
        gui->setInput(input.get());
    }

    inline void halt()
    {
        gui.reset();
        imageLoader.reset();
        input.reset();
        graphics.reset();
        SDL_DestroyWindow(window);
        SDL_Quit();
    }

    inline void run()
    {
        while (running) {
            SDL_Event event;
            while (SDL_PollEvent(&event) != 0) {
                if (event.type == SDL_KEYDOWN) {
                    if (event.key.keysym.sym == SDLK_ESCAPE) {
                        running = false;
                    }
                } else if (event.type == SDL_QUIT) {
                    running = false;
                }
                input->pushInput(event);
            }

            gui->logic();
            gui->draw();
            SDL_UpdateWindowSurface(window);
        }
    }
} // namespace openglsdl

namespace helloworld
{
    inline fcn::Container* top  = nullptr;
    inline fcn::ImageFont* font = nullptr;
    inline fcn::Label* label    = nullptr;

    inline void init()
    {
        if (openglsdl::gui == nullptr) {
            fcn::throwException("openglsdl::gui is null. Initialize GUI backend before helloworld::init().");
        }

        top = new fcn::Container();
        top->setDimension(fcn::Rectangle(0, 0, 640, 480));
        openglsdl::gui->setTop(top);

        font = new fcn::ImageFont("fixedfont.bmp", " abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789");
        fcn::Widget::setGlobalFont(font);

        label = new fcn::Label("Hello World");
        label->setPosition(280, 220);
        top->add(label);
    }

    inline void halt()
    {
        delete label;
        label = nullptr;

        delete font;
        font = nullptr;

        delete top;
        top = nullptr;
    }
} // namespace helloworld

#endif // TESTS_INTEGRATION_OPENGL_SDL_OPENGLSDLHELLOWORLD_HPP_
