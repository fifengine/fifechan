// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#ifndef FCN_HGEINPUT_HPP
#define FCN_HGEINPUT_HPP

#include <queue>

#include <hge.h>

#if defined(DELETE)
#    undef DELETE
#endif

#include "fifechan/input.hpp"
#include "fifechan/keyinput.hpp"
#include "fifechan/mouseinput.hpp"
#include "fifechan/platform.hpp"

namespace fcn
{
    /**
     * HGE implementation of Input.
     */
    class FIFEGUI_EXT_API HGEInput : public Input
    {
    public:
        /**
         *  Constructor.
         */
        HGEInput();

        /**
         *  Destructor.
         */
        virtual ~HGEInput();

        // Inherited from Input

        virtual bool isKeyQueueEmpty();

        virtual bool isMouseQueueEmpty();

        virtual KeyInput dequeueKeyInput();

        virtual MouseInput dequeueMouseInput();

        virtual void _pollInput();

    protected:
        /**
         * Handles key input. The function is called by _pollInput.
         *
         * @param ki the hge input event to handle.
         */
        void pollKeyInput(hgeInputEvent& ki);

        /**
         * Handles mouse input. The function is called by _pollInput.
         *
         * This function directly deal with the mouse input , and it avoid
         * the odd things in HGE.
         */
        void pollMouseInput();

        /**
         * Converts the keycode and characters to a Key object.
         *
         * @param key The key to convert.
         * @param chr The character to convert.
         */
        Key convertToKey(int key, int chr);

        /**
         * Checks if a keyCode is numeric.
         *
         * @param keyCode The key code to check.
         */
        bool isNumericPad(int keyCode);

    protected:
        static HGE* mHGE;

        std::queue<KeyInput> mKeyInputQueue;
        std::queue<MouseInput> mMouseInputQueue;

        float mMouseX;
        float mMouseY;

        bool mLeftMouseButtonDown;
        bool mRightMouseButtonDown;
        bool mMiddleMouseButtonDown;
    };
} // namespace fcn

#endif // end FCN_HGEINPUT_HPP
