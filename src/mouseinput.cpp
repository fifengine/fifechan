#include "guichan/mouseinput.hpp"

namespace gcn
{
  /**
   * 
   */
  MouseInput::MouseInput(int button, int type, int mousex, int mousey, int timeStamp)
  {
    mType = type;
    mButton = button;
    mTimeStamp = timeStamp;
    x = mousex;
    y = mousey;
  }
  
  /**
   * 
   */
  void MouseInput::setType(int type)
  {
    mType = type;
  }

  /**
   * 
   */
  int MouseInput::getType() const
  {
    return mType;
  }

  /**
   * 
   */
  void MouseInput::setButton(int button)
  {
    mButton = button;
  }

  /**
   * 
   */
  int MouseInput::getButton() const
  {
    return mButton;
  }

  /**
   *
   */
  int MouseInput::getTimeStamp() const
  {
    return mTimeStamp;
  }

  /**
   *
   */
  void MouseInput::setTimeStamp(int timeStamp)
  {
    mTimeStamp = timeStamp;
  }

} // end gcn
