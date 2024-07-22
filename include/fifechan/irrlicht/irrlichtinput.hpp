// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#ifndef FCN_IRRLICHTINPUT_HPP
#define FCN_IRRLICHTINPUT_HPP

#include <queue>

#include "fifechan/input.hpp"
#include "fifechan/keyinput.hpp"
#include "fifechan/mouseinput.hpp"
#include "fifechan/platform.hpp"
#include "irrlicht.h"

namespace fcn
{
    class Key;

    /**
     * Irrlicht implementation of Input.
     */
    class FIFEGUI_EXT_API IrrlichtInput : public Input
    {
    public:
        /**
         * Constructor.
         */
        IrrlichtInput(irr::IrrlichtDevice* device);

        /**
         * Pushes an Irrlicht event. It should be called at least once per frame to
         * update input with user input.
         *
         * @param event an event from Irrlicht.
         */
        virtual void pushInput(irr::SEvent const & event);

        /**
         * Polls all input. It exists for input driver compatibility. If you
         * only use Irrlicht and plan sticking with Irrlicht you can safely ignore this
         * function as it in the Irrlicht case does nothing.
         */
        virtual void _pollInput() { }

        // Inherited from Input

        virtual bool isKeyQueueEmpty();

        virtual KeyInput dequeueKeyInput();

        virtual bool isMouseQueueEmpty();

        virtual MouseInput dequeueMouseInput();

    protected:
        /**
         * Converts an Irrlicht event to a Fifechan key value.
         *
         * @param keysym The Irrlicht event to convert.
         * @return A Fifechan key value. -1 if no conversion took place.
         * @see Key
         */
        int convertIrrlichtEventToFifechanKeyValue(irr::SEvent const & event);

        std::queue<KeyInput> mKeyInputQueue;
        std::queue<MouseInput> mMouseInputQueue;

        irr::IrrlichtDevice* mIrrlichtDevice;
        bool mAltKeyDown;
        bool mMetaKeyDown;
    };
} // namespace fcn

#endif // end FCN_IRRLICHTINPUT_HPP
