#ifndef GCN_INPUT_HPP
#define GCN_INPUT_HPP

#include "guichan/keyinput.hpp"
#include "guichan/mouseinput.hpp"

namespace gcn
{
  /**
   * 
   */
  class Input
  {
  public:
    /**
     * 
     */
    virtual ~Input(){ }
    /**
     * 
     */
    virtual int getMouseX() = 0;
    /**
     * 
     */
    virtual int getMouseY() = 0;
    /**
     * 
     */
    virtual void getMousePosition(int& x, int& y) = 0;

    /**
     * 
     */
    virtual bool isKeyQueueEmpty() = 0;

    /**
     * 
     */
    virtual KeyInput dequeueKeyInput() = 0;

    /**
     * 
     */
    virtual bool isMouseQueueEmpty() = 0;

    /**
     * 
     */
    virtual MouseInput dequeueMouseInput() = 0;

    /**
     * 
     */
    virtual bool isMouseMoved() = 0;

    /**
     * 
     */
    virtual void pollInput() = 0;
    
  }; // end Input
  
} // end gcn

#endif // end GCN_INPUT_HPP
