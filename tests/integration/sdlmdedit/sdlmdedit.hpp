// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2026 Fifengine contributors

#ifndef SDLMDEdit_HPP
#define SDLMDEdit_HPP

// Standard library includes
#include <filesystem>
#include <format>
#include <iostream>
#include <memory>
#include <string>

// Third-party library includes
#include <SDL3/SDL.h>

#include <fifechan/widgets/activitybar.hpp>
#include <fifechan/widgets/activitybaritem.hpp>
#include <fifechan/widgets/menubar.hpp>
#include <fifechan/widgets/menuitem.hpp>
#include <fifechan/widgets/menupopup.hpp>
#include <fifechan/widgets/primarypanel.hpp>
#include <fifechan/widgets/secondarypanel.hpp>
#include <fifechan/widgets/statusbar.hpp>

#include <fifechan/backends/sdl3/sdl.hpp>
#include <fifechan/gui.hpp>

#include <fifechan.hpp>

namespace tests::integration::sdl::mdedit
{

    class Application
    {
    public:
        explicit Application(std::string const & title, int width = 800, int height = 600);
        ~Application();
        Application(Application const &)            = delete;
        Application& operator=(Application const &) = delete;
        Application(Application&&)                  = delete;
        Application& operator=(Application&&)       = delete;

        void run();

    private:
        void init_SDL(std::string const & title, int width, int height);
        void init_GUI(int width, int height);
        void cleanup();
        static std::filesystem::path getExecutableDir();

        static std::shared_ptr<SDL_Window> initWindow(std::string const & title, int width, int height, int flags);
        static std::shared_ptr<SDL_Renderer> initRenderer(std::shared_ptr<SDL_Window> const & window);

        std::unique_ptr<fcn::Gui> gui;
        std::shared_ptr<SDL_Window> window{nullptr};
        std::shared_ptr<SDL_Renderer> renderer{nullptr};

        std::unique_ptr<fcn::sdl3::Graphics> graphics;
        std::unique_ptr<fcn::sdl3::Input> input;

        // New widgets used in demo
        std::unique_ptr<fcn::Container> top{nullptr};
        fcn::Font* font{nullptr};
        fcn::MenuBar* menuBar{nullptr};
        fcn::ActivityBar* activityBar{nullptr};
        // Keep the activity bar font alive while the application runs
        std::shared_ptr<fcn::Font> activityFont{nullptr};
        fcn::PrimaryPanel* primaryPanel{nullptr};
        fcn::SecondaryPanel* secondaryPanel{nullptr};
        fcn::StatusBar* statusBar{nullptr};
        fcn::TextBox* textBox{nullptr};

        // Track cursor position for status bar updates
        int mLastCaretRow{0};
        int mLastCaretColumn{0};
        std::string mLastText;

        // Helper to update status bar with cursor position and text stats
        void updateStatusBar();

        bool running{false};
    };

} // namespace tests::integration::sdl::mdedit

#endif // SDLMDEdit_HPP
