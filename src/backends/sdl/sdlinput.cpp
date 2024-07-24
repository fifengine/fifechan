// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#include "fifechan/backends/sdl/sdlinput.hpp"

#include <string>
#include <vector>

#include "fifechan/exception.hpp"
#include "fifechan/util/utf8/utf8.hpp"

namespace fcn
{
    SDLInput::SDLInput()
    {
        mMouseInWindow = true;
        mMouseDown     = false;
    }

    bool SDLInput::isKeyQueueEmpty()
    {
        return mKeyInputQueue.empty();
    }

    KeyInput SDLInput::dequeueKeyInput()
    {
        KeyInput keyInput;

        if (mKeyInputQueue.empty()) {
            fcn::throwException("The queue is empty.", static_cast<char const *>(__FUNCTION__), __FILE__, __LINE__);
        }

        keyInput = mKeyInputQueue.front();
        mKeyInputQueue.pop();

        return keyInput;
    }

    bool SDLInput::isMouseQueueEmpty()
    {
        return mMouseInputQueue.empty();
    }

    MouseInput SDLInput::dequeueMouseInput()
    {
        MouseInput mouseInput;

        if (mMouseInputQueue.empty()) {
            fcn::throwException("The queue is empty.", static_cast<char const *>(__FUNCTION__), __FILE__, __LINE__);
        }

        mouseInput = mMouseInputQueue.front();
        mMouseInputQueue.pop();

        return mouseInput;
    }

