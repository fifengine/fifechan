/***************************************************************************
 *   Copyright (C) 2012 by the fifechan team                               *
 *   http://fifechan.github.com/fifechan                                   *
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
 * Copyright (c) 2004, 2005, 2006, 2007 Olof Naessén and Per Larsson
 *
 *                                                         Js_./
 * Per Larsson a.k.a finalman                          _RqZ{a<^_aa
 * Olof Naessén a.k.a jansem/yakslem                _asww7!uY`>  )\a//
 *                                                 _Qhm`] _f "'c  1!5m
 * Visit: http://guichan.darkbits.org             )Qk<P ` _: :+' .'  "{[
 *                                               .)j(] .d_/ '-(  P .   S
 * License: (BSD)                                <Td/Z <fP"5(\"??"\a.  .L
 * Redistribution and use in source and          _dV>ws?a-?'      ._/L  #'
 * binary forms, with or without                 )4d[#7r, .   '     )d`)[
 * modification, are permitted provided         _Q-5'5W..j/?'   -?!\)cam'
 * that the following conditions are met:       j<<WP+k/);.        _W=j f
 * 1. Redistributions of source code must       .$%w\/]Q  . ."'  .  mj$
 *    retain the above copyright notice,        ]E.pYY(Q]>.   a     J@\
 *    this list of conditions and the           j(]1u<sE"L,. .   ./^ ]{a
 *    following disclaimer.                     4'_uomm\.  )L);-4     (3=
 * 2. Redistributions in binary form must        )_]X{Z('a_"a7'<a"a,  ]"[
 *    reproduce the above copyright notice,       #}<]m7`Za??4,P-"'7. ).m
 *    this list of conditions and the            ]d2e)Q(<Q(  ?94   b-  LQ/
 *    following disclaimer in the                <B!</]C)d_, '(<' .f. =C+m
 *    documentation and/or other materials      .Z!=J ]e []('-4f _ ) -.)m]'
 *    provided with the distribution.          .w[5]' _[ /.)_-"+?   _/ <W"
 * 3. Neither the name of Guichan nor the      :$we` _! + _/ .        j?
 *    names of its contributors may be used     =3)= _f  (_yQmWW$#(    "
 *    to endorse or promote products derived     -   W,  sQQQQmZQ#Wwa]..
 *    from this software without specific        (js, \[QQW$QWW#?!V"".
 *    prior written permission.                    ]y:.<\..          .
 *                                                 -]n w/ '         [.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT       )/ )/           !
 * HOLDERS AND CONTRIBUTORS "AS IS" AND ANY         <  (; sac    ,    '
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING,               ]^ .-  %
 * BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF            c <   r
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR            aga<  <La
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE          5%  )P'-3L
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR        _bQf` y`..)a
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,          ,J?4P'.P"_(\?d'.,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES               _Pa,)!f/<[]/  ?"
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT      _2-..:. .r+_,.. .
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,     ?a.<%"'  " -'.a_ _,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION)                     ^
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
 * IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 * For comments regarding functions please see the header file.
 */

#include "fifechan/directx3d/directx3dinput.hpp"

#include "fifechan/exception.hpp"

namespace fcn
{
    DirectX3DInput::DirectX3DInput()
        : mLastMouseZ(0),
          mMouseDown(false),
          mMouseInWindow(true)
    {
    }

    bool DirectX3DInput::isKeyQueueEmpty()
    {
        return mKeyInputQueue.empty();
    }

    KeyInput DirectX3DInput::dequeueKeyInput()
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

    bool DirectX3DInput::isMouseQueueEmpty()
    {
        return mMouseInputQueue.empty();
    }

    MouseInput DirectX3DInput::dequeueMouseInput()
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

