#include "guichan/key.hpp"

namespace gcn
{

  /**
   * 
   */
  Key::Key(unsigned char ascii)
  {
    mAscii = ascii;
  }

  /**
   * 
   */
  Key::Key(int scancode)
  {
    mScancode = scancode;
  }

  /**
   * 
   */
  void Key::setAscii(unsigned char ascii)
  {
    mAscii = ascii;
  }
  
  /**
   * 
   */
  char Key::getAscii()
  {
    return mAscii;
  }

  /**
   * 
   */
  void Key::setScancode(int scancode)
  {
    mScancode = scancode;
  }
  
  /**
   * 
   */
  int Key::getScancode()
  {
    return mScancode;
  }

  /**
   * 
   */
  bool Key::isCharacter()
  {
    return (mAscii >=32 && mAscii <= 126)
      || (mAscii >= 162 && mAscii <= 255)
      || (mAscii == 9);
  }
  
  /**
   * 
   */
  bool Key::isNumber()
  {
    return mAscii >= 48 && mAscii <= 57;
  }
  
  /**
   * 
   */
  bool Key::isLetter()
  {
    return ((mAscii >= 65 && mAscii <= 90)
            || (mAscii >= 97 && mAscii <= 122)
            || (mAscii >= 192 && mAscii <= 255)
            && (mAscii != 215) && (mAscii != 247));
  }

} // end gcn
