// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2026 Fifengine contributors

#ifndef TESTS_INTEGRATION_SDL_SDLSPEECHBUBBLE_HPP_
#define TESTS_INTEGRATION_SDL_SDLSPEECHBUBBLE_HPP_

// Standard library includes
#include <filesystem>
#include <memory>
#include <string>
#include <utility>
#include <vector>

// Third-party library includes
#include <SDL3/SDL.h>

#include <fifechan/backends/sdl3/sdl.hpp>
#include <fifechan/gui.hpp>

#include <fifechan.hpp>

namespace tests::integration::sdl::speechbubble
{

    class ActionDisplay : public fcn::ActionListener
    {
        public:
            explicit ActionDisplay(fcn::Label* target) : mTarget(target)
            {
            }
            void action(fcn::ActionEvent const & actionEvent) override;

        private:
            fcn::Label* mTarget;
    };

    class OptionMouseListener : public fcn::MouseListener
    {
        public:
            explicit OptionMouseListener(fcn::Label* target, std::string text) : mTarget(target), mText(std::move(text))
            {
            }
            void mouseClicked(fcn::MouseEvent& mouseEvent) override;

        private:
            fcn::Label* mTarget;
            std::string mText;
    };

    class Application
    {
        public:
            explicit Application(std::string const & title, int width = 1280, int height = 768);
            ~Application();
            Application(Application const &)            = delete;
            Application& operator=(Application const &) = delete;
            Application(Application&&)                  = delete;
            Application& operator=(Application&&)       = delete;

            void run();
            void screenshotPage(int page, char const * filename);

        private:
            static std::filesystem::path getExecutableDir();
            void init_sdl(std::string const & title, int width, int height);
            void init_gui(int width, int height);
            void cleanup();
            void showPage(int index);
            void showInstructions();
            void clearPage();

            static std::shared_ptr<SDL_Window> initWindow(std::string const & title, int width, int height, int flags);
            static std::shared_ptr<SDL_Renderer> initRenderer(std::shared_ptr<SDL_Window> const & window);

            bool running{true};

            std::shared_ptr<SDL_Window> window{nullptr};
            std::shared_ptr<SDL_Renderer> renderer{nullptr};

            std::unique_ptr<fcn::sdl3::Graphics> graphics;
            std::unique_ptr<fcn::sdl3::Input> input;
            std::shared_ptr<fcn::sdl3::ImageLoader> imageLoader;

            std::unique_ptr<fcn::Gui> gui;

            std::string exePath;

            std::unique_ptr<fcn::Container> top;
            fcn::Font* font{nullptr};
            std::unique_ptr<fcn::Label> instructionLabel;
            std::unique_ptr<fcn::Label> feedbackLabel;

            int mWidth{1280};
            int mHeight{768};
            int mCurrentPage{0};

            // Page widget ownership
            std::vector<std::unique_ptr<fcn::Container>> mBubbles;
            std::vector<std::unique_ptr<fcn::Widget>> mChildren;
            std::unique_ptr<ActionDisplay> mActionDisplay;
            std::vector<std::unique_ptr<OptionMouseListener>> mOptionListeners;
    };

} // namespace tests::integration::sdl::speechbubble

#endif // TESTS_INTEGRATION_SDL_SDLSPEECHBUBBLE_HPP_
