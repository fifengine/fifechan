/***************************************************************************
 *   Copyright (C) 2012 by the fifechan team                               *
 *   https://github.com/fifechan                                           *
 *   This file is part of fifechan.                                        *
 *                                                                         *
 *   fifechan is free software; you can redistribute it and/or             *
 *   modify it under the terms of the GNU Lesser General Public            *
 *   License as published by the Free Software Foundation; either          *
 *   version 2.1 of the License, or (at your option) any later version.    *
 *                                                                         *
 *   This library is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU     *
 *   Lesser General Public License for more details.                       *
 *                                                                         *
 *   You should have received a copy of the GNU Lesser General Public      *
 *   License along with this library; if not, write to the                 *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA          *
 ***************************************************************************/

/*      _______   __   __   __   ______   __   __   _______   __   __
 *     / _____/\ / /\ / /\ / /\ / ____/\ / /\ / /\ / ___  /\ /  |\/ /\
 *    / /\____\// / // / // / // /\___\// /_// / // /\_/ / // , |/ / /
 *   / / /__   / / // / // / // / /    / ___  / // ___  / // /| ' / /
 *  / /_// /\ / /_// / // / // /_/_   / / // / // /\_/ / // / |  / /
 * /______/ //______/ //_/ //_____/\ /_/ //_/ //_/ //_/ //_/ /|_/ /
 * \______\/ \______\/ \_\/ \_____\/ \_\/ \_\/ \_\/ \_\/ \_\/ \_\/
 *
 * Copyright (c) 2004 - 2008 Olof Naessén and Per Larsson
 *
 *
 * Per Larsson a.k.a finalman
 * Olof Naessén a.k.a jansem/yakslem
 *
 * Visit: http://guichan.sourceforge.net
 *
 * License: (BSD)
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name of Guichan nor the names of its contributors may
 *    be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 * TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
* For comments regarding functions please see the header file.
*/

#include "fifechan/irrlicht/irrlichtinput.hpp"

#include "fifechan/exception.hpp"

namespace fcn
{
    IrrlichtInput::IrrlichtInput(irr::IrrlichtDevice* device) : mIrrlichtDevice(device),
    mAltKeyDown(false), mMetaKeyDown(false)
    {
    }

    bool IrrlichtInput::isKeyQueueEmpty()
    {
        return mKeyInputQueue.empty();
    }

    KeyInput IrrlichtInput::dequeueKeyInput()
    {
        KeyInput keyInput;

        if (mKeyInputQueue.empty())
        {
            throw FCN_EXCEPTION("The queue is empty.");
        }

        keyInput = mKeyInputQueue.front();
        mKeyInputQueue.pop();

        return keyInput;
    }

    bool IrrlichtInput::isMouseQueueEmpty()
    {
        return mMouseInputQueue.empty();
    }

    MouseInput IrrlichtInput::dequeueMouseInput()
    {
        MouseInput mouseInput;

        if (mMouseInputQueue.empty())
        {
            throw FCN_EXCEPTION("The queue is empty.");
        }

        mouseInput = mMouseInputQueue.front();
        mMouseInputQueue.pop();

        return mouseInput;
    }