    void DirectX3DInput:: dispatchMessage(HWND window, MSG message)
    {
        KeyInput keyInput;
        MouseInput mouseInput;
        WPARAM wParam = message.wParam;
        LPARAM lParam = message.lParam;

        switch (message.message)
        {
            case WM_SYSKEYDOWN:
            case WM_KEYDOWN:
                {
                    unsigned char kbstate[256];
	                GetKeyboardState(kbstate);
                    keyInput.setKey(Key(convertKeyCharacter(wParam, lParam, kbstate)));
                    keyInput.setType(KeyInput::Pressed);
                    keyInput.setShiftPressed(kbstate[VK_SHIFT] & 0x80);
	                keyInput.setControlPressed(kbstate[VK_CONTROL] & 0x80);
	                keyInput.setAltPressed(kbstate[VK_MENU] & 0x80);
                    keyInput.setNumericPad(wParam >= VK_NUMPAD0
                                            && wParam <= VK_DIVIDE);

                    mKeyInputQueue.push(keyInput);
                }
                break;
            case WM_SYSKEYUP:
            case WM_KEYUP:
                {
                    unsigned char kbstate[256];
	                GetKeyboardState(kbstate);
                    keyInput.setKey(Key(convertKeyCharacter(wParam, lParam, kbstate)));
                    keyInput.setType(KeyInput::Released);
                    keyInput.setShiftPressed(kbstate[VK_SHIFT] & 0x80);
	                keyInput.setControlPressed(kbstate[VK_CONTROL] & 0x80);
	                keyInput.setAltPressed(kbstate[VK_MENU] & 0x80);
                    keyInput.setNumericPad(wParam >= VK_NUMPAD0
                                            && wParam <= VK_DIVIDE);

                    mKeyInputQueue.push(keyInput);
                }
                break;
            case WM_LBUTTONDOWN:
                {
                    int x = (int)((signed short)(LOWORD(lParam)));
                    int y = (int)((signed short)(HIWORD(lParam)));
               
                    mouseInput.setX(x);
                    mouseInput.setY(y);
                    mouseInput.setButton(MouseInput::Left);
                    mouseInput.setType(MouseInput::Pressed);
                    mouseInput.setTimeStamp(GetTickCount());
                    mMouseInputQueue.push(mouseInput);
                    mMouseDown = true;
                    SetCapture(window);
                    break;
                }
            case WM_MBUTTONDOWN:
                {
                    int x = (int)((signed short)(LOWORD(lParam)));
                    int y = (int)((signed short)(HIWORD(lParam)));
                   
                    mouseInput.setX(x);
                    mouseInput.setY(y);
                    mouseInput.setButton(MouseInput::Middle);
                    mouseInput.setType(MouseInput::Pressed);
                    mouseInput.setTimeStamp(GetTickCount());
                    mMouseInputQueue.push(mouseInput);
                    mMouseDown = true;
                    SetCapture(window);
                    break;
                }
            case WM_RBUTTONDOWN:
                {
                    int x = (int)((signed short)(LOWORD(lParam)));
                    int y = (int)((signed short)(HIWORD(lParam)));
                   
                    mouseInput.setX(x);
                    mouseInput.setY(y);
                    mouseInput.setButton(MouseInput::Right);
                    mouseInput.setType(MouseInput::Pressed);
                    mouseInput.setTimeStamp(GetTickCount());
                    mMouseInputQueue.push(mouseInput);
                    mMouseDown = true;
                    SetCapture(window);
                    break;
                }
            case WM_LBUTTONUP:
                {
                    int x = (int)((signed short)(LOWORD(lParam)));
                    int y = (int)((signed short)(HIWORD(lParam)));
                   
                    mouseInput.setX(x);
                    mouseInput.setY(y);
                    mouseInput.setButton(MouseInput::Left);
                    mouseInput.setType(MouseInput::Released);
                    mouseInput.setTimeStamp(GetTickCount());
                    mMouseInputQueue.push(mouseInput);
                    mMouseDown = false;
                    ReleaseCapture();
                    break;
                }
            case WM_MBUTTONUP:
                {
                    int x = (int)((signed short)(LOWORD(lParam)));
                    int y = (int)((signed short)(HIWORD(lParam)));
                   
                    mouseInput.setX(x);
                    mouseInput.setY(y);
                    mouseInput.setButton(MouseInput::Middle);
                    mouseInput.setType(MouseInput::Released);
                    mouseInput.setTimeStamp(GetTickCount());
                    mMouseInputQueue.push(mouseInput);
                    mMouseDown = false;
                    ReleaseCapture();
                    break;
                }
            case WM_RBUTTONUP:
                {
                    int x = (int)((signed short)(LOWORD(lParam)));
                    int y = (int)((signed short)(HIWORD(lParam)));
                  
                    mouseInput.setX(x);
                    mouseInput.setY(y);
                    mouseInput.setButton(MouseInput::Right);
                    mouseInput.setType(MouseInput::Released);
                    mouseInput.setTimeStamp(GetTickCount());
                    mMouseInputQueue.push(mouseInput);
                    mMouseDown = false;
                    ReleaseCapture();
                    break;
                }
            case WM_MOUSEMOVE:
                {
                    int x = (int)((signed short)(LOWORD(lParam)));
                    int y = (int)((signed short)(HIWORD(lParam)));
                    /*
                    POINT point = {x,y};
                    ScreenToClient(hWnd, &point);
                   
                    RECT rect; 
                    GetClientRect(hWnd, &rect);*/
                    /*
                    if (!mMouseDown
                        && mMouseInWindow
                        && (point.x < 0 
                            || point.y < 0 
                            || point.x > rect.right 
                            || point.y > rect.bottom))
                    {
                        mouseInput.setX(-1);
                        mouseInput.setY(-1);
                        mouseInput.setButton(MouseInput::Empty);
                        mouseInput.setType(MouseInput::Moved);
                        mouseInput.setTimeStamp(GetTickCount());
                   
                        mMouseInputQueue.push(mouseInput);
                        mMouseInWindow = false;
                    }
                    else if (mMouseDown
                              || (point.x >= 0 
                                    && point.y >= 0 
                                    && point.x < rect.right 
                                    && point.y < rect.bottom))
                    {
                        mouseInput.setX(point.x);
                        mouseInput.setY(point.y);
                        mouseInput.setButton(MouseInput::Empty);
                        mouseInput.setType(MouseInput::Moved);
                        mouseInput.setTimeStamp(GetTickCount());
                   
                        mMouseInputQueue.push(mouseInput);
                    }
                    
                    mMouseInWindow = point.x >= 0 
                                        && point.y >= 0 
                                        && point.x < rect.right 
                                        && point.y < rect.bottom;
                */
                        mouseInput.setX(x);
                        mouseInput.setY(y);
                        mouseInput.setButton(MouseInput::Empty);
                        mouseInput.setType(MouseInput::Moved);
                        mouseInput.setTimeStamp(GetTickCount());
                   
                        mMouseInputQueue.push(mouseInput);
                    break;
                }
            // For some reason WM_MOUSEWHEEL isn't defined at some systems
            // (including mine), but 0x020A should be equal to the WM_MOUSEWHEEL
            // value.
            case 0x020A: // WM_MOUSEWHEEL         
                int mouseZ = (short)HIWORD(wParam);
                int x = (int)((signed short)(LOWORD(lParam)));
                int y = (int)((signed short)(HIWORD(lParam)));
                POINT point = {x,y};
                ScreenToClient(window, &point);

                RECT rect; 
                GetClientRect(window, &rect);

                if (point.x < 0 
                    || point.y < 0 
                    || point.x > rect.right 
                    || point.y > rect.bottom)
                {
                    break;
                }

                // Mouse wheels doesn't have to move 120 units anymore, new
                // mouses can move less than 120, therefore we need to check
                // that the mouse has at least moved 120 units before we push
                // an input event.
                if (mLastMouseZ + mouseZ >= 120)
                {
                    mMouseInputQueue.push(MouseInput(MouseInput::Empty,
                                                     MouseInput::WheelMovedUp,
                                                     point.x,
                                                     point.y,
                                                     GetTickCount()));
                    mLastMouseZ = 0;
                }
                else if (mLastMouseZ + mouseZ <= -120)
                {     
                    mMouseInputQueue.push(MouseInput(MouseInput::Empty,
                                                     MouseInput::WheelMovedDown,
                                                     point.x,
                                                     point.y,
                                                     GetTickCount()));
                    mLastMouseZ = 0;
                }
                else
                {
                    mLastMouseZ += mouseZ;
                }

                break;
        } // end switch
    }

