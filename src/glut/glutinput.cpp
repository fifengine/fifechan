/*      _______   __   __   __   ______   __   __   _______   __   __                 
 *     / _____/\ / /\ / /\ / /\ / ____/\ / /\ / /\ / ___  /\ /  |\/ /\                
 *    / /\____\// / // / // / // /\___\// /_// / // /\_/ / // , |/ / /                 
 *   / / /__   / / // / // / // / /    / ___  / // ___  / // /| ' / /                  
 *  / /_// /\ / /_// / // / // /_/_   / / // / // /\_/ / // / |  / /                   
 * /______/ //______/ //_/ //_____/\ /_/ //_/ //_/ //_/ //_/ /|_/ /                    
 * \______\/ \______\/ \_\/ \_____\/ \_\/ \_\/ \_\/ \_\/ \_\/ \_\/                      
 *
 * Copyright (c) 2004, 2005 darkbits                        Js_./
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

#include "guichan/glut/glutinput.hpp"
#include "guichan/exception.hpp"

namespace gcn
{

    GLUTInput::GLUTInput()
    {
    }
    
    bool GLUTInput::isKeyQueueEmpty()
    {
        return mKeyInputQueue.empty();
    }
  
    KeyInput GLUTInput::dequeueKeyInput()
    {
        KeyInput keyInput;
    
        if (mKeyInputQueue.empty())
        {
            throw GCN_EXCEPTION("The queue is empty.");
        }
    
        keyInput = mKeyInputQueue.front();
        mKeyInputQueue.pop();

        return keyInput;    
    }

    bool GLUTInput::isMouseQueueEmpty()
    {
        return mMouseInputQueue.empty();
    }

    MouseInput GLUTInput::dequeueMouseInput()
    {
        MouseInput mouseInput;
    
        if (mMouseInputQueue.empty())
        {
            throw GCN_EXCEPTION("The queue is empty.");
        }
    
        mouseInput = mMouseInputQueue.front();
        mMouseInputQueue.pop();

        return mouseInput;    
    }
  
    void GLUTInput::pushKeyDownInput(unsigned char key)
    {
        KeyInput keyInput;    
        keyInput.setKey(convertKeyCharacter(key));
        keyInput.setType(KeyInput::PRESS);        
        mKeyInputQueue.push(keyInput);
    }

    void GLUTInput::pushKeyUpInput(unsigned char key)
    {
        KeyInput keyInput;    
        keyInput.setKey(convertKeyCharacter(key));
        keyInput.setType(KeyInput::RELEASE);
        mKeyInputQueue.push(keyInput);
    }

    void GLUTInput::pushSpecialDownInput(unsigned char key)
    {
        KeyInput keyInput;    
        keyInput.setKey(convertSpecialCharacter(key));
        keyInput.setType(KeyInput::PRESS);        
        mKeyInputQueue.push(keyInput);

    }

    void GLUTInput::pushSpecialUpInput(unsigned char key)
    {
        KeyInput keyInput;    
        keyInput.setKey(convertSpecialCharacter(key));
        keyInput.setType(KeyInput::RELEASE);
        mKeyInputQueue.push(keyInput);
    }

    void GLUTInput::pushMouseInput(int button, int state, int x, int y)
    {
        MouseInput mouseInput;
        mouseInput.x = x;
        mouseInput.y = y;
        mouseInput.setButton(convertMouseButton(button));

        if (state == GLUT_DOWN)
        {
            mouseInput.setType(MouseInput::PRESS);
        }
        else
        {
            mouseInput.setType(MouseInput::RELEASE);
        }

        mMouseInputQueue.push(mouseInput);        
    }

    void GLUTInput::pushMotionInput(int x, int y)
    {
        MouseInput mouseInput;
        mouseInput.x = x;
        mouseInput.y = y;
        mouseInput.setButton(MouseInput::EMPTY);
        mouseInput.setType(MouseInput::MOTION);
        mMouseInputQueue.push(mouseInput);              
    }

    void GLUTInput::pushPassiveMotionInput(int x, int y)
    {
        pushMotionInput(x, y);
    }

    Key GLUTInput::convertKeyCharacter(unsigned char key)
    {
        int value = 0; 
        Key gcnKey;

        value = (int)key;

        switch(key)
        {
            case 8:
                value = Key::BACKSPACE;
                break;
            case 13:
                value = Key::ENTER;
                break;
            case 27:
                value = Key::ESCAPE;
                break;
            case 127:
                value = Key::DELETE;
                break;
        }

        int modifiers = glutGetModifiers();
        gcnKey.setShiftPressed(modifiers & GLUT_ACTIVE_SHIFT);
        gcnKey.setControlPressed(modifiers & GLUT_ACTIVE_CTRL);
        gcnKey.setAltPressed(modifiers & GLUT_ACTIVE_ALT);

        gcnKey.setValue(value);

        return gcnKey;
    }
    
    Key GLUTInput::convertSpecialCharacter(unsigned char key)
    {
        int value = 0; 
        Key gcnKey;

        value = (int)key;
        
        switch(key)
        {
            case GLUT_KEY_UP:
                value = Key::UP;
                break;
            case GLUT_KEY_DOWN:
                value = Key::DOWN;
                break;
            case GLUT_KEY_RIGHT:
                value = Key::RIGHT;
                break;
            case GLUT_KEY_LEFT:
                value = Key::LEFT;
                break;                
            case GLUT_KEY_F1:
                value = Key::F1;
                break;
            case GLUT_KEY_F2:
                value = Key::F2;
                break;
            case GLUT_KEY_F3:
                value = Key::F3;
                break;
            case GLUT_KEY_F4:
                value = Key::F4;
                break;
            case GLUT_KEY_F5:
                value = Key::F5;
                break;
            case GLUT_KEY_F6:
                value = Key::F6;
                break;
            case GLUT_KEY_F7:
                value = Key::F7;
                break;
            case GLUT_KEY_F8:
                value = Key::F8;
                break;
            case GLUT_KEY_F9:
                value = Key::F9;
                break;
            case GLUT_KEY_F10:
                value = Key::F10;
                break;
            case GLUT_KEY_F11:
                value = Key::F11;
                break;
            case GLUT_KEY_F12:
                value = Key::F12;
                break;
            case GLUT_KEY_PAGE_UP:
                value = Key::PAGE_UP;
                break;
            case GLUT_KEY_PAGE_DOWN:
                value = Key::PAGE_DOWN;
                break;
            case GLUT_KEY_HOME:
                value = Key::HOME;
                break;
            case GLUT_KEY_END:
                value = Key::END;
                break;
            case GLUT_KEY_INSERT:
                value = Key::INSERT;
                break;                
            default:
                break;
        }

        int modifiers = glutGetModifiers();
        gcnKey.setShiftPressed(modifiers & GLUT_ACTIVE_SHIFT);
        gcnKey.setControlPressed(modifiers & GLUT_ACTIVE_CTRL);
        gcnKey.setAltPressed(modifiers & GLUT_ACTIVE_ALT);

        gcnKey.setValue(value);

        return gcnKey;
    }

    int GLUTInput::convertMouseButton(int button)
    {
        switch (button)
        {
            case GLUT_LEFT_BUTTON:
                return MouseInput::LEFT;
                break;
            case GLUT_RIGHT_BUTTON:
                return MouseInput::RIGHT;
                break;
            case GLUT_MIDDLE_BUTTON:
                return MouseInput::MIDDLE;
                break;
        } 
        
        throw GCN_EXCEPTION("Unknown GLUT mouse type.");
        
        return 0;
    }
}
