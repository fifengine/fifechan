#ifndef GCN_KEY_HPP
#define GCN_KEY_HPP

#include <string>

namespace gcn
{
  /**
   * 
   */
  class Key
  {
  public:
    Key(){};
    
    /**
     * 
     */
    Key(unsigned char ascii, int scancode);

    /**
     * 
     */
    void setAscii(unsigned char ascii);

    /**
     * 
     */
    char getAscii();

    /**
     * 
     */
    void setScancode(int scancode);

    /**
     * 
     */
    int getScancode();

    /**
     * 
     */
    bool isCharacter();

    /**
     *
     */
    bool isNumber();
    
    /**
     *
     */
    bool isLetter();
 
  protected:
    unsigned char mAscii;
    int mScancode;

  }; // end Key

} // end gcn

#endif // end GCN_KEY_HPP
