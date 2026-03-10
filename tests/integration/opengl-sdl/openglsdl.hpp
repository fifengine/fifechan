// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#ifndef TESTS_INTEGRATION_OPENGL_SDL_OPENGLSDL_HPP_
#define TESTS_INTEGRATION_OPENGL_SDL_OPENGLSDL_HPP_

/**
 * @brief This example shows the widgets present in FifeGUI using the OpenGL-SDL backend
 * and the SDL backend, for loading images and checking for user input.
 * OpenGL cannot load images nor check for user input, so an additional backend is needed.
 */

#include <fifechan/backends/opengl/opengl.hpp>
#include <fifechan/gui.hpp>

#include <fifechan.hpp>
// The openglsdlimageloader.hpp header file needs to be included
// in order to get the image loader that uses OpenGL and SDL.
#include <fifechan/backends/opengl/openglsdlimageloader.hpp>
#include <fifechan/backends/sdl2/sdl.hpp>

#include <cstdio>
#include <memory>

namespace openglsdl
{
    inline bool running = true;

    inline SDL_Window* window  = nullptr;
    inline SDL_Surface* screen = nullptr;

    // This examples uses two wrapper backends OpenGL and SDL for FifeGUI.
    // OpenGL is used for drawing and SDL for input and image loading.
    inline std::unique_ptr<fcn::OpenGLGraphics> graphics;
    inline std::unique_ptr<fcn::SDLInput> input;
    inline std::unique_ptr<fcn::OpenGLSDLImageLoader> imageLoader;

    inline std::unique_ptr<fcn::Gui> gui;

    /**
     * Initialises the OpenGL and SDL application. This function creates the global
     * Gui object that can be populated by various examples.
     */
    void init()
    {
        // Initialize SDL with video and timer subsystems.
        if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0) {
            printf("SDL_Init Error: %s\n", SDL_GetError());
            return;
        }

        // Create an SDL window with OpenGL context.
        window = SDL_CreateWindow(
            "My Game",
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

        // Create OpenGL context.
        SDL_GLContext glContext = SDL_GL_CreateContext(window);
        if (glContext == nullptr) {
            printf("SDL_GL_CreateContext Error: %s\n", SDL_GetError());
            SDL_DestroyWindow(window);
            SDL_Quit();
            return;
        }

        // Set the OpenGL viewport.
        glViewport(0, 0, 640, 480);
        glClearColor(0.0F, 0.0F, 0.0F, 0.0F);

        // SDL_TEXTEDITING events are not sent,
        // and text that is being composed will be rendered in its own UI
        // SDL_SetHint(SDL_HINT_IME_INTERNAL_EDIT, 1);

        // Enable key repeat.
        SDL_StartTextInput();

        // Now it's time to initialise OpenGL backend and the SDL backend.

        imageLoader = std::make_unique<fcn::OpenGLSDLImageLoader>();
        // Set the ImageLoader by calling a static function of the Image class.
        fcn::Image::setImageLoader(imageLoader.get());
        graphics = std::make_unique<fcn::OpenGLGraphics>();
        // We need to tell the OpenGL Graphics object how big the screen is.
        graphics->setTargetPlane(640, 480);
        input = std::make_unique<fcn::SDLInput>();

        // Finally, we create the Gui object and pass graphics and input to it.
        gui = std::make_unique<fcn::Gui>();
        gui->setGraphics(graphics.get());
        gui->setInput(input.get());
    }

    /**
     * Halts the OpenGL and SDL application.
     */
    void halt()
    {
        gui.reset();

        imageLoader.reset();
        input.reset();
        graphics.reset();

        SDL_DestroyWindow(window);
        SDL_Quit();
    }

    /**
     * Runs the OpenGL and SDL application.
     */
    void run()
    {
        // The main loop
        while (running) {
            // Check user input
            SDL_Event event;
            while (SDL_PollEvent(&event) != 0) {
                if (event.type == SDL_KEYDOWN) {
                    if (event.key.keysym.sym == SDLK_ESCAPE) {
                        running = false;
                    }
                    if (event.key.keysym.sym == SDLK_f) {
                        if ((event.key.keysym.mod & KMOD_CTRL) != 0) {
                            // Works with X11 only
                            // SDL_WM_ToggleFullScreen(screen);
                        }
                    }
                } else if (event.type == SDL_QUIT) {
                    running = false;
                }

                // After checking SDL events we forward the events to the GUI.
                input->pushInput(event);
            }

            gui->logic();

            gui->draw();

            SDL_UpdateWindowSurface(window);
        }
    }
} // namespace openglsdl

#endif // TESTS_INTEGRATION_OPENGL_SDL_OPENGLSDL_HPP_
