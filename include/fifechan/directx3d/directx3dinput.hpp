// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#ifndef FCN_DIRECTX3DINPUT_HPP
#define FCN_DIRECTX3DINPUT_HPP

#include <queue>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "fifechan/input.hpp"
#include "fifechan/keyinput.hpp"
#include "fifechan/mouseinput.hpp"
#include "fifechan/platform.hpp"

namespace fcn
{
    class Key;

    /**
     * DirectX 3D implementation of Input.
     */
    class FIFEGUI_EXT_API DirectX3DInput : public Input
    {
    public:
        /**
         * Constructor.
         */
        DirectX3DInput();

        /**
         * Dispatches a windows message. It should be called as soon as a windows
         * message is received in the window proc function.
         *
         * @param window A windows window handle.
         * @param message A message from windows.
         */
        virtual void dispatchMessage(HWND window, MSG message);

        /**
         * Polls all input. It exists for input driver compatibility. If you
         * only use DirectX and plan sticking with DirectX you can safely ignore this
         * function as it in the DirectX case does nothing.
         */
        virtual void _pollInput() { }

        // Inherited from Input

        virtual bool isKeyQueueEmpty();

        virtual KeyInput dequeueKeyInput();

        virtual bool isMouseQueueEmpty();

        virtual MouseInput dequeueMouseInput();

    protected:
        /**
         * Converts a character to a key value.
         *
         * @param wParam Windows parameter to retreive
         *               character information from.
         * @param lParam Windows parameter to retreive
         *               character information from.
         * @param kbstate The keybord state retreived from
         *                GetKeyboardState.
         * @return A key value.
         * @see Key
         */
        int convertKeyCharacter(WPARAM wParam, LPARAM lParam, unsigned char* kbstate);

        std::queue<KeyInput> mKeyInputQueue;
        std::queue<MouseInput> mMouseInputQueue;

        int mLastMouseZ;
        bool mMouseDown;
        bool mMouseInWindow;
    };
} // namespace fcn

#endif // end FCN_DIRECTX3DINPUT_HPP
