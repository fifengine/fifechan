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
    /**
     * 
     */
    Key(char character);
    /**
     * 
     */
    Key(int scancode);
    /**
     * 
     */
    char getCharacter();
    /**
     * 
     */
    int getScanCode();

  protected:
    char mCharacter;
    int mScanCode;
    
  }; // end Key

} // end gcn

#endif // end GCN_KEY_HPP
