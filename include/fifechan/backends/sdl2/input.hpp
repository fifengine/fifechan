// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2026 Fifengine contributors

#ifndef INCLUDE_FIFECHAN_BACKENDS_SDL_SDLINPUT_HPP_
#define INCLUDE_FIFECHAN_BACKENDS_SDL_SDLINPUT_HPP_

#include <SDL2/SDL.h>

#include <queue>

#include "fifechan/input.hpp"
#include "fifechan/keyinput.hpp"
#include "fifechan/mouseinput.hpp"
#include "fifechan/platform.hpp"

namespace fcn
{
    class Key;
}

namespace fcn::sdl2
{

    /**
     * SDL2-specific implementation of Input.
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
        void _pollInput() override { }

        // Inherited from Input

        bool isKeyQueueEmpty() override;

        KeyInput dequeueKeyInput() override;

        bool isMouseQueueEmpty() override;

        MouseInput dequeueMouseInput() override;

    protected:
        /**
         * Converts a mouse button from SDL to a FifeGUI mouse button.
         *
         * @param button an SDL mouse button.
         * @return a FifeGUI mouse button.
         */
        MouseInput::Button convertMouseButton(int button);

        /**
         * Converts an SDL event to a FifeGUI key value.
         *
         * @param event The SDL event to convert.
         * @return A FifeGUI key value. -1 if no conversion took place.
         * @see Key
         */
        int convertSDLEventToFifechanKeyValue(SDL_Event event);

        /** Queue of key inputs waiting to be processed. */
        std::queue<KeyInput> mKeyInputQueue;

        /** Queue of mouse inputs waiting to be processed. */
        std::queue<MouseInput> mMouseInputQueue;

        /** True if a mouse button is currently held down. */
        bool mMouseDown{false};

        /** True if the mouse cursor is currently within the application window. */
        bool mMouseInWindow{false};
    };
} // namespace fcn::sdl2

#endif // INCLUDE_FIFECHAN_BACKENDS_SDL_SDLINPUT_HPP_
