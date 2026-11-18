// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2026 Fifengine contributors

// Corresponding header include
#include "fifechan/backends/sdl3/input.hpp"

// Standard library includes
#include <algorithm>
#include <cmath>
#include <string>
#include <utility>
#include <vector>

// Third-party library includes
#include <utf8cpp/utf8.h>

// Project headers (subdirs before local)
#include "fifechan/exception.hpp"

namespace fcn::sdl3
{
    namespace
    {
        /**
         * A pan has to move this far (normalized window units) before a
         * wheel tick is emitted. Fifechan scrolls a fraction of the content
         * per wheel event, so a smaller step would scroll far too fast.
         */
        constexpr float TOUCH_SCROLL_STEP = 0.08F;

        /**
         * Horizontal pans below this threshold are ignored. Vertical pans
         * are never suppressed.
         */
        constexpr float TOUCH_SCROLL_DEAD_ZONE = 0.01F;

        /** Upper bound on wheel events produced by one finger motion. */
        constexpr int MAX_WHEEL_EVENTS_PER_UPDATE = 8;

        /**
         * Native touch is only safe when SDL is not also synthesizing mouse
         * events from the same touches.
         */
        bool nativeTouchEnabled()
        {
            return !SDL_GetHintBoolean(SDL_HINT_TOUCH_MOUSE_EVENTS, true);
        }

        /** SDL synthesizes finger events from the mouse for this touch id. */
        bool isSyntheticFinger(SDL_TouchFingerEvent const & finger)
        {
            return finger.touchID == SDL_MOUSE_TOUCHID;
        }

        /** Trackpads are indirect devices and normally emit mouse events. */
        bool isTrackpad(SDL_TouchID touchId)
        {
            SDL_TouchDeviceType const type = SDL_GetTouchDeviceType(touchId);
            return (type == SDL_TOUCH_DEVICE_INDIRECT_ABSOLUTE) || (type == SDL_TOUCH_DEVICE_INDIRECT_RELATIVE);
        }

        /** Converts one normalized coordinate to a window point. */
        int windowCoordinate(float normalized, int length)
        {
            if (length <= 0) {
                return 0;
            }

            // SDL may report values outside 0..1 for touches that leave the
            // window. Truncating the normalized value would always yield 0.
            float const clamped = std::clamp(normalized, 0.0F, 1.0F);
            return static_cast<int>(std::lround(static_cast<double>(clamped) * static_cast<double>(length)));
        }

        /** Resolves a normalized touch position for a window id. */
        bool windowPoint(SDL_WindowID windowId, float normalizedX, float normalizedY, int& x, int& y)
        {
            SDL_Window* const window = SDL_GetWindowFromID(windowId);
            if (window == nullptr) {
                return false;
            }

            int width  = 0;
            int height = 0;
            if (!SDL_GetWindowSize(window, &width, &height)) {
                return false;
            }

            x = windowCoordinate(normalizedX, width);
            y = windowCoordinate(normalizedY, height);
            return true;
        }
    } // namespace

    Input::Input(SDL_Window* window) : mWindow(window), mMouseInWindow(true)
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

    void Input::startTextInput()
    {
        if (mWindow == nullptr || SDL_TextInputActive(mWindow)) {
            return;
        }

        // Called from Gui::logic(); never throw on failure.
        (void)SDL_StartTextInput(mWindow);
    }

    void Input::stopTextInput()
    {
        if (mWindow == nullptr || !SDL_TextInputActive(mWindow)) {
            return;
        }

        // Called from Gui::logic() and ~Gui(); never throw on failure.
        (void)SDL_StopTextInput(mWindow);
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
            mouseInput.setTimeStamp(static_cast<int>(SDL_GetTicks()));
            mMouseInputQueue.push(mouseInput);
            break;
        }

