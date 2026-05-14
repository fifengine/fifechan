// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2026 Fifengine contributors

// Corresponding header include
#include "fifechan/backends/sdl3/input.hpp"

// Standard library includes
#include <string>
#include <utility>
#include <vector>

// Third-party library includes
#include <utf8cpp/utf8.h>

// Project headers (subdirs before local)
#include "fifechan/exception.hpp"

namespace fcn::sdl3
{
    Input::Input() : mMouseInWindow(true)
    {
    }

    bool Input::isKeyQueueEmpty()
    {
        return mKeyInputQueue.empty();
    }

    KeyInput Input::dequeueKeyInput()
    {
        KeyInput keyInput;

        if (mKeyInputQueue.empty()) {
            throwException("The queue is empty.");
        }

        keyInput = mKeyInputQueue.front();
        mKeyInputQueue.pop();

        return keyInput;
    }

    bool Input::isMouseQueueEmpty()
    {
        return mMouseInputQueue.empty();
    }

    MouseInput Input::dequeueMouseInput()
    {
        MouseInput mouseInput{};

        if (mMouseInputQueue.empty()) {
            throwException("The queue is empty.");
        }

        mouseInput = mMouseInputQueue.front();
        mMouseInputQueue.pop();

        return mouseInput;
    }

    bool Input::isTextQueueEmpty()
    {
        return mTextInputQueue.empty();
    }

    std::string Input::dequeueTextInput()
    {
        if (mTextInputQueue.empty()) {
            return {};
        }
        std::string text = mTextInputQueue.front();
        mTextInputQueue.pop();
        return text;
    }

    void Input::pushInput(SDL_Event event)
    {
        KeyInput keyInput{};
        MouseInput mouseInput{};

        switch (event.type) {
        case SDL_EVENT_KEY_DOWN:
        case SDL_EVENT_KEY_UP: {
            // KeyType mirrors SDL_Keycode 1:1 use the SDL keycode directly.
            keyInput.setKey(Key(static_cast<int>(event.key.key)));
            keyInput.setType(event.type == SDL_EVENT_KEY_DOWN ? KeyInput::Type::Pressed : KeyInput::Type::Released);
            keyInput.setShiftPressed((event.key.mod & SDL_KMOD_SHIFT) != 0);
            keyInput.setControlPressed((event.key.mod & SDL_KMOD_CTRL) != 0);
            keyInput.setAltPressed((event.key.mod & SDL_KMOD_ALT) != 0);
            keyInput.setMetaPressed((event.key.mod & SDL_KMOD_GUI) != 0);
            keyInput.setNumericPad((event.key.mod & SDL_KMOD_NUM) != 0);
            mKeyInputQueue.push(keyInput);
            break;
        }

        case SDL_EVENT_MOUSE_BUTTON_DOWN: {
            mMouseDown = true;
            mouseInput.setX(static_cast<int>(event.button.x));
            mouseInput.setY(static_cast<int>(event.button.y));
            mouseInput.setButton(convertMouseButton(event.button.button));
            mouseInput.setType(MouseInput::Type::Pressed);
            mouseInput.setTimeStamp(SDL_GetTicks());
            mMouseInputQueue.push(mouseInput);
            break;
        }

        case SDL_EVENT_MOUSE_BUTTON_UP: {
            mMouseDown = false;
            mouseInput.setX(static_cast<int>(event.button.x));
            mouseInput.setY(static_cast<int>(event.button.y));
            mouseInput.setButton(convertMouseButton(event.button.button));
            mouseInput.setType(MouseInput::Type::Released);
            mouseInput.setTimeStamp(SDL_GetTicks());
            mMouseInputQueue.push(mouseInput);
            break;
        }

        case SDL_EVENT_MOUSE_MOTION: {
            mouseInput.setX(static_cast<int>(event.motion.x));
            mouseInput.setY(static_cast<int>(event.motion.y));
            mouseInput.setButton(MouseInput::Button::Empty);
            mouseInput.setType(MouseInput::Type::Moved);
            mouseInput.setTimeStamp(SDL_GetTicks());
            mMouseInputQueue.push(mouseInput);
            break;
        }

        case SDL_EVENT_MOUSE_WHEEL: {
            if (event.wheel.y > 0) {
                mouseInput.setType(MouseInput::Type::WheelMovedUp);
            } else if (event.wheel.y < 0) {
                mouseInput.setType(MouseInput::Type::WheelMovedDown);
            }
            if (event.wheel.x > 0) {
                mouseInput.setType(MouseInput::Type::WheelMovedRight);
            } else if (event.wheel.x < 0) {
                mouseInput.setType(MouseInput::Type::WheelMovedLeft);
            }
            float x = 0;
            float y = 0;
            SDL_GetMouseState(&x, &y);
            mouseInput.setX(static_cast<int>(x));
            mouseInput.setY(static_cast<int>(y));
            mouseInput.setButton(convertMouseButton(SDL_BUTTON_LEFT));
            mouseInput.setTimeStamp(SDL_GetTicks());
            mMouseInputQueue.push(mouseInput);
            break;
        }

        // case SDL_EVENT_TEXT_EDITING:
        case SDL_EVENT_TEXT_INPUT: {
            std::string text(static_cast<char const *>(event.text.text));
            if (!text.empty()) {
                mTextInputQueue.push(std::move(text));
            }
            break;
        }

        case SDL_EVENT_WINDOW_MOUSE_ENTER: {
            mMouseInWindow = true;
            break;
        }
        case SDL_EVENT_WINDOW_MOUSE_LEAVE: {
            mMouseInWindow = false;
            break;
        }

        default:
            break;
        }

    } // end switch

    MouseInput::Button Input::convertMouseButton(int button)
    {
        switch (button) {
        case SDL_BUTTON_LEFT:
            return MouseInput::Button::Left;
            break;
        case SDL_BUTTON_RIGHT:
            return MouseInput::Button::Right;
            break;
        case SDL_BUTTON_MIDDLE:
            return MouseInput::Button::Middle;
            break;
        case SDL_BUTTON_X1:
            return MouseInput::Button::X1;
            break;
        case SDL_BUTTON_X2:
            return MouseInput::Button::X2;
            break;
        default:
            return MouseInput::Button::Empty;
        }
    }

} // namespace fcn::sdl3
