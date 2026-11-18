// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2026 Fifengine contributors

#ifndef INCLUDE_FIFECHAN_GENERICINPUT_HPP_
#define INCLUDE_FIFECHAN_GENERICINPUT_HPP_

// Standard library includes
#include <queue>
#include <string>

// Project headers (subdirs before local)
#include "fifechan/input.hpp"
#include "fifechan/keyinput.hpp"
#include "fifechan/mouseinput.hpp"

namespace fcn
{
    class Key;

    /**
     * A generic input implementation usable with various backends.
     *
     * This input does not derive text from key presses. Integrators must push
     * printable text explicitly with pushTextInput(); otherwise text widgets
     * receive no input. This keeps the key/text distinction that IME
     * composition, dead keys and paste require.
     *
     * @ingroup input
     */
    class FIFEGUI_API GenericInput : public Input
    {
        public:
            GenericInput();

            /**
             * Pushes a UTF-8 text input event.
             *
             * Call this once per committed text event: printable keys, IME
             * composition results and pasted text. Empty strings are ignored.
             *
             * @param text The UTF-8 text that was input.
             */
            void pushTextInput(std::string text);

            /**
             * Pushes a key pressed event.
             *
             * @note If a special key is pressed, like the F1 key,
             *       the corresponding GUI key value found
             *       in the enum in Key should be pushed as the
             *       unicode value.
             *
             * @param unicode The unicode value of the key.
             */
            void pushKeyPressed(int unicode);

            /**
             * Pushes a key released event.
             *
             * @note If a special key is pressed, like the F1 key,
             *       the corresponding GUI key value found
             *       in the enum in Key should be pushed as the
             *       unicode value.
             *
             * @param unicode The unicode value of the key.
             */
            void pushKeyReleased(int unicode);

            /**
             * Pushes a mouse button pressed event.
             *
             * @param x The x coordinate of the mouse event.
             * @param y The y coordinate of the mouse event.
             * @param button The button of the mouse event.
             */
            void pushMouseButtonPressed(int x, int y, int button);

            /**
             * Pushes a mouse button released event.
             *
             * @param x The x coordinate of the mouse event.
             * @param y The y coordinate of the mouse event.
             * @param button The button of the mouse event.
             */
            void pushMouseButtonReleased(int x, int y, int button);

            /**
             * Pushes a mouse wheel moved up event.
             *
             * @param x The x coordinate of the mouse event.
             * @param y The y coordinate of the mouse event.
             */
            void pushMouseWheelMovedUp(int x, int y);

            /**
             * Pushes a mouse wheel moved down event.
             *
             * @param x The x coordinate of the mouse event.
             * @param y The y coordinate of the mouse event.
             */
            void pushMouseWheelMovedDown(int x, int y);

            /**
             * Pushes a mouse wheel moved right event.
             *
             * @param x The x coordinate of the mouse event.
             * @param y The y coordinate of the mouse event.
             */
            void pushMouseWheelMovedRight(int x, int y);

            /**
             * Pushes a mouse wheel moved left event.
             *
             * @param x The x coordinate of the mouse event.
             * @param y The y coordinate of the mouse event.
             */
            void pushMouseWheelMovedLeft(int x, int y);

            /**
             * Pushes a mouse moved event.
             *
             * @param x The x coordinate of the mouse event.
             * @param y The y coordinate of the mouse event.
             */
            void pushMouseMoved(int x, int y);

            // Inherited from Input

            bool isKeyQueueEmpty() override;

            KeyInput dequeueKeyInput() override;

            bool isMouseQueueEmpty() override;

            MouseInput dequeueMouseInput() override;

            bool isTextQueueEmpty() override;

            std::string dequeueTextInput() override;

            void _pollInput() override;

        protected:
            /**
             * Holds the key input queue.
             */
            std::queue<KeyInput> mKeyInputQueue;

            /**
             * Holds the mouse input queue.
             */
            std::queue<MouseInput> mMouseInputQueue;

            /**
             * Holds the text input queue.
             */
            std::queue<std::string> mTextInputQueue;
    };
} // namespace fcn

#endif // INCLUDE_FIFECHAN_GENERICINPUT_HPP_