    void SDLInput::pushInput(SDL_Event event)
    {
        KeyInput keyInput;
        MouseInput mouseInput;

        switch (event.type) {
        case SDL_KEYDOWN: {
            int value = convertSDLEventToFifechanKeyValue(event);

            keyInput.setKey(Key(value));
            keyInput.setType(KeyInput::Pressed);
            keyInput.setShiftPressed(event.key.keysym.mod & KMOD_SHIFT);
            keyInput.setControlPressed(event.key.keysym.mod & KMOD_CTRL);
            keyInput.setAltPressed(event.key.keysym.mod & KMOD_ALT);
            keyInput.setMetaPressed(event.key.keysym.mod & KMOD_GUI);
            keyInput.setNumericPad(event.key.keysym.mod & KMOD_NUM);
            mKeyInputQueue.push(keyInput);
            break;
        }

        case SDL_KEYUP: {
            int value = convertSDLEventToFifechanKeyValue(event);

            if (value == -1) {
                value = static_cast<int>(event.key.keysym.sym);
            }

            keyInput.setKey(Key(value));
            keyInput.setType(KeyInput::Released);
            keyInput.setShiftPressed(event.key.keysym.mod & KMOD_SHIFT);
            keyInput.setControlPressed(event.key.keysym.mod & KMOD_CTRL);
            keyInput.setAltPressed(event.key.keysym.mod & KMOD_ALT);
            keyInput.setMetaPressed(event.key.keysym.mod & KMOD_GUI);
            keyInput.setNumericPad(event.key.keysym.mod & KMOD_NUM);
            mKeyInputQueue.push(keyInput);
            break;
        }

        case SDL_MOUSEBUTTONDOWN: {
            mMouseDown = true;
            mouseInput.setX(event.button.x);
            mouseInput.setY(event.button.y);
            mouseInput.setButton(convertMouseButton(event.button.button));
            mouseInput.setType(MouseInput::Pressed);
            mouseInput.setTimeStamp(SDL_GetTicks());
            mMouseInputQueue.push(mouseInput);
            break;
        }

        case SDL_MOUSEBUTTONUP: {
            mMouseDown = false;
            mouseInput.setX(event.button.x);
            mouseInput.setY(event.button.y);
            mouseInput.setButton(convertMouseButton(event.button.button));
            mouseInput.setType(MouseInput::Released);
            mouseInput.setTimeStamp(SDL_GetTicks());
            mMouseInputQueue.push(mouseInput);
            break;
        }

        case SDL_MOUSEMOTION: {
            mouseInput.setX(event.motion.x);
            mouseInput.setY(event.motion.y);
            mouseInput.setButton(MouseInput::Empty);
            mouseInput.setType(MouseInput::Moved);
            mouseInput.setTimeStamp(SDL_GetTicks());
            mMouseInputQueue.push(mouseInput);
            break;
        }

        case SDL_MOUSEWHEEL: {
            /*if (event.wheel.y > 0 || (event.wheel.direction == SDL_MOUSEWHEEL_FLIPPED && event.wheel.y < 0)) {
                mouseInput.setType(MouseInput::WheelMovedUp);
            } else if (event.wheel.y < 0 || (event.wheel.direction == SDL_MOUSEWHEEL_FLIPPED && event.wheel.y > 0)) {
                mouseInput.setType(MouseInput::WheelMovedDown);
            }*/
            /*if (event.wheel.x > 0 || (event.wheel.direction == SDL_MOUSEWHEEL_FLIPPED && event.wheel.x < 0)) {
                mouseInput.setType(MouseInput::WheelMovedRight);
            } else if (event.wheel.x < 0 || (event.wheel.direction == SDL_MOUSEWHEEL_FLIPPED && event.wheel.x > 0)) {
                mouseInput.setType(MouseInput::WheelMovedLeft);
            }*/
            if (event.wheel.y > 0) {
                mouseInput.setType(MouseInput::WheelMovedUp);
            } else if (event.wheel.y < 0) {
                mouseInput.setType(MouseInput::WheelMovedDown);
            }
            if (event.wheel.x > 0) {
                mouseInput.setType(MouseInput::WheelMovedRight);
            } else if (event.wheel.x < 0) {
                mouseInput.setType(MouseInput::WheelMovedLeft);
            }
            int x, y;
            SDL_GetMouseState(&x, &y);
            mouseInput.setX(x);
            mouseInput.setY(y);
            mouseInput.setButton(convertMouseButton(SDL_BUTTON_LEFT));
            mouseInput.setTimeStamp(SDL_GetTicks());
            mMouseInputQueue.push(mouseInput);
            break;
        }

        // case SDL_TEXTEDITING:
        case SDL_TEXTINPUT: {
            std::string text(event.text.text);
            if (!text.empty()) {
                // hack to transport text
                std::vector<char16_t> result;
                utf8::utf8to16(text.begin(), text.end(), std::back_inserter(result));
                int value = result[0];

                keyInput.setKey(Key(value));
                keyInput.setType(KeyInput::Pressed);
                keyInput.setShiftPressed(event.key.keysym.mod & KMOD_SHIFT);
                keyInput.setControlPressed(event.key.keysym.mod & KMOD_CTRL);
                keyInput.setAltPressed(event.key.keysym.mod & KMOD_ALT);
                keyInput.setMetaPressed(event.key.keysym.mod & KMOD_GUI);
                keyInput.setNumericPad(event.key.keysym.mod & KMOD_NUM);
                mKeyInputQueue.push(keyInput);
            }
            break;
        }

        case SDL_WINDOWEVENT: {
            /*
             * This occurs when the mouse enters/leaves the window
             * and the gui application gains/loses its mousefocus.
             */
            if (event.window.event == SDL_WINDOWEVENT_ENTER) {
                mMouseInWindow = true;
            } else if (event.window.event == SDL_WINDOWEVENT_LEAVE) {
                mMouseInWindow = false;
                // Why???
                /*if (!mMouseDown) {
                    mouseInput.setX(-1);
                    mouseInput.setY(-1);
                    mouseInput.setButton(MouseInput::Empty);
                    mouseInput.setType(MouseInput::Moved);
                    mMouseInputQueue.push(mouseInput);
                }*/
            }
            break;
        }

        } // end switch
    }

    int SDLInput::convertMouseButton(int button)
    {
        switch (button) {
        case SDL_BUTTON_LEFT:
            return MouseInput::Left;
            break;
        case SDL_BUTTON_RIGHT:
            return MouseInput::Right;
            break;
        case SDL_BUTTON_MIDDLE:
            return MouseInput::Middle;
            break;
        case SDL_BUTTON_X1:
            return MouseInput::X1;
            break;
        case SDL_BUTTON_X2:
            return MouseInput::X2;
            break;
        default:
            // We have an unknown mouse type which is ignored.
            return button;
        }
    }

