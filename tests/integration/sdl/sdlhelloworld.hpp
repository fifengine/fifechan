// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2026 Fifengine contributors

#ifndef TESTS_INTEGRATION_SDL_SDLHELLOWORLD_HPP_
#define TESTS_INTEGRATION_SDL_SDLHELLOWORLD_HPP_

#include <SDL2/SDL.h>

#include <fifechan/backends/sdl2/sdl.hpp>

#include <fifechan.hpp>

#include <filesystem>
#include <string>

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

    bool running{true};

    SDL_Window* window{nullptr};
    SDL_Renderer* renderer{nullptr};

    fcn::SDL2Graphics* graphics{nullptr};
    fcn::SDLInput* input{nullptr};
    fcn::SDLImageLoader* imageLoader{nullptr};

    fcn::Gui* gui{nullptr};

    fcn::Container* top{nullptr};
    fcn::ImageFont* font{nullptr};
    fcn::ImageFont* rpgFont{nullptr};
    fcn::Label* label{nullptr};
    fcn::Label* label2{nullptr};
};

#endif // TESTS_INTEGRATION_SDL_SDLHELLOWORLD_HPP_
