#ifndef GCN_INPUT_HPP
#define GCN_INPUT_HPP

namespace gcn
{
  /**
   * 
   */
  enum InputType
  {
    EMPTY,
    KEY_DOWN,
    KEY_TYPE,
    KEY_RELEASE,
    MOUSE_DOWN,
    MOUSE_RELEASE,
    MOUSE_WHEEL_UP,
    MOUSE_WHEEL_DOWN

  }; // end InputType
  
  /**
   * 
   */
  class Input
  {
  public:
    /**
     * 
     */
    virtual ~Input();
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
    virtual const InputType& pollKeyInput(Key& key) = 0;
    /**
     * 
     */
    virtual const InputType& pollMouseInput(int& x, int& y, int& button) = 0;
    
  }; // end Input
  
} // end gcn

#endif // end GCN_INPUT_HPP