    int SDLInput::convertSDLEventToFifechanKeyValue(SDL_Event event)
    {
        int value = -1;

        switch (event.key.keysym.sym) {
        case SDLK_TAB:
            value = Key::Tab;
            break;
        case SDLK_LALT:
            value = Key::LeftAlt;
            break;
        case SDLK_RALT:
            value = Key::RightAlt;
            break;
        case SDLK_LSHIFT:
            value = Key::LeftShift;
            break;
        case SDLK_RSHIFT:
            value = Key::RightShift;
            break;
        case SDLK_LCTRL:
            value = Key::LeftControl;
            break;
        case SDLK_RCTRL:
            value = Key::RightControl;
            break;
        case SDLK_BACKSPACE:
            value = Key::Backspace;
            break;
        case SDLK_PAUSE:
            value = Key::Pause;
            break;
        case SDLK_SPACE:
            // Special characters like ~ (tilde) ends up
            // with the keysym.sym SDLK_SPACE which
            // without this check would be lost. The check
            // is only valid on key up events in SDL.
            // if (event.type == SDL_KEYUP || event.key.keysym.unicode == ' ')
            if (event.type == SDL_KEYUP) {
                value = Key::Space;
            }
            break;
        case SDLK_ESCAPE:
            value = Key::Escape;
            break;
        case SDLK_DELETE:
            value = Key::Delete;
            break;
        case SDLK_INSERT:
            value = Key::Insert;
            break;
        case SDLK_HOME:
            value = Key::Home;
            break;
        case SDLK_END:
            value = Key::End;
            break;
        case SDLK_PAGEUP:
            value = Key::PageUp;
            break;
        case SDLK_PRINTSCREEN:
            value = Key::PrintScreen;
            break;
        case SDLK_PAGEDOWN:
            value = Key::PageDown;
            break;
        case SDLK_F1:
            value = Key::F1;
            break;
        case SDLK_F2:
            value = Key::F2;
            break;
        case SDLK_F3:
            value = Key::F3;
            break;
        case SDLK_F4:
            value = Key::F4;
            break;
        case SDLK_F5:
            value = Key::F5;
            break;
        case SDLK_F6:
            value = Key::F6;
            break;
        case SDLK_F7:
            value = Key::F7;
            break;
        case SDLK_F8:
            value = Key::F8;
            break;
        case SDLK_F9:
            value = Key::F9;
            break;
        case SDLK_F10:
            value = Key::F10;
            break;
        case SDLK_F11:
            value = Key::F11;
            break;
        case SDLK_F12:
            value = Key::F12;
            break;
        case SDLK_F13:
            value = Key::F13;
            break;
        case SDLK_F14:
            value = Key::F14;
            break;
        case SDLK_F15:
            value = Key::F15;
            break;
        case SDLK_NUMLOCKCLEAR:
            value = Key::NumLock;
            break;
        case SDLK_CAPSLOCK:
            value = Key::CapsLock;
            break;
        case SDLK_SCROLLLOCK:
            value = Key::ScrollLock;
            break;
        case SDLK_LGUI:
            value = Key::LeftSuper;
            break;
        case SDLK_RGUI:
            value = Key::RightSuper;
            break;
        case SDLK_MODE:
            value = Key::AltGr;
            break;
        case SDLK_UP:
            value = Key::Up;
            break;
        case SDLK_DOWN:
            value = Key::Down;
            break;
        case SDLK_LEFT:
            value = Key::Left;
            break;
        case SDLK_RIGHT:
            value = Key::Right;
            break;
        case SDLK_RETURN:
            value = Key::Enter;
            break;
        case SDLK_KP_ENTER:
            value = Key::Enter;
            break;

        default:
            break;
        }

        if (!(event.key.keysym.mod & KMOD_NUM)) {
            switch (event.key.keysym.sym) {
            case SDLK_KP_0:
                value = Key::Insert;
                break;
            case SDLK_KP_1:
                value = Key::End;
                break;
            case SDLK_KP_2:
                value = Key::Down;
                break;
            case SDLK_KP_3:
                value = Key::PageDown;
                break;
            case SDLK_KP_4:
                value = Key::Left;
                break;
            case SDLK_KP_5:
                value = 0;
                break;
            case SDLK_KP_6:
                value = Key::Right;
                break;
            case SDLK_KP_7:
                value = Key::Home;
                break;
            case SDLK_KP_8:
                value = Key::Up;
                break;
            case SDLK_KP_9:
                value = Key::PageUp;
                break;
            default:
                break;
            }
        }

        return value;
    }
} // namespace fcn
