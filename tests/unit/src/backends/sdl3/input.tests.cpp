// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2013 - 2026 Fifengine contributors

// Standard library includes
#include <stdexcept>
#include <string>
#include <vector>

// Third-party library includes
#include <SDL3/SDL.h>

#include <catch2/catch_test_macros.hpp>

// Project headers (subdirs before local)
#include "fifechan/backends/sdl3/input.hpp"

namespace
{
    constexpr int TEST_WINDOW_WIDTH  = 100;
    constexpr int TEST_WINDOW_HEIGHT = 50;

    /**
     * Creates a hidden window so finger coordinates can be mapped to window
     * points. The window is required by SDL_GetWindowSize().
     */
    class SDLInputEnvironment
    {
        public:
            SDLInputEnvironment()
            {
                if (!SDL_Init(SDL_INIT_VIDEO)) {
                    throw std::runtime_error(std::string("SDL_Init failed: ") + SDL_GetError());
                }

                mWindow =
                    SDL_CreateWindow("FifeGUI Input Test", TEST_WINDOW_WIDTH, TEST_WINDOW_HEIGHT, SDL_WINDOW_HIDDEN);

                if (mWindow == nullptr) {
                    SDL_Quit();
                    throw std::runtime_error(std::string("SDL_CreateWindow failed: ") + SDL_GetError());
                }

                mWindowId = SDL_GetWindowID(mWindow);

                // Native touch handling is opt-in: SDL must not synthesize
                // mouse events from the same fingers.
                SDL_SetHint(SDL_HINT_TOUCH_MOUSE_EVENTS, "0");
            }

            ~SDLInputEnvironment()
            {
                SDL_SetHint(SDL_HINT_TOUCH_MOUSE_EVENTS, "1");

                if (mWindow != nullptr) {
                    SDL_DestroyWindow(mWindow);
                }
                SDL_Quit();
            }

            SDLInputEnvironment(SDLInputEnvironment const &)            = delete;
            SDLInputEnvironment& operator=(SDLInputEnvironment const &) = delete;
            SDLInputEnvironment(SDLInputEnvironment&&)                  = delete;
            SDLInputEnvironment& operator=(SDLInputEnvironment&&)       = delete;

            SDL_Window* mWindow    = nullptr;
            SDL_WindowID mWindowId = 0;
    };

    SDL_Event makeFinger(
        uint32_t type,
        SDL_TouchID touchId,
        SDL_FingerID fingerId,
        float x,
        float y,
        float dx              = 0.0F,
        float dy              = 0.0F,
        SDL_WindowID windowId = 0)
    {
        SDL_Event event{};
        event.type             = type;
        event.tfinger.touchID  = touchId;
        event.tfinger.fingerID = fingerId;
        event.tfinger.x        = x;
        event.tfinger.y        = y;
        event.tfinger.dx       = dx;
        event.tfinger.dy       = dy;
        event.tfinger.pressure = 0.5F;
        event.tfinger.windowID = windowId;
        return event;
    }

    SDL_Event makeMouseMotion(float x, float y, SDL_WindowID windowId)
    {
        SDL_Event event{};
        event.type            = SDL_EVENT_MOUSE_MOTION;
        event.motion.x        = x;
        event.motion.y        = y;
        event.motion.windowID = windowId;
        return event;
    }

    std::vector<fcn::MouseInput> drain(fcn::sdl3::Input& input)
    {
        std::vector<fcn::MouseInput> events;
        while (!input.isMouseQueueEmpty()) {
            events.push_back(input.dequeueMouseInput());
        }
        return events;
    }
} // namespace

TEST_CASE("SDL3 input translates a finger tap into mouse press and release", "[unit][input][sdl3][touch]")
{
    SDLInputEnvironment env;
    fcn::sdl3::Input input;

    input.pushInput(makeFinger(SDL_EVENT_FINGER_DOWN, 1, 10, 0.5F, 0.5F, 0.0F, 0.0F, env.mWindowId));
    input.pushInput(makeFinger(SDL_EVENT_FINGER_UP, 1, 10, 0.5F, 0.5F, 0.0F, 0.0F, env.mWindowId));

    std::vector<fcn::MouseInput> const events = drain(input);
    REQUIRE(events.size() == 2U);

    CHECK(events.at(0).getType() == fcn::MouseInput::Type::Pressed);
    CHECK(events.at(0).getButton() == fcn::MouseInput::Button::Left);
    CHECK(events.at(0).getX() == 50);
    CHECK(events.at(0).getY() == 25);

    CHECK(events.at(1).getType() == fcn::MouseInput::Type::Released);
    CHECK(events.at(1).getButton() == fcn::MouseInput::Button::Left);
    CHECK(events.at(1).getX() == 50);
    CHECK(events.at(1).getY() == 25);
}

