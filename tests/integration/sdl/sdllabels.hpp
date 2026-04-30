// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2026 Fifengine contributors

#ifndef TESTS_INTEGRATION_SDL_SDLLABELS_HPP_
#define TESTS_INTEGRATION_SDL_SDLLABELS_HPP_

// Standard library includes
#include <filesystem>
#include <memory>
#include <string>
#include <vector>

// Third-party library includes
#include <SDL3/SDL.h>

#include <fifechan/backends/sdl3/sdl.hpp>
#include <fifechan/gui.hpp>

#include <fifechan.hpp>

namespace tests::integration::sdl::labels
{

    class Application
    {
    public:
        explicit Application(std::string const & title, int width = 1024, int height = 768);
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

        static std::shared_ptr<SDL_Window> initWindow(std::string const & title, int width, int height, int flags);
        static std::shared_ptr<SDL_Renderer> initRenderer(std::shared_ptr<SDL_Window> const & window);

        bool running{true};

        std::shared_ptr<SDL_Window> window{nullptr};
        std::shared_ptr<SDL_Renderer> renderer{nullptr};

        std::unique_ptr<fcn::sdl3::Graphics> graphics;
        std::unique_ptr<fcn::sdl3::Input> input;
        std::shared_ptr<fcn::sdl3::ImageLoader> imageLoader;

        std::unique_ptr<fcn::Gui> gui;

        std::unique_ptr<fcn::Container> top;
        std::unique_ptr<fcn::ImageFont> font;

        // 3x3 grid of labels for all alignment combinations
        std::vector<std::unique_ptr<fcn::Label>> labels;
        // Containers to hold each label with fixed size
        std::vector<std::unique_ptr<fcn::Container>> cells;
    };

} // namespace tests::integration::sdl::labels

#endif // TESTS_INTEGRATION_SDL_SDLLABELS_HPP_