        case SDL_EVENT_MOUSE_BUTTON_UP: {
            mMouseDown = false;
            mouseInput.setX(static_cast<int>(event.button.x));
            mouseInput.setY(static_cast<int>(event.button.y));
            mouseInput.setButton(convertMouseButton(event.button.button));
            mouseInput.setType(MouseInput::Type::Released);
            mouseInput.setTimeStamp(static_cast<int>(SDL_GetTicks()));
            mMouseInputQueue.push(mouseInput);
            break;
        }

        case SDL_EVENT_MOUSE_MOTION: {
            mouseInput.setX(static_cast<int>(event.motion.x));
            mouseInput.setY(static_cast<int>(event.motion.y));
            mouseInput.setButton(MouseInput::Button::Empty);
            mouseInput.setType(MouseInput::Type::Moved);
            mouseInput.setTimeStamp(static_cast<int>(SDL_GetTicks()));
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
            // Use the coordinates carried by the event. SDL_GetMouseState()
            // returns the last mouse position, which is wrong for wheel
            // events synthesized from touch or remote input.
            mouseInput.setX(static_cast<int>(event.wheel.mouse_x));
            mouseInput.setY(static_cast<int>(event.wheel.mouse_y));
            mouseInput.setButton(convertMouseButton(SDL_BUTTON_LEFT));
            mouseInput.setTimeStamp(static_cast<int>(SDL_GetTicks()));
            mMouseInputQueue.push(mouseInput);
            break;
        }

        case SDL_EVENT_FINGER_DOWN:
        case SDL_EVENT_FINGER_MOTION:
        case SDL_EVENT_FINGER_UP:
        case SDL_EVENT_FINGER_CANCELED: {
            if (nativeTouchEnabled()) {
                pushTouchInput(event);
            }
            break;
        }

