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

#include "guichan/hge/hgeinput.hpp"
#include "guichan/exception.hpp"

namespace gcn
{
    HGE *HGEInput::mHGE = NULL;

    HGEInput::HGEInput()
    {
        mHGE = hgeCreate(HGE_VERSION);

        mMouseX = 0;
        mMouseY = 0;

        mLeftMouseButtonDown = false;
        mRightMouseButtonDown = false;
        mMiddleMouseButtonDown = false;
    }

    HGEInput::~HGEInput()
    {
        mHGE->Release();
    }

    bool  HGEInput::isKeyQueueEmpty()
    {
        return mKeyInputQueue.empty();
    }

    bool  HGEInput::isMouseQueueEmpty()
    {
        return mMouseInputQueue.empty();
    }

    KeyInput  HGEInput::dequeueKeyInput()
    {
        if (isKeyQueueEmpty())
        {
            throw GCN_EXCEPTION("Key queue is empty.");
        }

        KeyInput keyInput;

        keyInput = mKeyInputQueue.front();
        mKeyInputQueue.pop();

        return keyInput;

    }

    MouseInput HGEInput::dequeueMouseInput()
    {
        if (isMouseQueueEmpty())
        {
            throw GCN_EXCEPTION("Mouse queue is empty.");
        }

        MouseInput mouseInput;

        mouseInput = mMouseInputQueue.front();
        mMouseInputQueue.pop();

        return mouseInput;
    }

