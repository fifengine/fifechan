// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2026 Fifengine contributors

// Standard library includes
#include <memory>
#include <stdexcept>
#include <string>

// Third-party library includes
#include <SDL3/SDL.h>

#include <catch2/catch_test_macros.hpp>

// Project headers
#include "fifechan/backends/sdl3/input.hpp"
#include "fifechan/genericinput.hpp"
#include "fifechan/gui.hpp"
#include "fifechan/widgets/button.hpp"
#include "fifechan/widgets/container.hpp"
#include "fifechan/widgets/textbox.hpp"
#include "fifechan/widgets/textfield.hpp"

namespace
{
    constexpr int TEST_WINDOW_WIDTH  = 100;
    constexpr int TEST_WINDOW_HEIGHT = 50;

    /**
     * Initializes SDL video and creates two hidden windows.
     *
     * Two windows are needed to prove that replacing the backend input stops
     * text input on the old window before the old input object is destroyed.
     */
    class SDLWindowEnvironment
    {
        public:
            SDLWindowEnvironment()
            {
                if (!SDL_Init(SDL_INIT_VIDEO)) {
                    throw std::runtime_error(std::string("SDL_Init failed: ") + SDL_GetError());
                }

                mWindow = SDL_CreateWindow(
                    "FifeGUI Gui Text Input Test", TEST_WINDOW_WIDTH, TEST_WINDOW_HEIGHT, SDL_WINDOW_HIDDEN);

                if (mWindow == nullptr) {
                    SDL_Quit();
                    throw std::runtime_error(std::string("SDL_CreateWindow failed: ") + SDL_GetError());
                }

                mSecondaryWindow = SDL_CreateWindow(
                    "FifeGUI Gui Text Input Test 2", TEST_WINDOW_WIDTH, TEST_WINDOW_HEIGHT, SDL_WINDOW_HIDDEN);

                if (mSecondaryWindow == nullptr) {
                    SDL_DestroyWindow(mWindow);
                    SDL_Quit();
                    throw std::runtime_error(std::string("SDL_CreateWindow failed: ") + SDL_GetError());
                }
            }

            ~SDLWindowEnvironment()
            {
                if (mSecondaryWindow != nullptr) {
                    SDL_DestroyWindow(mSecondaryWindow);
                }
                if (mWindow != nullptr) {
                    SDL_DestroyWindow(mWindow);
                }
                SDL_Quit();
            }

            SDLWindowEnvironment(SDLWindowEnvironment const &)            = delete;
            SDLWindowEnvironment& operator=(SDLWindowEnvironment const &) = delete;
            SDLWindowEnvironment(SDLWindowEnvironment&&)                  = delete;
            SDLWindowEnvironment& operator=(SDLWindowEnvironment&&)       = delete;

            SDL_Window* window() const
            {
                return mWindow;
            }

            SDL_Window* secondaryWindow() const
            {
                return mSecondaryWindow;
            }

        private:
            SDL_Window* mWindow{nullptr};
            SDL_Window* mSecondaryWindow{nullptr};
    };
} // namespace

// ============================================================================
// Focus-driven text input lifecycle
// ============================================================================

TEST_CASE("Gui starts text input when a text field gains focus", "[unit][gui][textinput][sdl3]")
{
    SDLWindowEnvironment env;
    fcn::Container top;
    fcn::TextField field;
    top.add(&field);

    fcn::Gui gui;
    gui.setTop(&top);
    gui.setInput(std::make_unique<fcn::sdl3::Input>(env.window()));

    REQUIRE_FALSE(SDL_TextInputActive(env.window()));

    field.requestFocus();
    gui.logic();

    REQUIRE(SDL_TextInputActive(env.window()));
}