        // case SDL_EVENT_TEXT_EDITING:
        case SDL_EVENT_TEXT_INPUT: {
            std::string text(event.text.text);
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

    void Input::pushTouchInput(SDL_Event const & event)
    {
        SDL_TouchFingerEvent const & finger = event.tfinger;

        // SDL synthesizes touch from the mouse when SDL_HINT_MOUSE_TOUCH_EVENTS
        // is enabled. That is a feedback path, not new input.
        if (isSyntheticFinger(finger)) {
            return;
        }

        // Trackpads normally emit mouse events. Ignoring their finger stream
        // prevents a gesture from being applied twice.
        if (isTrackpad(finger.touchID)) {
            return;
        }

        int x               = 0;
        int y               = 0;
        bool const hasPoint = windowPoint(finger.windowID, finger.x, finger.y, x, y);

        TouchFingerKey const key{finger.touchID, finger.fingerID};

        switch (event.type) {
        case SDL_EVENT_FINGER_DOWN: {
            bool const wasEmpty = mTouchFingers.empty();
            mTouchFingers.insert(key);
            mTouchScrollX = 0.0F;
            mTouchScrollY = 0.0F;

            if (wasEmpty && !mTouchPointerDown && hasPoint) {
                mTouchPointerDown     = true;
                mTouchPointerTouchId  = finger.touchID;
                mTouchPointerFingerId = finger.fingerID;
                mTouchPointerX        = x;
                mTouchPointerY        = y;
                pushMouseInput(MouseInput::Type::Pressed, MouseInput::Button::Left, x, y);
            } else if (mTouchPointerDown) {
                // A second finger starts a pan. End the synthetic pointer
                // without turning the gesture into a click.
                cancelTouchPointer();
            }
            break;
        }

        case SDL_EVENT_FINGER_MOTION: {
            // Recover from a missed FINGER_DOWN instead of dropping the
            // finger forever.
            mTouchFingers.insert(key);

            if (mTouchFingers.size() >= 2) {
                if (hasPoint) {
                    pushTouchScroll(finger.dx, finger.dy, x, y);
                }
                break;
            }

            if (mTouchPointerDown && hasPoint && (finger.touchID == mTouchPointerTouchId) &&
                (finger.fingerID == mTouchPointerFingerId)) {
                mTouchPointerX = x;
                mTouchPointerY = y;
                pushMouseInput(MouseInput::Type::Moved, MouseInput::Button::Left, x, y);
            }
            break;
        }

        case SDL_EVENT_FINGER_UP:
        case SDL_EVENT_FINGER_CANCELED: {
            bool const canceled   = (event.type == SDL_EVENT_FINGER_CANCELED);
            bool const wasPointer = mTouchPointerDown && (finger.touchID == mTouchPointerTouchId) &&
                                    (finger.fingerID == mTouchPointerFingerId);

            mTouchFingers.erase(key);

            if (wasPointer) {
                if (canceled || !hasPoint) {
                    cancelTouchPointer();
                } else {
                    mTouchPointerDown = false;
                    mTouchPointerX    = x;
                    mTouchPointerY    = y;
                    pushMouseInput(MouseInput::Type::Released, MouseInput::Button::Left, x, y);
                }
            }

            if (mTouchFingers.empty()) {
                resetTouchTracking();
            }
            break;
        }

        default:
            break;
        }
    }

    void Input::pushMouseInput(MouseInput::Type type, MouseInput::Button button, int x, int y)
    {
        MouseInput mouseInput{};
        mouseInput.setX(x);
        mouseInput.setY(y);
        mouseInput.setButton(button);
        mouseInput.setType(type);
        mouseInput.setTimeStamp(static_cast<int>(SDL_GetTicks()));
        mMouseInputQueue.push(mouseInput);
    }

    void Input::cancelTouchPointer()
    {
        if (!mTouchPointerDown) {
            return;
        }

        mTouchPointerDown = false;
        pushMouseInput(MouseInput::Type::Canceled, MouseInput::Button::Left, mTouchPointerX, mTouchPointerY);
    }

    void Input::resetTouchTracking()
    {
        mTouchFingers.clear();
        mTouchScrollX     = 0.0F;
        mTouchScrollY     = 0.0F;
        mTouchPointerDown = false;
    }

    void Input::pushTouchScroll(float deltaX, float deltaY, int x, int y)
    {
        mTouchScrollX += deltaX;
        mTouchScrollY += deltaY;

        float const absX = std::abs(mTouchScrollX);
        float const absY = std::abs(mTouchScrollY);

        // Vertical wins ties: a small diagonal jitter must not turn an
        // intentional vertical pan into a horizontal wheel callback.
        if ((absY > 0.0F) && (absY >= absX)) {
            mTouchScrollX = 0.0F;

            int emitted = 0;
            while ((std::abs(mTouchScrollY) >= TOUCH_SCROLL_STEP) && (emitted < MAX_WHEEL_EVENTS_PER_UPDATE)) {
                bool const down = (mTouchScrollY > 0.0F);
                mTouchScrollY += down ? -TOUCH_SCROLL_STEP : TOUCH_SCROLL_STEP;
                pushMouseInput(
                    down ? MouseInput::Type::WheelMovedDown : MouseInput::Type::WheelMovedUp,
                    MouseInput::Button::Empty,
                    x,
                    y);
                ++emitted;
            }
            return;
        }

        if (absX >= TOUCH_SCROLL_DEAD_ZONE) {
            mTouchScrollY = 0.0F;

            int emitted = 0;
            while ((std::abs(mTouchScrollX) >= TOUCH_SCROLL_STEP) && (emitted < MAX_WHEEL_EVENTS_PER_UPDATE)) {
                bool const right = (mTouchScrollX > 0.0F);
                mTouchScrollX += right ? -TOUCH_SCROLL_STEP : TOUCH_SCROLL_STEP;
                pushMouseInput(
                    right ? MouseInput::Type::WheelMovedRight : MouseInput::Type::WheelMovedLeft,
                    MouseInput::Button::Empty,
                    x,
                    y);
                ++emitted;
            }
        }
    }

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
