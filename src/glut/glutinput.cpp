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

// /*
//  * For comments regarding functions please see the header file.
//  */

// #include "fifechan/glut/glutinput.hpp"
// #include "fifechan/exception.hpp"

// namespace fcn
// {

//     GLUTInput::GLUTInput()
//     {
//     }

//     bool GLUTInput::isKeyQueueEmpty()
//     {
//         return mKeyInputQueue.empty();
//     }

//     KeyInput GLUTInput::dequeueKeyInput()
//     {
//         KeyInput keyInput;

//         if (mKeyInputQueue.empty())
//         {
//             throw FCN_EXCEPTION("The queue is empty.");
//         }

//         keyInput = mKeyInputQueue.front();
//         mKeyInputQueue.pop();

//         return keyInput;
//     }

//     bool GLUTInput::isMouseQueueEmpty()
//     {
//         return mMouseInputQueue.empty();
//     }

//     MouseInput GLUTInput::dequeueMouseInput()
//     {
//         MouseInput mouseInput;

//         if (mMouseInputQueue.empty())
//         {
//             throw FCN_EXCEPTION("The queue is empty.");
//         }

//         mouseInput = mMouseInputQueue.front();
//         mMouseInputQueue.pop();

//         return mouseInput;
//     }

//     void GLUTInput::pushKeyDownInput(unsigned char key)
//     {
//         KeyInput keyInput;
//         keyInput.setKey(convertKeyCharacter(key));
//         keyInput.setType(KeyInput::PRESS);
//         mKeyInputQueue.push(keyInput);
//     }

//     void GLUTInput::pushKeyUpInput(unsigned char key)
//     {
//         KeyInput keyInput;
//         keyInput.setKey(convertKeyCharacter(key));
//         keyInput.setType(KeyInput::RELEASE);
//         mKeyInputQueue.push(keyInput);
//     }

//     void GLUTInput::pushSpecialDownInput(unsigned char key)
//     {
//         KeyInput keyInput;
//         keyInput.setKey(convertSpecialCharacter(key));
//         keyInput.setType(KeyInput::PRESS);
//         mKeyInputQueue.push(keyInput);

//     }

//     void GLUTInput::pushSpecialUpInput(unsigned char key)
//     {
//         KeyInput keyInput;
//         keyInput.setKey(convertSpecialCharacter(key));
//         keyInput.setType(KeyInput::RELEASE);
//         mKeyInputQueue.push(keyInput);
//     }

//     void GLUTInput::pushMouseInput(int button, int state, int x, int y)
//     {
//         MouseInput mouseInput;
//         mouseInput.x = x;
//         mouseInput.y = y;
//         mouseInput.setButton(convertMouseButton(button));

//         if (state == GLUT_DOWN)
//         {
//             mouseInput.setType(MouseInput::PRESS);
//         }
//         else
//         {
//             mouseInput.setType(MouseInput::RELEASE);
//         }

//         mMouseInputQueue.push(mouseInput);
//     }

//     void GLUTInput::pushMotionInput(int x, int y)
//     {
//         MouseInput mouseInput;
//         mouseInput.x = x;
//         mouseInput.y = y;
//         mouseInput.setButton(MouseInput::EMPTY);
//         mouseInput.setType(MouseInput::MOTION);
//         mMouseInputQueue.push(mouseInput);
//     }

//     void GLUTInput::pushPassiveMotionInput(int x, int y)
//     {
//         pushMotionInput(x, y);
//     }

//     Key GLUTInput::convertKeyCharacter(unsigned char key)
//     {
//         int value = 0;
//         Key fcnKey;

//         value = (int)key;

//         switch(key)
//         {
//             case 8:
//                 value = Key::BACKSPACE;
//                 break;
//             case 13:
//                 value = Key::ENTER;
//                 break;
//             case 27:
//                 value = Key::ESCAPE;
//                 break;
//             case 127:
//                 value = Key::DELETE;
//                 break;
//         }

//         int modifiers = glutGetModifiers();
//         fcnKey.setShiftPressed(modifiers & GLUT_ACTIVE_SHIFT);
//         fcnKey.setControlPressed(modifiers & GLUT_ACTIVE_CTRL);
//         fcnKey.setAltPressed(modifiers & GLUT_ACTIVE_ALT);

//         fcnKey.setValue(value);

//         return fcnKey;
//     }

//     Key GLUTInput::convertSpecialCharacter(unsigned char key)
//     {
//         int value = 0;
//         Key fcnKey;

//         value = (int)key;

//         switch(key)
//         {
//             case GLUT_KEY_UP:
//                 value = Key::UP;
//                 break;
//             case GLUT_KEY_DOWN:
//                 value = Key::DOWN;
//                 break;
//             case GLUT_KEY_RIGHT:
//                 value = Key::RIGHT;
//                 break;
//             case GLUT_KEY_LEFT:
//                 value = Key::LEFT;
//                 break;
//             case GLUT_KEY_F1:
//                 value = Key::F1;
//                 break;
//             case GLUT_KEY_F2:
//                 value = Key::F2;
//                 break;
//             case GLUT_KEY_F3:
//                 value = Key::F3;
//                 break;
//             case GLUT_KEY_F4:
//                 value = Key::F4;
//                 break;
//             case GLUT_KEY_F5:
//                 value = Key::F5;
//                 break;
//             case GLUT_KEY_F6:
//                 value = Key::F6;
//                 break;
//             case GLUT_KEY_F7:
//                 value = Key::F7;
//                 break;
//             case GLUT_KEY_F8:
//                 value = Key::F8;
//                 break;
//             case GLUT_KEY_F9:
//                 value = Key::F9;
//                 break;
//             case GLUT_KEY_F10:
//                 value = Key::F10;
//                 break;
//             case GLUT_KEY_F11:
//                 value = Key::F11;
//                 break;
//             case GLUT_KEY_F12:
//                 value = Key::F12;
//                 break;
//             case GLUT_KEY_PAGE_UP:
//                 value = Key::PAGE_UP;
//                 break;
//             case GLUT_KEY_PAGE_DOWN:
//                 value = Key::PAGE_DOWN;
//                 break;
//             case GLUT_KEY_HOME:
//                 value = Key::HOME;
//                 break;
//             case GLUT_KEY_END:
//                 value = Key::END;
//                 break;
//             case GLUT_KEY_INSERT:
//                 value = Key::INSERT;
//                 break;
//             default:
//                 break;
//         }

//         int modifiers = glutGetModifiers();
//         fcnKey.setShiftPressed(modifiers & GLUT_ACTIVE_SHIFT);
//         fcnKey.setControlPressed(modifiers & GLUT_ACTIVE_CTRL);
//         fcnKey.setAltPressed(modifiers & GLUT_ACTIVE_ALT);

//         fcnKey.setValue(value);

//         return fcnKey;
//     }

//     int GLUTInput::convertMouseButton(int button)
//     {
//         switch (button)
//         {
//             case GLUT_LEFT_BUTTON:
//                 return MouseInput::LEFT;
//                 break;
//             case GLUT_RIGHT_BUTTON:
//                 return MouseInput::RIGHT;
//                 break;
//             case GLUT_MIDDLE_BUTTON:
//                 return MouseInput::MIDDLE;
//                 break;
//         }

//         throw FCN_EXCEPTION("Unknown GLUT mouse type.");

//         return 0;
//     }
// }
