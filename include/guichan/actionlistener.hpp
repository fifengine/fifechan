#ifndef GCN_ACTIONLISTENER_HPP
#define GCN_ACTIONLISTENER_HPP

#include <string>

namespace gcn
{
  /**
   * An ActionListener listens for action events from a widget.
   *
   * @see Widget::addActionListener
   */
  class ActionListener
  {
  public:

    /**
     * Destructor.
     */
    virtual ~ActionListener() { }
    
    /**
     * This function is called upon an action recieved from a widget.
     *
     * @param eventId the identifier of the widget.
     */
    virtual void action(const std::string& eventId) = 0;

  }; // end ActionListener

} // end gcn

#endif // end GCN_ACTIONLISTENER_HPP
