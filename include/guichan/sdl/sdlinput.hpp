#ifndef GCN_SDLINPUT_HPP
#define GCN_SDLINPUT_HPP

#include <stack>
#include <queue>
#include <SDL/SDL.h>

#include "guichan/input.hpp"
#include "guichan/key.hpp"
#include "guichan/keyinput.hpp"
#include "guichan/mouseinput.hpp"

namespace gcn
{
  /**
   * 
   */
  class SDLInput
  {
  public:

    /**
     *
     */
    SDLInput();

    virtual void init();
    /**
     * 
     */
    virtual int getMouseX();

    /**
     * 
     */

    virtual int getMouseY();
    /**
     * 
     */

    virtual void getMousePosition(int& x, int& y);

    /**
     * 
     */
    virtual bool isKeyQueueEmpty();

    /**
     * 
     */
    virtual KeyInput dequeueKeyInput();

    /**
     * 
     */
    virtual bool isMouseQueueEmpty();

    /**
     * 
     */
    virtual MouseInput dequeueMouseInput();

    /**
     *
     */
    virtual void pollInput();

    /**
     * 
     */
    virtual bool isMouseMoved();

    /**
     * 
     */
    virtual void setConsumeSDLEventQueue(bool consume);

  protected:
    int convertMouseButton(int button);
    unsigned char convertKeyCharacter(unsigned int unicode);
    std::queue<KeyInput> mKeyInputQueue;
    std::queue<MouseInput> mMouseInputQueue;
    std::queue<SDL_Event> mEventQueue;
    int mMouseX;
    int mMouseY;
    bool mConsumeQueue;
    bool mMouseMotion;
    
  }; // end Input
  
} // end gcn

#endif // end GCN_SDLINPUT_HPP
