#ifndef GCN_KEYINPUT_HPP
#define GCN_KEYINPUT_HPP

#include "guichan/key.hpp"

namespace gcn
{
  /**
   * 
   */
  class KeyInput
  {
  public:
    /**
     * 
     */
    KeyInput(){};

    /**
     * 
     */
    KeyInput(const Key& key, int type);

    /**
     * 
     */
    void setType(int type);

    /**
     * 
     */
    int getType();

    /**
     * 
     */
    void setKey(const Key& key);

    /**
     * 
     */
    const Key& getKey();

    /**
     * 
     */
    const static int EMPTY = 0;
    const static int PRESS = 1;
    const static int RELEASE = 2;
    
  protected:
    Key mKey;
    int mType;
    int mButton;
  };

} // end gcn

#endif // end GCN_KEYINPUT_HPP
