#ifndef GCN_ACTIONLISTENER_HPP
#define GCN_ACTIONLISTENER_HPP

#include <string>

namespace gcn
{
  /**
   * 
   */
  class ActionListener
  {
  public:
    /**
     * 
     */
    virtual ~ActionListener();
    
    /**
     * 
     */
    virtual void actionEvent(const std::string& eventId) = 0;

  }; // end ActionListener

} // end gcn

#endif // end GCN_ACTIONLISTENER_HPP
