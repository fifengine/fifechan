// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

/**
 * @brief This example shows the widgets present in FifeGUI using the SDL backend.
 */

#include "sdlwidgets.hpp"

#include <fifechan/backends/sdl2/sdl.hpp>
#include <fifechan/backends/sdl2/sdltruetypefont.hpp>
#include <fifechan/gui.hpp>

#include <fifechan.hpp>

#include <filesystem>
#include <iostream>
#include <memory>
#include <string>

#ifdef _WIN32
    #include <windows.h>
#endif

namespace
{
    bool running = true;

    std::filesystem::path getExecutableDir()
    {
#ifdef __linux__
        return std::filesystem::read_symlink("/proc/self/exe").parent_path();
#elif defined(_WIN32)
        char buffer[MAX_PATH] = {0};
        GetModuleFileNameA(nullptr, buffer, MAX_PATH);
        return std::filesystem::path(buffer).parent_path();
#else
        return std::filesystem::current_path();
#endif
    }

    std::unique_ptr<SDL_Window, void (*)(SDL_Window*)> window(nullptr, SDL_DestroyWindow);
    std::unique_ptr<SDL_Renderer, void (*)(SDL_Renderer*)> renderer(nullptr, SDL_DestroyRenderer);
    std::unique_ptr<fcn::SDL2Graphics> graphics;
    std::unique_ptr<fcn::SDLInput> input;
    std::unique_ptr<fcn::SDLImageLoader> imageLoader;
    std::unique_ptr<fcn::SDLTrueTypeFont> loremFont;
    std::unique_ptr<fcn::Gui> gui;

    void init_sdl()
    {
        std::filesystem::current_path(getExecutableDir());

        SDL_Init(SDL_INIT_VIDEO);
        if (TTF_Init() == -1) {
            throw std::runtime_error(std::string("Failed to initialize SDL2_ttf: ") + TTF_GetError());
        }

        SDL_Window* rawWindow     = nullptr;
        SDL_Renderer* rawRenderer = nullptr;
        SDL_CreateWindowAndRenderer(640, 480, 0, &rawWindow, &rawRenderer);
        window.reset(rawWindow);
        renderer.reset(rawRenderer);
        SDL_SetWindowTitle(window.get(), "FifeGUI - SDL Widgets");

        imageLoader = std::make_unique<fcn::SDLImageLoader>();
        imageLoader->setRenderer(renderer.get());
        fcn::Image::setImageLoader(imageLoader.get());

        graphics = std::make_unique<fcn::SDL2Graphics>();
        graphics->setTarget(renderer.get(), 640, 480);

        input = std::make_unique<fcn::SDLInput>();

        gui = std::make_unique<fcn::Gui>();
        gui->setGraphics(graphics.get());
        gui->setInput(input.get());
    }

    void init_gui()
    {
        widgets::gui = gui.get();
        widgets::init();

        loremFont = std::make_unique<fcn::SDLTrueTypeFont>("ArchitectsDaughter.ttf", 18);
        widgets::textBox->setFont(loremFont.get());
        widgets::textBox->setText(
            "Lorem ipsum dolor sit amet\n"
            "consectetur adipiscing elit\n"
            "Integer vitae ultrices\n"
            "eros Curabitur malesuada dolor imperdiet\n"
            "ante facilisis ut convallis sem rutrum\n"
            "Praesent consequat urna convallis leo\n"
            "aliquam pellentesque Integer laoreet\n"
            "enim vehicula libero blandit at pellentesque\n"
            "ipsum vehicula Mauris id turpis hendrerit\n"
            "tempor velit nec hendrerit nulla");
    }

    void run()
    {
        while (running) {
            SDL_Event event;
            while (SDL_PollEvent(&event) != 0) {
                if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
                    running = false;
                }
                if (event.type == SDL_QUIT) {
                    running = false;
                }
                input->pushInput(event);
            }

            gui->logic();
            gui->draw();
            SDL_RenderPresent(renderer.get());
        }
    }

    void halt()
    {
        widgets::halt();
        widgets::gui = nullptr;
        loremFont.reset();
        gui.reset();
        fcn::Image::setImageLoader(nullptr);
        imageLoader.reset();
        input.reset();
        graphics.reset();
        renderer.reset();
        window.reset();
        TTF_Quit();
        SDL_Quit();
    }
} // namespace

int main(int argc, char** argv)
{
    (void)argc;
    (void)argv;

    try {
        init_sdl();
        init_gui();
        run();
        halt();
    } catch (fcn::Exception const & e) {
        std::cerr << e.getMessage() << '\n';
        return 1;
    } catch (std::exception const & e) {
        std::cerr << "Std exception: " << e.what() << '\n';
        return 1;
    } catch (...) {
        std::cerr << "Unknown exception" << '\n';
        return 1;
    }

    return 0;
}
