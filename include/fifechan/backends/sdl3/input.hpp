// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2026 Fifengine contributors

#ifndef INCLUDE_FIFECHAN_BACKENDS_SDL_SDLINPUT_HPP_
#define INCLUDE_FIFECHAN_BACKENDS_SDL_SDLINPUT_HPP_

// Standard library includes
#include <queue>
#include <set>
#include <string>
#include <utility>

// Platform config include
#include "fifechan/platform.hpp"

// Third-party library includes
#include <SDL3/SDL.h>

// Project headers (subdirs before local)
#include "fifechan/input.hpp"
#include "fifechan/keyinput.hpp"
#include "fifechan/mouseinput.hpp"

namespace fcn
{
    class Key;
}

namespace fcn::sdl3
{

    /**
     * SDL3-specific implementation of Input.
     *
     * @ingroup input
     */
    class FIFEGUI_EXT_API Input : public fcn::Input
    {
        public:
            /**
             * Constructor.
             */
            Input();

            /**
             * Pushes an SDL event. It should be called at least once per frame to
             * update input with user input.
             *
             * @param event an event from SDL.
             */
            virtual void pushInput(SDL_Event event);

            /**
             * Whether this backend translates raw SDL finger events itself.
             *
             * Native touch handling is active when
             * SDL_HINT_TOUCH_MOUSE_EVENTS is "0". With the default "1", SDL
             * synthesizes mouse events and finger events are ignored to avoid
             * processing the same gesture twice.
             *
             * @return true, because this backend can handle finger events.
             */
            static constexpr bool hasNativeTouchInput() noexcept
            {
                return true;
            }

            /**
             * Polls all input. It exists for input driver compatibility. If you
             * only use SDL and plan sticking with SDL you can safely ignore this
             * function as it in the SDL case does nothing.
             */
            void _pollInput() override
            {
            }

            // Inherited from Input

            bool isKeyQueueEmpty() override;

            KeyInput dequeueKeyInput() override;

            bool isMouseQueueEmpty() override;

            MouseInput dequeueMouseInput() override;

            bool isTextQueueEmpty() override;

            std::string dequeueTextInput() override;

        protected:
            /**
             * Converts a mouse button from SDL to a FifeGUI mouse button.
             *
             * @param button an SDL mouse button.
             * @return a FifeGUI mouse button.
             */
            static MouseInput::Button convertMouseButton(int button);

            /**
             * Translates one SDL finger event into queued mouse input.
             *
             * The first direct-touch finger behaves like the left mouse
             * button. A second finger switches the gesture to panning and
             * ends the synthetic pointer without generating a click.
             *
             * @param event an SDL_EVENT_FINGER_* event.
             */
            void pushTouchInput(SDL_Event const & event);

            /**
             * Adds mouse input to the queue using the current SDL timestamp.
             *
             * @param type the mouse input type.
             * @param button the mouse button.
             * @param x the mouse x coordinate.
             * @param y the mouse y coordinate.
             */
            void pushMouseInput(MouseInput::Type type, MouseInput::Button button, int x, int y);

            /**
             * Ends the synthetic touch pointer without generating a click.
             */
            void cancelTouchPointer();

            /**
             * Clears finger tracking and pending scroll deltas.
             */
            void resetTouchTracking();

            /**
             * Converts finger pan deltas into discrete wheel input.
             *
             * The dominant axis wins. Vertical pans are never suppressed;
             * small horizontal pans are ignored so diagonal jitter cannot
             * produce horizontal wheel callbacks.
             *
             * @param deltaX normalized horizontal delta.
             * @param deltaY normalized vertical delta.
             * @param x the current pointer x coordinate.
             * @param y the current pointer y coordinate.
             */
            void pushTouchScroll(float deltaX, float deltaY, int x, int y);

            /** Key for the active fingers of all touch devices. */
            using TouchFingerKey = std::pair<SDL_TouchID, SDL_FingerID>;

            /** Active fingers keyed by (touch id, finger id). */
            std::set<TouchFingerKey> mTouchFingers;

            /** Touch device and finger that own the synthetic pointer. */
            SDL_TouchID mTouchPointerTouchId{0};
            SDL_FingerID mTouchPointerFingerId{0};

            /** Last emitted synthetic pointer position, in window points. */
            int mTouchPointerX{0};
            int mTouchPointerY{0};

            /** Pending pan deltas, in normalized window units. */
            float mTouchScrollX{0.0F};
            float mTouchScrollY{0.0F};

            /** True while a synthetic pointer press is active. */
            bool mTouchPointerDown{false};

            /** Queue of key inputs waiting to be processed. */
            std::queue<KeyInput> mKeyInputQueue;

            /** Queue of mouse inputs waiting to be processed. */
            std::queue<MouseInput> mMouseInputQueue;

            /** Queue of text input strings waiting to be processed. */
            std::queue<std::string> mTextInputQueue;

            /** True if a mouse button is currently held down. */
            bool mMouseDown{false};

            /** True if the mouse cursor is currently within the application window. */
            bool mMouseInWindow{false};
    };
} // namespace fcn::sdl3

#endif // INCLUDE_FIFECHAN_BACKENDS_SDL_SDLINPUT_HPP_
