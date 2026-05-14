// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2026 Fifengine contributors

#ifndef INCLUDE_FIFECHAN_BACKENDS_SDL_SDLINPUT_HPP_
#define INCLUDE_FIFECHAN_BACKENDS_SDL_SDLINPUT_HPP_

// Standard library includes
#include <queue>
#include <string>

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