    void IrrlichtInput::pushInput(const irr::SEvent& event)
    {
        KeyInput keyInput;
        MouseInput mouseInput;

        if (event.EventType == irr::EET_MOUSE_INPUT_EVENT)
        {
            mouseInput.setX(event.MouseInput.X);
            mouseInput.setY(event.MouseInput.Y);

            irr::u32 ticks = mIrrlichtDevice->getTimer()->getRealTime();

            switch(event.MouseInput.Event)
            {
                case irr::EMIE_LMOUSE_PRESSED_DOWN:
                    mouseInput.setButton(MouseInput::Left);
                    mouseInput.setType(MouseInput::Pressed);
                    mouseInput.setTimeStamp(ticks);
                    mMouseInputQueue.push(mouseInput);
                    break;

                case irr::EMIE_RMOUSE_PRESSED_DOWN:
                    mouseInput.setButton(MouseInput::Right);
                    mouseInput.setType(MouseInput::Pressed);
                    mouseInput.setTimeStamp(ticks);
                    mMouseInputQueue.push(mouseInput);
                    break;

                case irr::EMIE_MMOUSE_PRESSED_DOWN:
                    mouseInput.setButton(MouseInput::Middle);
                    mouseInput.setType(MouseInput::Pressed);
                    mouseInput.setTimeStamp(ticks);
                    mMouseInputQueue.push(mouseInput);
                    break;

                case irr::EMIE_LMOUSE_LEFT_UP:
                    mouseInput.setButton(MouseInput::Left);
                    mouseInput.setType(MouseInput::Released);
                    mouseInput.setTimeStamp(ticks);
                    mMouseInputQueue.push(mouseInput);
                    break;

                case irr::EMIE_RMOUSE_LEFT_UP:
                    mouseInput.setButton(MouseInput::Right);
                    mouseInput.setType(MouseInput::Released);
                    mouseInput.setTimeStamp(ticks);
                    mMouseInputQueue.push(mouseInput);
                    break;

                case irr::EMIE_MMOUSE_LEFT_UP:
                    mouseInput.setButton(MouseInput::Middle);
                    mouseInput.setType(MouseInput::Released);
                    mouseInput.setTimeStamp(ticks);
                    mMouseInputQueue.push(mouseInput);
                    break;

                case irr::EMIE_MOUSE_MOVED:
                    mouseInput.setButton(MouseInput::Empty);
                    mouseInput.setType(MouseInput::Moved);
                    mouseInput.setTimeStamp(ticks);
                    mMouseInputQueue.push(mouseInput);
                    break;

                case irr::EMIE_MOUSE_WHEEL:
                    mouseInput.setButton(MouseInput::Middle);
                    mouseInput.setType(event.MouseInput.Wheel > 0 ? MouseInput::WheelMovedUp : MouseInput::WheelMovedDown);
                    mouseInput.setTimeStamp(ticks);
                    mMouseInputQueue.push(mouseInput);
                    break;

                default:
                    break;
            }
        }

        else if(event.EventType == irr::EET_KEY_INPUT_EVENT)
        {
            int value = convertIrrlichtEventToFifechanKeyValue(event);

            if (value == -1)
            {
                value = (int)event.KeyInput.Char;
            }

            keyInput.setKey(Key(value));
            keyInput.setType(event.KeyInput.PressedDown ? KeyInput::Pressed : KeyInput::Released);
            keyInput.setShiftPressed(event.KeyInput.Shift);
            keyInput.setControlPressed(event.KeyInput.Control);
            keyInput.setAltPressed(mAltKeyDown);
            keyInput.setMetaPressed(mMetaKeyDown);
            keyInput.setNumericPad(event.KeyInput.Key >= irr::KEY_NUMPAD0 && event.KeyInput.Key <= irr::KEY_DIVIDE); 

            mKeyInputQueue.push(keyInput);
        }
    }