TEST_CASE("SDL3 input translates one-finger motion into mouse movement", "[unit][input][sdl3][touch]")
{
    SDLInputEnvironment env;
    fcn::sdl3::Input input;

    input.pushInput(makeFinger(SDL_EVENT_FINGER_DOWN, 1, 10, 0.1F, 0.1F, 0.0F, 0.0F, env.mWindowId));
    input.pushInput(makeFinger(SDL_EVENT_FINGER_MOTION, 1, 10, 0.25F, 0.75F, 0.15F, 0.65F, env.mWindowId));

    std::vector<fcn::MouseInput> const events = drain(input);
    REQUIRE(events.size() == 2U);

    CHECK(events.at(0).getType() == fcn::MouseInput::Type::Pressed);

    CHECK(events.at(1).getType() == fcn::MouseInput::Type::Moved);
    CHECK(events.at(1).getButton() == fcn::MouseInput::Button::Left);
    CHECK(events.at(1).getX() == 25);
    CHECK(events.at(1).getY() == 38);
}

TEST_CASE("SDL3 input clamps normalized finger coordinates", "[unit][input][sdl3][touch]")
{
    SDLInputEnvironment env;
    fcn::sdl3::Input input;

    input.pushInput(makeFinger(SDL_EVENT_FINGER_DOWN, 1, 10, 1.5F, -0.5F, 0.0F, 0.0F, env.mWindowId));

    std::vector<fcn::MouseInput> const events = drain(input);
    REQUIRE(events.size() == 1U);
    CHECK(events.at(0).getX() == TEST_WINDOW_WIDTH);
    CHECK(events.at(0).getY() == 0);
}

TEST_CASE("SDL3 input turns a second finger into a canceled pointer", "[unit][input][sdl3][touch]")
{
    SDLInputEnvironment env;
    fcn::sdl3::Input input;

    input.pushInput(makeFinger(SDL_EVENT_FINGER_DOWN, 1, 10, 0.5F, 0.5F, 0.0F, 0.0F, env.mWindowId));

    std::vector<fcn::MouseInput> events = drain(input);
    REQUIRE(events.size() == 1U);
    CHECK(events.at(0).getType() == fcn::MouseInput::Type::Pressed);

    // The second finger turns the gesture into a pan. The synthetic pointer is
    // canceled instead of released so the widget does not receive a click.
    input.pushInput(makeFinger(SDL_EVENT_FINGER_DOWN, 1, 11, 0.75F, 0.5F, 0.0F, 0.0F, env.mWindowId));

    events = drain(input);
    REQUIRE(events.size() == 1U);
    CHECK(events.at(0).getType() == fcn::MouseInput::Type::Canceled);
    CHECK(events.at(0).getX() == 50);
    CHECK(events.at(0).getY() == 25);

    // A two-finger pan produces wheel input at the moving finger.
    input.pushInput(makeFinger(SDL_EVENT_FINGER_MOTION, 1, 10, 0.5F, 0.6F, 0.0F, 0.1F, env.mWindowId));

    events = drain(input);
    REQUIRE(events.size() == 1U);
    CHECK(events.at(0).getType() == fcn::MouseInput::Type::WheelMovedDown);
    CHECK(events.at(0).getX() == 50);
    CHECK(events.at(0).getY() == 30);

    // Lifting the fingers must not emit a release: the pointer was canceled.
    input.pushInput(makeFinger(SDL_EVENT_FINGER_UP, 1, 10, 0.5F, 0.6F, 0.0F, 0.0F, env.mWindowId));
    input.pushInput(makeFinger(SDL_EVENT_FINGER_UP, 1, 11, 0.75F, 0.5F, 0.0F, 0.0F, env.mWindowId));

    CHECK(input.isMouseQueueEmpty());
}

