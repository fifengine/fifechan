#include "guichan/mouseinput.hpp"

namespace gcn
{
  /**
   * 
   */
  MouseInput::MouseInput(int button, int type)
  {
    mType = type;
    mButton = button;
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
  int MouseInput::getType()
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
  int MouseInput::getButton()
  {
    return mButton;
  }

} // end gcn