    int IrrlichtInput::convertIrrlichtEventToFifechanKeyValue(const irr::SEvent& event)
    {
        int value = -1;

        switch (event.KeyInput.Key)
        {
            case irr::KEY_LBUTTON:
                break;
            case irr::KEY_RBUTTON:
                break;
            case irr::KEY_CANCEL:
                break;
            case irr::KEY_MBUTTON:
                break;
            case irr::KEY_XBUTTON1:
                break;
            case irr::KEY_XBUTTON2:
                break;
            case irr::KEY_BACK:
                value = Key::Backspace;
                break;
            case irr::KEY_TAB:
                value = Key::Tab;
                break;
            case irr::KEY_CLEAR:
                break;
            case irr::KEY_RETURN:
                value = Key::Enter;
                break;
            case irr::KEY_SHIFT:
                break;
            case irr::KEY_CONTROL:
                break;
            case irr::KEY_MENU:
                mAltKeyDown = event.KeyInput.PressedDown;
                value = Key::LeftAlt; // is this right?
                break;
            case irr::KEY_PAUSE:
                value = Key::Pause;
                break;
            case irr::KEY_CAPITAL:
                value = Key::CapsLock;
                break;
            case irr::KEY_KANA:
                break;
            case irr::KEY_JUNJA:
                break;
            case irr::KEY_FINAL:
                break;
            case irr::KEY_KANJI:
                break;
            case irr::KEY_ESCAPE:
                value = Key::Escape;
                break;
            case irr::KEY_CONVERT:
                break;
            case irr::KEY_NONCONVERT:
                break;
            case irr::KEY_ACCEPT:
                break;
            case irr::KEY_MODECHANGE:
                value = Key::AltGr;
                break;
            case irr::KEY_SPACE:
                value = Key::Space;
                break;
            case irr::KEY_PRIOR:
                value = Key::PageUp;
                break;
            case irr::KEY_NEXT:
                value = Key::PageDown;
                break;
            case irr::KEY_END:
                value = Key::End;
                break;
            case irr::KEY_HOME:
                value = Key::Home;
                break;
            case irr::KEY_LEFT:
                value = Key::Left;
                break;
            case irr::KEY_UP:
                value = Key::Up;
                break;
            case irr::KEY_RIGHT:
                value = Key::Right;
                break;
            case irr::KEY_DOWN:
                value = Key::Down;
                break;
            case irr::KEY_SELECT:
                break;
            case irr::KEY_PRINT:
                value = Key::PrintScreen; // is this right?
                break;
            case irr::KEY_EXECUT:
                break;
            case irr::KEY_SNAPSHOT:
                value = Key::PrintScreen; // is this right?
                break;
            case irr::KEY_INSERT:
                value = Key::Insert;
                break;
            case irr::KEY_DELETE:
                value = Key::Delete;
                break;
            case irr::KEY_HELP:
                break;

            case irr::KEY_LWIN:
                mMetaKeyDown = event.KeyInput.PressedDown;
                value = Key::LeftMeta;
                break;
            case irr::KEY_RWIN:
                value = Key::RightMeta;
                break;
            case irr::KEY_APPS:
                break;
            case irr::KEY_SLEEP:
                break;

            case irr::KEY_F1:
                value = Key::F1;
                break;
            case irr::KEY_F2:
                value = Key::F2;
                break;
            case irr::KEY_F3:
                value = Key::F3;
                break;
            case irr::KEY_F4:
                value = Key::F4;
                break;
            case irr::KEY_F5:
                value = Key::F5;
                break;
            case irr::KEY_F6:
                value = Key::F6;
                break;
            case irr::KEY_F7:
                value = Key::F7;
                break;
            case irr::KEY_F8:
                value = Key::F8;
                break;
            case irr::KEY_F9:
                value = Key::F9;
                break;
            case irr::KEY_F10:
                value = Key::F10;
                break;
            case irr::KEY_F11:
                value = Key::F11;
                break;
            case irr::KEY_F12:
                value = Key::F12;
                break;
            case irr::KEY_F13:
                value = Key::F13;
                break;
            case irr::KEY_F14:
                value = Key::F14;
                break;
            case irr::KEY_F15:
                value = Key::F15;
                break;
            case irr::KEY_NUMLOCK:
                value = Key::NumLock;
                break;
            case irr::KEY_SCROLL:
                value = Key::ScrollLock;
                break;
            case irr::KEY_LSHIFT:
                value = Key::LeftShift;
                break;
            case irr::KEY_RSHIFT:
                value = Key::RightShift;
                break;
            case irr::KEY_LCONTROL:
                value = Key::LeftControl;
                break;
            case irr::KEY_RCONTROL:
                value = Key::RightControl;
                break;
            case irr::KEY_LMENU:
                value = Key::LeftSuper;
                break;
            case irr::KEY_RMENU:
                value = Key::RightSuper;
                break;
            default:
                break;
        }

        return value;
    }
}
