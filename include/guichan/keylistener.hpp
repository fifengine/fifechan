#ifndef GCN_KEYLISTENER_HPP
#define GCN_KEYLISTENER_HPP

#include <string>

namespace gcn
{
  /**
   * 
   */
  class KeyListener
  {
  public:

    /**
     * 
     */
    virtual ~KeyListener() { }
    
    /**
     * 
     */
    virtual void keyPress(const Key& key) { }

    /**
     * 
     */
    virtual void keyRelease(const Key& key) { }

    /**
     * 
     */
    virtual void keyType(const Key& key) { }

  }; // end KeyListener

} // end gcn

#endif // end GCN_KEYLISTENER_HPP
