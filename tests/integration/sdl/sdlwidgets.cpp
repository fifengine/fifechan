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

    SDL_Window* window               = nullptr;
    SDL_Renderer* renderer           = nullptr;
    fcn::SDL2Graphics* graphics      = nullptr;
    fcn::SDLInput* input             = nullptr;
    fcn::SDLImageLoader* imageLoader = nullptr;
    fcn::SDLTrueTypeFont* loremFont  = nullptr;
    fcn::Gui* gui                    = nullptr;

    void init_sdl()
    {
        std::filesystem::current_path(getExecutableDir());

        SDL_Init(SDL_INIT_VIDEO);
        if (TTF_Init() == -1) {
            throw std::runtime_error(std::string("Failed to initialize SDL2_ttf: ") + TTF_GetError());
        }

        SDL_CreateWindowAndRenderer(640, 480, 0, &window, &renderer);
        SDL_SetWindowTitle(window, "FifeGUI - SDL Widgets");

        imageLoader = new fcn::SDLImageLoader();
        imageLoader->setRenderer(renderer);
        fcn::Image::setImageLoader(imageLoader);

        graphics = new fcn::SDL2Graphics();
        graphics->setTarget(renderer, 640, 480);

        input = new fcn::SDLInput();

        gui = new fcn::Gui();
        gui->setGraphics(graphics);
        gui->setInput(input);
    }

    void init_gui()
    {
        widgets::gui = gui;
        widgets::init();

        loremFont = new fcn::SDLTrueTypeFont("ArchitectsDaughter.ttf", 18);
        widgets::textBox->setFont(loremFont);
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
            SDL_RenderPresent(renderer);
        }
    }

    void halt()
    {
        widgets::halt();
        delete loremFont;
        loremFont = nullptr;
        delete gui;
        delete imageLoader;
        delete input;
        delete graphics;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
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