TEST_CASE("Gui stops text input when focus moves to a button", "[unit][gui][textinput][sdl3]")
{
    SDLWindowEnvironment env;
    fcn::Container top;
    fcn::TextField field;
    fcn::Button button;
    top.add(&field);
    top.add(&button);

    fcn::Gui gui;
    gui.setTop(&top);
    gui.setInput(std::make_unique<fcn::sdl3::Input>(env.window()));

    field.requestFocus();
    gui.logic();
    REQUIRE(SDL_TextInputActive(env.window()));

    button.requestFocus();
    gui.logic();

    REQUIRE_FALSE(SDL_TextInputActive(env.window()));
}

TEST_CASE("Gui keeps text input active between text widgets", "[unit][gui][textinput][sdl3]")
{
    SDLWindowEnvironment env;
    fcn::Container top;
    fcn::TextField field;
    fcn::TextBox box;
    top.add(&field);
    top.add(&box);

    fcn::Gui gui;
    gui.setTop(&top);
    gui.setInput(std::make_unique<fcn::sdl3::Input>(env.window()));

    field.requestFocus();
    gui.logic();
    REQUIRE(SDL_TextInputActive(env.window()));

    box.requestFocus();
    gui.logic();

    REQUIRE(SDL_TextInputActive(env.window()));
}

TEST_CASE("Gui focusNone stops text input immediately", "[unit][gui][textinput][sdl3]")
{
    SDLWindowEnvironment env;
    fcn::Container top;
    fcn::TextField field;
    top.add(&field);

    fcn::Gui gui;
    gui.setTop(&top);
    gui.setInput(std::make_unique<fcn::sdl3::Input>(env.window()));

    field.requestFocus();
    gui.logic();
    REQUIRE(SDL_TextInputActive(env.window()));

    gui.focusNone();

    REQUIRE_FALSE(SDL_TextInputActive(env.window()));
}

TEST_CASE("Gui clearing the input stops text input", "[unit][gui][textinput][sdl3]")
{
    SDLWindowEnvironment env;
    fcn::Container top;
    fcn::TextField field;
    top.add(&field);

    fcn::Gui gui;
    gui.setTop(&top);
    gui.setInput(std::make_unique<fcn::sdl3::Input>(env.window()));

    field.requestFocus();
    gui.logic();
    REQUIRE(SDL_TextInputActive(env.window()));

    gui.setInput(static_cast<fcn::Input*>(nullptr));

    REQUIRE(gui.getInput() == nullptr);
    REQUIRE_FALSE(SDL_TextInputActive(env.window()));
}

TEST_CASE("Gui destructor stops text input", "[unit][gui][textinput][sdl3]")
{
    SDLWindowEnvironment env;
    fcn::Container top;
    fcn::TextField field;
    top.add(&field);

    {
        fcn::Gui gui;
        gui.setTop(&top);
        gui.setInput(std::make_unique<fcn::sdl3::Input>(env.window()));

        field.requestFocus();
        gui.logic();
        REQUIRE(SDL_TextInputActive(env.window()));
    }

    REQUIRE_FALSE(SDL_TextInputActive(env.window()));
}

TEST_CASE("Gui stops text input when the focused field becomes read-only", "[unit][gui][textinput][sdl3]")
{
    SDLWindowEnvironment env;
    fcn::Container top;
    fcn::TextField field;
    top.add(&field);

    fcn::Gui gui;
    gui.setTop(&top);
    gui.setInput(std::make_unique<fcn::sdl3::Input>(env.window()));

    field.requestFocus();
    gui.logic();
    REQUIRE(SDL_TextInputActive(env.window()));

    field.setEditable(false);
    gui.logic();

    REQUIRE_FALSE(SDL_TextInputActive(env.window()));
}

TEST_CASE("Gui stops text input when the focused field becomes disabled", "[unit][gui][textinput][sdl3]")
{
    SDLWindowEnvironment env;
    fcn::Container top;
    fcn::TextField field;
    top.add(&field);

    fcn::Gui gui;
    gui.setTop(&top);
    gui.setInput(std::make_unique<fcn::sdl3::Input>(env.window()));

    field.requestFocus();
    gui.logic();
    REQUIRE(SDL_TextInputActive(env.window()));

    field.setEnabled(false);
    gui.logic();

    REQUIRE_FALSE(SDL_TextInputActive(env.window()));
}

