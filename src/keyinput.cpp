#include "guichan/keyinput.hpp"

namespace gcn
{

  /**
   * 
   */
  KeyInput::KeyInput(const Key& key, int type)
  {
    mKey = key;
    mType = type;
  }
  
  /**
   * 
   */
  void KeyInput::setType(int type)
  {
    mType = type;
  }

  /**
   * 
   */
  int KeyInput::getType()
  {
    return mType;
  }

  /**
   * 
   */
  void KeyInput::setKey(const Key& key)
  {
    mKey = key;
  }

  /**
   * 
   */
  const Key& KeyInput::getKey()
  {
    return mKey;
  }
  
} //end gcn

