#ifndef GCN_ALLEGROINPUT_HPP
#define GCN_ALLEGROINPUT_HPP

#include <queue>

#include "guichan/input.hpp"
#include "guichan/key.hpp"
#include "guichan/keyinput.hpp"
#include "guichan/mouseinput.hpp"

namespace gcn
{
  /**
   * 
   */
  class AllegroInput
  {
  public:

    /**
     *
     */
    AllegroInput();

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

  protected:
    unsigned char convertKeyCharacter(unsigned int unicode);
    std::queue<KeyInput> mKeyInputQueue;
    std::queue<MouseInput> mMouseInputQueue;
    int mMouseX;
    int mMouseY;
    bool mMouseMotion;

    int mOldMouseButtons;
    
  }; // end Input
  
} // end gcn

#endif // end GCN_ALLEGROINPUT_HPP