    int DirectX3DInput::convertKeyCharacter(WPARAM wParam, 
                                            LPARAM lParam,
                                            unsigned char* kbstate)
    {
        int value = -1;

        switch (wParam)
        {
          //VK_CLEAR
          //VK_EXECUTE
          //VK_PRINT
          //VK_HELP
          case VK_TAB:
              value = Key::Tab;
              break;
          case VK_LMENU:
              value = Key::LeftAlt;
              break;
          case VK_RMENU:
              value = Key::RightAlt;
              break;
          case VK_LSHIFT:
              value = Key::LeftShift;
              break;
          case VK_RSHIFT:
              value = Key::RightShift;
              break;
          case VK_LCONTROL:
              value = Key::LeftControl;
              break;
          case VK_RCONTROL:
              value = Key::RightControl;
              break;
          case VK_BACK:
              value = Key::Backspace;
              break;
          case VK_PAUSE:
              value = Key::Pause;
              break;
          case VK_SPACE:
              value = Key::Space;
              break;
          case VK_ESCAPE:
              value = Key::Escape;
              break;
          case VK_DELETE:
              value = Key::Delete;
              break;
          case VK_INSERT:
              value = Key::Insert;
              break;
          case VK_HOME:
              value = Key::Home;
              break;
          case VK_END:
              value = Key::End;
              break;
          case VK_PRIOR:
              value = Key::PageUp;
              break;
          case VK_SNAPSHOT:
              value = Key::PrintScreen;
              break;
          case VK_NEXT:
              value = Key::PageDown;
              break;
          case VK_F1:
              value = Key::F1;
              break;
          case VK_F2:
              value = Key::F2;
              break;
          case VK_F3:
              value = Key::F3;
              break;
          case VK_F4:
              value = Key::F4;
              break;
          case VK_F5:
              value = Key::F5;
              break;
          case VK_F6:
              value = Key::F6;
              break;
          case VK_F7:
              value = Key::F7;
              break;
          case VK_F8:
              value = Key::F8;
              break;
          case VK_F9:
              value = Key::F9;
              break;
          case VK_F10:
              value = Key::F10;
              break;
          case VK_F11:
              value = Key::F11;
              break;
          case VK_F12:
              value = Key::F12;
              break;
          case VK_F13:
              value = Key::F13;
              break;
          case VK_F14:
              value = Key::F14;
              break;
          case VK_F15:
              value = Key::F15;
              break;
          case VK_NUMLOCK:
              value = Key::NumLock;
              break;
          case VK_CAPITAL:
              value = Key::CapsLock;
              break;
          case VK_SCROLL:
              value = Key::ScrollLock;
              break;
          case VK_RWIN:
              value = Key::RightMeta;
              break;
          case VK_LWIN:
              value = Key::LeftMeta;
              break;
          case VK_UP:
              value = Key::Up;
              break;
          case VK_DOWN:
              value = Key::Down;
              break;
          case VK_LEFT:
              value = Key::Left;
              break;
          case VK_RIGHT:
              value = Key::Right;
              break;
          case VK_RETURN:
              value = Key::Enter;
              break;
          default:
              ToAscii(wParam, 
                      HIWORD(lParam) & 0xff, // Gets the scan code.
                      kbstate, 
                      (unsigned short*)&value, 
                      0);
              value = (char)value;
              break;
        }

        return value;
    }
}
