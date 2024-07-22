// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#ifndef FCN_INPUT_HPP
#define FCN_INPUT_HPP

#include "fifechan/platform.hpp"

namespace fcn
{
    class KeyInput;
    class MouseInput;

    /**
     * Abstract class for providing functions for user input.
     *
     * Fifechan contains implementations of Input for common
     * libraries like the Allegro library, the HGE library,
     * and the SDL library.
     * To make Fifechan usable with other libraries, an Input
     * class must be implemented.
     *
     * @see AllegroInput, HGEInput, OpenLayerInput,
     *      SDLInput
     */
    class FIFEGUI_API Input
    {
    public:
        /**
         * Destructor.
         */
        virtual ~Input() = default;

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
         * Polls all exsisting input. Called when input should
         * be polled. The function exists for compatibility reason
         * where some libraries need to poll input at a certain
         * logic rate.
         */
        virtual void _pollInput() = 0;
    };
} // namespace fcn

#endif // end FCN_INPUT_HPP