    void  HGEInput::_pollInput()
    {
        hgeInputEvent ie;

        while (mHGE->Input_GetEvent(&ie)
            pollKeyInput(ie);

        pollMouseInput();
    }

    void  HGEInput::pollMouseInput()
    {
        float curMouseX, curMouseY;
        int mouseWheel;

        bool leftBtn, rightBtn, centerBtn;

        mHGE->Input_GetMousePos(&curMouseX, &curMouseY);
        mouseWheel = mHGE->Input_GetMouseWheel();

        leftBtn = mHGE->Input_GetKeyState(HGEK_LBUTTON);
        rightBtn = mHGE->Input_GetKeyState(HGEK_RBUTTON);
        centerBtn = mHGE->Input_GetKeyState(HGEK_MBUTTON);

        // Check mouse movement
        if (mMouseX != curMouseX 
            || mMouseY != curMouseY)
        {
            mMouseInputQueue.push(MouseInput(MouseInput::Empty,
                                             MouseInput::Moved,
                                             curMouseX,
                                             curMouseY,
                                             0));
     
            mMouseX = curMouseX;
            mMouseY = curMouseY;
        }

        // Check mouse wheel
        if (mouseWheel > 0)
        {
            mMouseInputQueue.push(MouseInput(MouseInput::Empty,
                                             MouseInput::WheelMovedUp,
                                             curMouseX,
                                             curMouseY,
                                             0));
        }
        else if (mouseWheel < 0)
        {
            mMouseInputQueue.push(MouseInput(MouseInput::Empty,
                                             MouseInput::WheelMovedDown,
                                             curMouseX,
                                             curMouseY,
                                             0));
        }

        //check mouse buttons
        if (!mLeftMouseButtonDown && leftBtn)
        {
            mMouseInputQueue.push(MouseInput(MouseInput::Left,
                                             MouseInput::Pressed,
                                             curMouseX,
                                             curMouseY,
                                             0));
        }
        else if (mLeftMouseButtonDown && !leftBtn )
        {
            mMouseInputQueue.push(MouseInput(MouseInput::Left,
                                             MouseInput::Released,
                                             curMouseX,
                                             curMouseY,
                                             0));
        }
        else if (!mRightMouseButtonDown && rightBtn)
        {
            mMouseInputQueue.push(MouseInput(MouseInput::Right,
                                             MouseInput::Pressed,
                                             curMouseX,
                                             curMouseY,
                                             0));
        }
        else if (mRightMouseButtonDown &&  !rightBtn)
        {
            mMouseInputQueue.push(MouseInput(MouseInput::Right,
                                             MouseInput::Released,
                                             curMouseX,
                                             curMouseY,
                                             0));
        }
        else if (!mMiddleMouseButtonDown && centerBtn)
        {
            mMouseInputQueue.push(MouseInput(MouseInput::Middle,
                                             MouseInput::Pressed,
                                             curMouseX,
                                             curMouseY,
                                             0));
        }
        else if (mMiddleMouseButtonDown && !centerBtn)
        {
            mMouseInputQueue.push(MouseInput(MouseInput::Middle,
                                             MouseInput::Released,
                                             curMouseX,
                                             curMouseY,
                                             0));
        }

        mLeftMouseButtonDown  = leftBtn;
        mRightMouseButtonDown = rightBtn;
        mMiddleMouseButtonDown  = centerBtn;
    }

    void  HGEInput::pollKeyInput(hgeInputEvent &ki)
    {
        if (ki.type != INPUT_KEYDOWN 
            && ki.type != INPUT_KEYUP )
        {
             return ;
        }
        
        if (ki.type == INPUT_KEYDOWN)
        {
            Key keyObj = convertToKey(ki.key, ki.chr);
  
            KeyInput keyInput(keyObj, KeyInput::Pressed);
  
            keyInput.setNumericPad(isNumericPad(ki.key));
  
            keyInput.setShiftPressed(ki.flags & HGEINP_SHIFT);
            keyInput.setAltPressed(ki.flags & HGEINP_ALT);
            keyInput.setControlPressed(ki.flags & HGEINP_CTRL);
        
            mKeyInputQueue.push(keyInput);
        }
  
        if (ki.type == INPUT_KEYUP)
        {
            Key keyObj = convertToKey(ki.key, ki.chr);
  
            KeyInput keyInput(keyObj, KeyInput::Released);
  
            keyInput.setNumericPad(isNumericPad(ki.key));
  
            keyInput.setShiftPressed(ki.flags & HGEINP_SHIFT);
            keyInput.setAltPressed(ki.flags & HGEINP_ALT);
            keyInput.setControlPressed(ki.flags & HGEINP_CTRL);
        
            mKeyInputQueue.push(keyInput);
        }
    }

    Key HGEInput::convertToKey(int key, int chr)
    {
        int keysym;

        switch(key)
        {
            case HGEK_TAB:
              keysym = Key::Tab;  
              break;
            case HGEK_ESCAPE:
              keysym = Key::Escape;
              break;
            case HGEK_ALT:
              keysym = Key::LeftAlt;
              break;
            case HGEK_SHIFT:
              keysym = Key::LeftShift;
              break;
            case HGEK_CTRL:
              keysym = Key::LeftControl;
              break;
            case HGEK_LWIN:
              keysym = Key::LeftMeta;
              break;
            case HGEK_RWIN:
              keysym = Key::RightMeta;
              break;   
            case HGEK_INSERT:
              keysym = Key::Insert;
              break;  
            case HGEK_HOME:
              keysym = Key::Home;
              break;  
            case HGEK_PGUP:
              keysym = Key::PageUp;
              break;
            case HGEK_PGDN:
              keysym = Key::PageDown;
              break;    
            case HGEK_DELETE:
              keysym = Key::Delete;
              break;
            case HGEK_END:
              keysym = Key::End;
              break;
            case HGEK_CAPSLOCK:
              keysym = Key::CapsLock;
              break;
            case HGEK_BACKSPACE:
              keysym = Key::Backspace;
              break;
            case HGEK_F1:
              keysym = Key::F1;
              break;
            case HGEK_F2:
              keysym = Key::F2;
              break;
            case HGEK_F3:
              keysym = Key::F3;
              break;
            case HGEK_F4:
              keysym = Key::F4;
              break;
            case HGEK_F5:
              keysym = Key::F5;
              break;
            case HGEK_F6:
              keysym = Key::F6;
              break;
            case HGEK_F7:
              keysym = Key::F7;
              break;
            case HGEK_F8:
              keysym = Key::F8;
              break;
            case HGEK_F9:
              keysym = Key::F9;
              break;
            case HGEK_F10:
              keysym = Key::F10;
              break;
            case HGEK_F11:
              keysym = Key::F11;
              break;
            case HGEK_F12:
              keysym = Key::F12;
              break;
            case HGEK_PAUSE:
              keysym = Key::Pause;
              break;
            case HGEK_SCROLLLOCK:
              keysym = Key::ScrollLock;
              break;
            case HGEK_NUMLOCK:
              keysym = Key::NumLock;
              break;
            case HGEK_LEFT:
              keysym = Key::Left;
              break;
            case HGEK_RIGHT:
              keysym = Key::Right;
              break;
            case HGEK_UP:
              keysym = Key::Up;
              break;
            case HGEK_DOWN:
              keysym = Key::Down;
              break;
            case HGEK_ENTER:
              keysym = Key::Enter;
              break;
            default:
              keysym = chr;
        }

        Key k = Key(keysym);

        return k;
    }

    bool  HGEInput::isNumericPad(int keyCode)
    {
        switch(keyCode)
        {
          case HGEK_NUMPAD0:
          case HGEK_NUMPAD1:
          case HGEK_NUMPAD2:
          case HGEK_NUMPAD3:
          case HGEK_NUMPAD4:
          case HGEK_NUMPAD5:
          case HGEK_NUMPAD6:
          case HGEK_NUMPAD7:
          case HGEK_NUMPAD8:
          case HGEK_NUMPAD9:
              return true;
          default: 
              return false;
        }
    }
}
