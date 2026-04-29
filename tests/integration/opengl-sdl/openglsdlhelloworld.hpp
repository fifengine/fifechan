// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2026 Fifengine contributors

#ifndef TESTS_INTEGRATION_OPENGL_SDL_OPENGLSDLHELLOWORLD_HPP_
#define TESTS_INTEGRATION_OPENGL_SDL_OPENGLSDLHELLOWORLD_HPP_

// Standard library includes
#include <filesystem>
#include <memory>
#include <string>

// Third-party library includes
#include <SDL3/SDL.h>

#include <fifechan/backends/opengl/imageloader.hpp>
#include <fifechan/backends/opengl/opengl.hpp>
#include <fifechan/backends/sdl3/sdl.hpp>
#include <fifechan/gui.hpp>

#include <fifechan.hpp>

namespace tests::integration::opengl_sdl::helloworld
{

    class Application
    {
    public:
        explicit Application(std::string const & title, int width = 640, int height = 480);
        ~Application();
        Application(Application const &)            = delete;
        Application& operator=(Application const &) = delete;
        Application(Application&&)                  = delete;
        Application& operator=(Application&&)       = delete;

        void run();

    private:
        static std::filesystem::path getExecutableDir();
        void init_sdl(std::string const & title, int width, int height);
        void init_gui(int width, int height);
        void cleanup();

        static SDL_Window* initWindow(std::string const & title, int width, int height, int flags);
        static SDL_GLContext initGLContext(SDL_Window* window);

        bool running{true};

        SDL_Window* window{nullptr};
        SDL_GLContext glContext{nullptr};

        std::unique_ptr<fcn::opengl::Graphics> graphics;
        std::unique_ptr<fcn::sdl2::Input> input;
        std::shared_ptr<fcn::opengl::ImageLoader> imageLoader;

        std::unique_ptr<fcn::Gui> gui;

        std::unique_ptr<fcn::Container> top;
        std::unique_ptr<fcn::ImageFont> font;
        std::unique_ptr<fcn::Label> label;
    };

} // namespace tests::integration::opengl_sdl::helloworld

#endif // TESTS_INTEGRATION_OPENGL_SDL_OPENGLSDLHELLOWORLD_HPP_
