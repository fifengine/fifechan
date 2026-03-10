// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2026 Fifengine contributors

#ifndef TESTS_INTEGRATION_SDL_SDLHELLOWORLD_HPP_
#define TESTS_INTEGRATION_SDL_SDLHELLOWORLD_HPP_

#include <SDL2/SDL.h>

#include <fifechan/backends/sdl2/sdl.hpp>
#include <fifechan/gui.hpp>

#include <fifechan.hpp>

#include <filesystem>
#include <memory>
#include <string>

namespace tests::integration::sdl::helloworld
{

    class Application
    {
    public:
        explicit Application(std::string const & title, int width = 640, int height = 480);
        ~Application();

        void run();

    private:
        static std::filesystem::path getExecutableDir();
        void init_sdl(std::string const & title, int width, int height);
        void init_gui(int width, int height);
        void cleanup();

        static std::shared_ptr<SDL_Window> initWindow(std::string const & title, int width, int height, int flags);
        static std::shared_ptr<SDL_Renderer> initRenderer(std::shared_ptr<SDL_Window> const & window, int flags);

        bool running{true};

        std::shared_ptr<SDL_Window> window{nullptr};
        std::shared_ptr<SDL_Renderer> renderer{nullptr};

        std::unique_ptr<fcn::SDL2Graphics> graphics;
        std::unique_ptr<fcn::SDLInput> input;
        std::shared_ptr<fcn::SDLImageLoader> imageLoader;

        std::unique_ptr<fcn::Gui> gui;

        std::unique_ptr<fcn::Container> top;
        std::unique_ptr<fcn::ImageFont> font;
        std::unique_ptr<fcn::ImageFont> rpgFont;
        std::unique_ptr<fcn::Label> label;
        std::unique_ptr<fcn::Label> label2;
    };

} // namespace tests::integration::sdl::helloworld

#endif // TESTS_INTEGRATION_SDL_SDLHELLOWORLD_HPP_
