#include <allegro.h>

#include "guichan/allegro/allegroinput.hpp"

namespace gcn
{
  AllegroInput::AllegroInput()
  {
    mMouseX = 0;
    mMouseX = 0;
    mMouseMotion = false;
  }
  
  /**
   *
   */
  bool AllegroInput::isKeyQueueEmpty()
  {
    return mKeyInputQueue.empty();
  }
  
  /**
   *
   */
  KeyInput AllegroInput::dequeueKeyInput()
  {
    KeyInput keyInput;
    
    if (mKeyInputQueue.empty())
    {
      // TODO
      // Add exception instead of return statement
      keyInput.setType(KeyInput::EMPTY);
      return keyInput;
    }
    
    keyInput = mKeyInputQueue.front();
    mKeyInputQueue.pop();

    return keyInput;
    
  } // end dequeueKeyInput
  
  bool AllegroInput::isMouseQueueEmpty()
  {
    return mMouseInputQueue.empty();
  }
  
  /**
   *
   */
  MouseInput AllegroInput::dequeueMouseInput()
  {
    MouseInput mouseInput;
    
    if (mMouseInputQueue.empty())
    {
      // TODO
      // Add exception instead of return statement
      mouseInput.setType(MouseInput::EMPTY);
      return mouseInput;
    }
    
    mouseInput = mMouseInputQueue.front();
    mMouseInputQueue.pop();

    return mouseInput;
    
  } // end dequeueMouseInput
  
  /**
   *
   */
  int AllegroInput::getMouseX()
  {
    return mMouseX;
  }

  /**
   *
   */
  int AllegroInput::getMouseY()
  {
    return mMouseY;
  }

  /**
   *
   */
  void AllegroInput::getMousePosition(int& x, int& y)
  {
    x = mMouseX;
    y = mMouseY;
  }

  /**
   *
   */
  void AllegroInput::pollInput()
  {
    mMouseMotion = false;

    /* Poll keyboard and mouse if needed */
    if (keyboard_needs_poll())
    {
      poll_keyboard();
    }

    if (mouse_needs_poll())
    {
      poll_mouse();
    }

    /* Check for mouse movement */
    if (mouse_x != mMouseX && mouse_y != mMouseY)
    {
      mMouseX = mouse_x;
      mMouseY = mouse_y;
      mMouseMotion = true;
    }

    /* Handle mouse wheel movement */
    int mz = mouse_z;
    while (mz != 0)
    {
      if (mz < 0)
      {
        mMouseInputQueue.push(MouseInput(MouseInput::WHEEL_DOWN, MouseInput::PRESS));
        mMouseInputQueue.push(MouseInput(MouseInput::WHEEL_DOWN, MouseInput::RELEASE));
        mz++;
      }
      else
      {
        mMouseInputQueue.push(MouseInput(MouseInput::WHEEL_UP, MouseInput::PRESS));
        mMouseInputQueue.push(MouseInput(MouseInput::WHEEL_UP, MouseInput::RELEASE));
        mz--;        
      }
    }
    position_mouse_z(0);

    /* Handle mouse buttons */
    int mb = mouse_b;
    if (mb & 1 && !(mOldMouseButtons & 1))
    {
      mMouseInputQueue.push(MouseInput(MouseInput::LEFT, MouseInput::PRESS));
    }
    if (!(mb & 1) && mOldMouseButtons & 1)
    {
      mMouseInputQueue.push(MouseInput(MouseInput::LEFT, MouseInput::RELEASE));
    }
    if (mb & 2 && !(mOldMouseButtons & 2))
    {
      mMouseInputQueue.push(MouseInput(MouseInput::RIGHT, MouseInput::PRESS));
    }
    if (!(mb & 2) && mOldMouseButtons & 2)
    {
      mMouseInputQueue.push(MouseInput(MouseInput::RIGHT, MouseInput::RELEASE));
    }
    if (mb & 4 && !(mOldMouseButtons & 4))
    {
      mMouseInputQueue.push(MouseInput(MouseInput::MIDDLE, MouseInput::PRESS));
    }
    if (!(mb & 4) && mOldMouseButtons & 4)
    {
      mMouseInputQueue.push(MouseInput(MouseInput::MIDDLE, MouseInput::RELEASE));
    }
    mOldMouseButtons = mb;
    
    /* Check for keypresses */
    while (keypressed())
    {
      int k = readkey();
      int scancode;
      unsigned char ascii;
      
      //unicode = ureadkey(&scancode);
      //ascii = convertKeyCharacter(unicode);

      ascii = k & 0xFF;
      scancode = k >> 8;

      Key key = Key(ascii, scancode);
            
      mKeyInputQueue.push(KeyInput(key, KeyInput::PRESS));
    }
  }
  
  /**
   * 
   */
  bool AllegroInput::isMouseMoved()
  {
    return mMouseMotion;
  }

  /**
   *
   */
  unsigned char AllegroInput::convertKeyCharacter(unsigned int unicode)
  {
    if (unicode > 255)
    {
      return 0;
    }
    else
    {
      return (unsigned char)unicode;
    }    
  }

} // end gcn
