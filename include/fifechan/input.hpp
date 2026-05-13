// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2026 Fifengine contributors

#ifndef INCLUDE_FIFECHAN_INPUT_HPP_
#define INCLUDE_FIFECHAN_INPUT_HPP_

// Standard library includes
#include <cstdint>
#include <string>

// Platform config include
#include "fifechan/platform.hpp"

namespace fcn
{
    class KeyInput;
    class MouseInput;

    /**
     * Abstract interface for polling user input devices.
     *
     * FifeGUI contains implementations of Input for common
     * libraries like SDL.
     * To make FifeGUI usable with other libraries, an Input
     * class must be implemented.
     *
     * @note
     * Functions beginning with underscore "_" should not be overloaded
     * unless you know what you are doing.
     *
     * @see SDLInput
     *
     * @ingroup input
     */
    class FIFEGUI_API Input
    {
        public:
            virtual ~Input() = default;

            Input(Input const &)            = delete;
            Input& operator=(Input const &) = delete;
            Input(Input&&)                  = delete;
            Input& operator=(Input&&)       = delete;

            /**
             * Checks if the key queue is empty, or not.
             *
             * @return True if the key queue is empty,
             *         false otherwise.
             */
            virtual bool isKeyQueueEmpty() = 0;

            /**
             * Dequeues the key input queue.
             *
             * @return The first key input in the key input queue.
             */
            virtual KeyInput dequeueKeyInput() = 0;

            /**
             * Checks if the mouse queue is empyt, or not.
             *
             * @return True if the mouse queue is empty,
             *         false otherwise.
             */
            virtual bool isMouseQueueEmpty() = 0;

            /**
             * Dequeues the mouse input queue.
             *
             * @return The first mouse input in the mouse input queue.
             */
            virtual MouseInput dequeueMouseInput() = 0;

            /**
             * Checks if the text input queue is empty.
             *
             * Base implementation returns true (no text queue).
             * Override in backends that support text input (e.g. SDL3).
             *
             * @return True if the text queue is empty, false otherwise.
             */
            virtual bool isTextQueueEmpty()
            {
                return true;
            }

            /**
             * Dequeues a UTF-8 text string from the text input queue.
             *
             * Base implementation returns an empty string.
             * Override in backends that support text input (e.g. SDL3).
             *
             * @return The UTF-8 text string.
             */
            virtual std::string dequeueTextInput()
            {
                return {};
            }

            /**
             * Polls all existing input.
             *
             * Called when input should be polled.
             *
             * @note
             * Provided for compatibility with backend libraries that require input
             * polling at a fixed update rate.
             */
            virtual void _pollInput() = 0;

        protected:
            Input() = default;
    };
} // namespace fcn

#endif // INCLUDE_FIFECHAN_INPUT_HPP_
