#include "guichan/sdl/sdlinput.hpp"

namespace gcn
{
  void SDLInput::init()
  {
    SDL_EnableUNICODE(1);
    SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);
  }
  
  /**
   *
   */
  bool SDLInput::isKeyQueueEmpty()
  {
    return mKeyInputQueue.empty();
  }
  
  /**
   *
   */
  KeyInput SDLInput::dequeueKeyInput()
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

  bool SDLInput::isMouseQueueEmpty()
  {
    return mMouseInputQueue.empty();
  }

  /**
   *
   */
  MouseInput SDLInput::dequeueMouseInput()
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
  int SDLInput::getMouseX()
  {
    return mMouseX;
  }

  /**
   *
   */
  int SDLInput::getMouseY()
  {
    return mMouseY;
  }

  /**
   *
   */
  void SDLInput::getMousePosition(int& x, int& y)
  {
    x = mMouseX;
    y = mMouseY;
  }

  void SDLInput::pollInput()
  {
    mMouseMotion = false;
  }
  
  /**
   * 
   */
  bool SDLInput::isMouseMoved()
  {
    return mMouseMotion;
  }

  void SDLInput::pushInput(SDL_Event event)
  {
    Key key;
    KeyInput keyInput;
    MouseInput mouseInput;
    
    switch (event.type)
    {
      case SDL_KEYDOWN:
        key.setAscii(convertKeyCharacter(event.key.keysym.unicode));
        keyInput.setKey(key);
        keyInput.setType(KeyInput::PRESS);        
        mKeyInputQueue.push(keyInput);
        break;
      case SDL_KEYUP:
        key.setAscii(convertKeyCharacter(event.key.keysym.unicode));
        keyInput.setKey(key);
        keyInput.setType(KeyInput::RELEASE);
        mKeyInputQueue.push(keyInput);
        break;
      case SDL_MOUSEBUTTONDOWN:
        mouseInput.setButton(convertMouseButton(event.button.button));
        mouseInput.setType(MouseInput::PRESS);
        mMouseInputQueue.push(mouseInput);
        break;
      case SDL_MOUSEBUTTONUP:
        mouseInput.setButton(convertMouseButton(event.button.button));
        mouseInput.setType(MouseInput::RELEASE);
        mMouseInputQueue.push(mouseInput);
        break;
      case SDL_MOUSEMOTION:
        mMouseX = event.button.x;
        mMouseY = event.button.y;
        mMouseMotion = true;
        break;
    } // end switch
  }
  
  int SDLInput::convertMouseButton(int button)
  {
    switch (button)
    {
      case SDL_BUTTON_LEFT:
        return MouseInput::LEFT;
        break;
      case SDL_BUTTON_RIGHT:
        return MouseInput::RIGHT;
        break;
      case SDL_BUTTON_MIDDLE:
        return MouseInput::MIDDLE;
        break;
      case SDL_BUTTON_WHEELUP:
        return MouseInput::WHEEL_UP;
        break;
      case SDL_BUTTON_WHEELDOWN:
        return MouseInput::WHEEL_DOWN;
        break;
    }
  }

  unsigned char SDLInput::convertKeyCharacter(unsigned int unicode)
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
