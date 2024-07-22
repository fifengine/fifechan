// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#ifndef FCN_SDLINPUT_HPP
#define FCN_SDLINPUT_HPP

#include <SDL2/SDL.h>

#include <queue>

#include "fifechan/input.hpp"
#include "fifechan/keyinput.hpp"
#include "fifechan/mouseinput.hpp"
#include "fifechan/platform.hpp"

namespace fcn
{
    class Key;

    /**
     * SDL implementation of Input.
     */
    class FIFEGUI_EXT_API SDLInput : public Input
    {
    public:
        /**
         * Constructor.
         */
        SDLInput();

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
        virtual void _pollInput() { }

        // Inherited from Input

        virtual bool isKeyQueueEmpty();

        virtual KeyInput dequeueKeyInput();

        virtual bool isMouseQueueEmpty();

        virtual MouseInput dequeueMouseInput();

    protected:
        /**
         * Converts a mouse button from SDL to a Fifechan mouse button
         * representation.
         *
         * @param button an SDL mouse button.
         * @return a Fifechan mouse button.
         */
        int convertMouseButton(int button);

        /**
         * Converts an SDL event to a Fifechan key value.
         *
         * @param keysym The SDL event to convert.
         * @return A Fifechan key value. -1 if no conversion took place.
         * @see Key
         */
        int convertSDLEventToFifechanKeyValue(SDL_Event event);

        std::queue<KeyInput> mKeyInputQueue;
        std::queue<MouseInput> mMouseInputQueue;

        bool mMouseDown;
        bool mMouseInWindow;
    };
} // namespace fcn

#endif // end FCN_SDLINPUT_HPP
