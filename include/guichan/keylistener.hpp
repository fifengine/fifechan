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
    virtual ~KeyListener();
    
    /**
     * 
     */
    virtual void keyPressEvent(const std::string& eventId, const Key& key) = 0;
    /**
     * 
     */
    virtual void keyReleaseEvent(const std::string& eventId, const Key& key) = 0;
    /**
     * 
     */
    virtual void keyTypeEvent(const std::string& eventId, const Key& key) = 0;

  }; // end KeyListener

} // end gcn

#endif // end GCN_KEYLISTENER_HPP
