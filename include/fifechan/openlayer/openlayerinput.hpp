// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#ifndef FCN_OPENLAYERINPUT_HPP
#define FCN_OPENLAYERINPUT_HPP

#include <map>
#include <queue>

#include "fifechan/input.hpp"
#include "fifechan/keyinput.hpp"
#include "fifechan/mouseinput.hpp"
#include "fifechan/platform.hpp"

namespace fcn
{
    /**
     * OpenLayer implementation of the Input.
     */
    class FIFEGUI_EXT_API OpenLayerInput : public Input
    {
    public:
        /**
         * Constructor.
         */
        OpenLayerInput();

        /**
         * Destructor.
         */
        virtual ~OpenLayerInput() { }

        // Inherited from Input

        virtual bool isKeyQueueEmpty();

        virtual KeyInput dequeueKeyInput();

        virtual bool isMouseQueueEmpty();

        virtual MouseInput dequeueMouseInput();

        virtual void _pollInput();

    protected:
        /**
         * Handles the mouse input called by _pollInput.
         */
        virtual void pollMouseInput();

        /**
         * Handles the key input called by _pollInput.
         */
        virtual void pollKeyInput();

        /**
         * Converts scancode and unicode to Key object.
         */
        virtual Key convertToKey(int scancode, int unicode);

        virtual bool isNumericPad(int scancode);

        // This map holds the currently pressed Keys
        // so we can send the correct key releases.
        // it maps from scancode to key objects.
        std::map<int, KeyInput> mPressedKeys;

        std::queue<KeyInput> mKeyQueue;
        std::queue<MouseInput> mMouseQueue;

        bool mMouseButton1, mMouseButton2, mMouseButton3;
        int mLastMouseX, mLastMouseY, mLastMouseZ;
    };
} // namespace fcn

#endif // end FCN_OPENLAYERINPUT_HPP
