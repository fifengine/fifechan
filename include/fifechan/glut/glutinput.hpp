// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#ifndef INCLUDE_FIFECHAN_GLUT_GLUTINPUT_HPP_
#define INCLUDE_FIFECHAN_GLUT_GLUTINPUT_HPP_

// #include <queue>

// #include <GL/glut.h>

// #include "fifechan/input.hpp"
// #include "fifechan/key.hpp"
// #include "fifechan/keyinput.hpp"
// #include "fifechan/mouseinput.hpp"
// #include "fifechan/platform.hpp"

// namespace fcn
// {
//     /**
//      * GLUT implementation of Input.
//      */
//     class FIFEGUI_EXT_API GLUTInput : public Input
//     {
//     public:

//         /**
//          * Constructor.
//          */
//         GLUTInput();

//         /**
//          * Pushes a key down. It should be called in the
//          * glutKeyboardFunc call back function.
//          *
//          * @param key a key from GLUT.
//          */
//         virtual void pushKeyDownInput(unsigned char key);

//         /**
//          * Pushes a key up. It should be called in the
//          * glutKeyboardUpFunc call back function.
//          *
//          * @param key a key from GLUT.
//          */
//         virtual void pushKeyUpInput(unsigned char key);

//         /**
//          * Pushes a special key down. It should be called in the
//          * glutSpecialFunc call back function.
//          *
//          * @param key a key from GLUT.
//          */
//         virtual void pushSpecialDownInput(unsigned char key);

//         /**
//          * Pushes a special key up. It should be called in the
//          * glutSpecialUpFunc call back function.
//          *
//          * @param key a key from GLUT.
//          */
//         virtual void pushSpecialUpInput(unsigned char key);

//         /**
//          * Pushes a mouse input. It should be called in the glutMouseFunc
//          * call back function.
//          *
//          * @param button a button from GLUT.
//          * @param state a mouse state from GLUT.
//          * @param x an x coordinate from GLUT.
//          * @param y an y coordinate from GLUT.
//          */
//         virtual void pushMouseInput(int button, int state, int x, int y);

//         /**
//          * Pushes a mouse motion input. It should be called in the
//          * glutMotionFunc call back function.
//          *
//          * @param x an x coordinate from GLUT.
//          * @param y an y coordinate from GLUT.
//          */
//         virtual void pushMotionInput(int x, int y);

//         /**
//          * Pushes a passive mouse motion input. It should be called in the
//          * glutPassiveMotionFunc call back function.
//          *
//          * @param x an x coordinate from GLUT.
//          * @param y an y coordinate from GLUT.
//          */
//         virtual void pushPassiveMotionInput(int x, int y);

//         /**
//          * Polls all input. It exists for input driver compatibility. If you
//          * only use GLUT and plan sticking with GLUT you can safely ignore this
//          * function as it in the GLUT case does nothing.
//          */
//         virtual void _pollInput() { }

//         // Inherited from Input

//         virtual bool isKeyQueueEmpty();

//         virtual KeyInput dequeueKeyInput();

//         virtual bool isMouseQueueEmpty();

//         virtual MouseInput dequeueMouseInput();

//     protected:
//         /**
//          * Converts a GLUT key to a Key object.
//          *
//          * @param key a GLUT key.
//          * @return a Key object.
//          */
//         Key convertKeyCharacter(unsigned char key);

//         /**
//          * Converts a special GLUT key to a Key object.
//          *
//          * @param key a GLUT key.
//          * @return a Key object.
//          */
//         Key convertSpecialCharacter(unsigned char key);

//         /**
//          * Converts a mouse button from GLUT to a GUI mouse button
//          * representation.
//          *
//          * @param button a GLUT mouse button.
//          * @return a mouse button.
//          */
//         int convertMouseButton(int button);

//         std::queue<KeyInput> mKeyInputQueue;
//         std::queue<MouseInput> mMouseInputQueue;
//     };
// }

#endif // INCLUDE_FIFECHAN_GLUT_GLUTINPUT_HPP_
