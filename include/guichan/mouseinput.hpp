#ifndef GCN_MOUSEINPUT_HPP
#define GCN_MOUSEINPUT_HPP

namespace gcn
{
  /**
   * 
   */
  class MouseInput
  {
  public:
    /**
     * 
     */
    MouseInput(){};

    /**
     * 
     */
    MouseInput(int button, int type, int mousex, int mousey, int timeStamp);

    /**
     * 
     */
    void setType(int type);

    /**
     * 
     */
    int getType() const;

    /**
     * 
     */
    void setButton(int button);

    /**
     * 
     */
    int getButton() const;

    /**
     *
     */
    int getTimeStamp() const;

    /**
     *
     */
    void setTimeStamp(int timeStamp);

    /**
     * 
     */
    // TODO
    // Remove EMPTY
    const static int EMPTY = 0;
    const static int LEFT = 1;
    const static int RIGHT = 2;
    const static int MIDDLE = 3;
    const static int WHEEL_UP = 4;
    const static int WHEEL_DOWN = 5;
    const static int PRESS = 6;
    const static int RELEASE = 7;
    const static int MOTION = 8;
    
    int x, y;
  protected:
    int mType;
    int mButton;
    int mTimeStamp;
    
  }; // end MouseInput

} // end gcn

#endif // end GCN_MOUSEINPUT_HPP