TEST_CASE("SDL3 input uses the dominant axis for touch panning", "[unit][input][sdl3][touch]")
{
    SDLInputEnvironment env;
    fcn::sdl3::Input input;

    input.pushInput(makeFinger(SDL_EVENT_FINGER_DOWN, 2, 20, 0.5F, 0.5F, 0.0F, 0.0F, env.mWindowId));
    input.pushInput(makeFinger(SDL_EVENT_FINGER_DOWN, 2, 21, 0.75F, 0.5F, 0.0F, 0.0F, env.mWindowId));
    drain(input);

    input.pushInput(makeFinger(SDL_EVENT_FINGER_MOTION, 2, 20, 0.6F, 0.54F, 0.1F, 0.04F, env.mWindowId));

    std::vector<fcn::MouseInput> events = drain(input);
    REQUIRE(events.size() == 1U);
    CHECK(events.at(0).getType() == fcn::MouseInput::Type::WheelMovedRight);
    CHECK(events.at(0).getX() == 60);
    CHECK(events.at(0).getY() == 27);

    // A mostly vertical pan must never produce a horizontal wheel callback.
    input.pushInput(makeFinger(SDL_EVENT_FINGER_MOTION, 2, 20, 0.6F, 0.64F, 0.0F, 0.1F, env.mWindowId));

    events = drain(input);
    REQUIRE(events.size() == 1U);
    CHECK(events.at(0).getType() == fcn::MouseInput::Type::WheelMovedDown);

    for (auto const & event : events) {
        CHECK(event.getType() != fcn::MouseInput::Type::WheelMovedLeft);
        CHECK(event.getType() != fcn::MouseInput::Type::WheelMovedUp);
    }
}

TEST_CASE("SDL3 input reports a canceled finger without a release", "[unit][input][sdl3][touch]")
{
    SDLInputEnvironment env;
    fcn::sdl3::Input input;

    input.pushInput(makeFinger(SDL_EVENT_FINGER_DOWN, 3, 30, 0.2F, 0.4F, 0.0F, 0.0F, env.mWindowId));
    drain(input);

    input.pushInput(makeFinger(SDL_EVENT_FINGER_CANCELED, 3, 30, 0.2F, 0.4F, 0.0F, 0.0F, env.mWindowId));

    std::vector<fcn::MouseInput> const events = drain(input);
    REQUIRE(events.size() == 1U);
    CHECK(events.at(0).getType() == fcn::MouseInput::Type::Canceled);
    CHECK(events.at(0).getX() == 20);
    CHECK(events.at(0).getY() == 20);
}

TEST_CASE("SDL3 input ignores synthetic finger events from the mouse", "[unit][input][sdl3][touch]")
{
    SDLInputEnvironment env;
    fcn::sdl3::Input input;

    input.pushInput(makeFinger(SDL_EVENT_FINGER_DOWN, SDL_MOUSE_TOUCHID, 40, 0.5F, 0.5F, 0.0F, 0.0F, env.mWindowId));

    CHECK(input.isMouseQueueEmpty());
}

TEST_CASE("SDL3 input defers to SDL synthesis when the touch hint is enabled", "[unit][input][sdl3][touch]")
{
    SDLInputEnvironment env;
    fcn::sdl3::Input input;

    SDL_SetHint(SDL_HINT_TOUCH_MOUSE_EVENTS, "1");

    input.pushInput(makeFinger(SDL_EVENT_FINGER_DOWN, 5, 50, 0.5F, 0.5F, 0.0F, 0.0F, env.mWindowId));
    CHECK(input.isMouseQueueEmpty());

    input.pushInput(makeMouseMotion(12.0F, 24.0F, env.mWindowId));

    std::vector<fcn::MouseInput> const events = drain(input);
    REQUIRE(events.size() == 1U);
    CHECK(events.at(0).getType() == fcn::MouseInput::Type::Moved);
    CHECK(events.at(0).getX() == 12);
    CHECK(events.at(0).getY() == 24);
}

TEST_CASE("SDL3 input uses wheel event coordinates for the mouse position", "[unit][input][sdl3][wheel]")
{
    SDLInputEnvironment env;
    fcn::sdl3::Input input;

    SDL_Event event{};
    event.type            = SDL_EVENT_MOUSE_WHEEL;
    event.wheel.x         = 0.0F;
    event.wheel.y         = 1.0F;
    event.wheel.mouse_x   = 12.0F;
    event.wheel.mouse_y   = 34.0F;
    event.wheel.windowID  = env.mWindowId;
    event.wheel.direction = SDL_MOUSEWHEEL_NORMAL;

    input.pushInput(event);

    std::vector<fcn::MouseInput> const events = drain(input);
    REQUIRE(events.size() == 1U);
    CHECK(events.at(0).getType() == fcn::MouseInput::Type::WheelMovedUp);
    CHECK(events.at(0).getX() == 12);
    CHECK(events.at(0).getY() == 34);
}