TEST_CASE("Gui stops text input when an ancestor is hidden", "[unit][gui][textinput][sdl3]")
{
    SDLWindowEnvironment env;
    fcn::Container top;
    fcn::Container panel;
    fcn::TextField field;
    panel.add(&field);
    top.add(&panel);

    fcn::Gui gui;
    gui.setTop(&top);
    gui.setInput(std::make_unique<fcn::sdl3::Input>(env.window()));

    field.requestFocus();
    gui.logic();
    REQUIRE(SDL_TextInputActive(env.window()));

    panel.setVisible(false);
    gui.logic();

    REQUIRE_FALSE(SDL_TextInputActive(env.window()));
}

TEST_CASE("Gui keeps text input active across unchanged frames", "[unit][gui][textinput][sdl3]")
{
    SDLWindowEnvironment env;
    fcn::Container top;
    fcn::TextField field;
    top.add(&field);

    fcn::Gui gui;
    gui.setTop(&top);
    gui.setInput(std::make_unique<fcn::sdl3::Input>(env.window()));

    field.requestFocus();
    gui.logic();
    gui.logic();
    gui.logic();

    REQUIRE(SDL_TextInputActive(env.window()));
}

TEST_CASE("Gui stops text input on the old input before destroying it", "[unit][gui][textinput][sdl3]")
{
    SDLWindowEnvironment env;
    fcn::Container top;
    fcn::TextField field;
    top.add(&field);

    fcn::Gui gui;
    gui.setTop(&top);
    gui.setInput(std::make_unique<fcn::sdl3::Input>(env.window()));

    field.requestFocus();
    gui.logic();
    REQUIRE(SDL_TextInputActive(env.window()));
    REQUIRE_FALSE(SDL_TextInputActive(env.secondaryWindow()));

    gui.setInput(std::make_unique<fcn::sdl3::Input>(env.secondaryWindow()));

    REQUIRE_FALSE(SDL_TextInputActive(env.window()));

    gui.logic();
    REQUIRE(SDL_TextInputActive(env.secondaryWindow()));
}

TEST_CASE("Gui clears text input when the top widget is replaced", "[unit][gui][textinput][sdl3]")
{
    SDLWindowEnvironment env;
    fcn::Container top;
    fcn::TextField field;
    top.add(&field);

    fcn::Container replacement;

    fcn::Gui gui;
    gui.setTop(&top);
    gui.setInput(std::make_unique<fcn::sdl3::Input>(env.window()));

    field.requestFocus();
    gui.logic();
    REQUIRE(SDL_TextInputActive(env.window()));

    gui.setTop(&replacement);

    REQUIRE_FALSE(SDL_TextInputActive(env.window()));
}

// ============================================================================
// Backend-agnostic text delivery
// ============================================================================

TEST_CASE("Gui delivers GenericInput text to the focused text field", "[unit][gui][textinput][genericinput]")
{
    fcn::Container top;
    fcn::TextField field("");
    top.add(&field);

    fcn::Gui gui;
    gui.setTop(&top);
    gui.setInput(std::make_unique<fcn::GenericInput>());

    auto* const input = static_cast<fcn::GenericInput*>(gui.getInput());

    field.requestFocus();
    input->pushTextInput("abc");
    gui.logic();

    REQUIRE(field.getText() == "abc");
}

TEST_CASE("Gui drops GenericInput text when no text widget is focused", "[unit][gui][textinput][genericinput]")
{
    fcn::Container top;
    fcn::TextField field("");
    fcn::Button button;
    top.add(&field);
    top.add(&button);

    fcn::Gui gui;
    gui.setTop(&top);
    gui.setInput(std::make_unique<fcn::GenericInput>());

    auto* const input = static_cast<fcn::GenericInput*>(gui.getInput());

    button.requestFocus();
    input->pushTextInput("abc");
    gui.logic();

    REQUIRE(field.getText().empty());
    REQUIRE(input->isTextQueueEmpty());
}
