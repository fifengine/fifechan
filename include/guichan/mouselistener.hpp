#ifndef GCN_MOUSELISTENER_HPP
#define GCN_MOUSELISTENER_HPP

#include <string>

namespace gcn
{
  /**
   * 
   */
  class MouseListener
  {
  public:
    /**
     * 
     */
    virtual ~MouseListener();
    /**
     * 
     */
    virtual void mouseInEvent(const std::string& eventId) = 0;
    /**
     * 
     */
    virtual void mouseOutEvent(const std::string& eventId) = 0;
    /**
     * 
     */
    virtual void mousePressEvent(const std::string& eventId, int x,
                                 int y, int button) = 0;
    /**
     * 
     */
    virtual void mouseReleaseEvent(const std::string& eventId, int x,
                                   int y, int button) = 0;
    /**
     * 
     */
    virtual void mouseWheelUpEvent(const std::string& eventId, int x, int y) = 0;
    /**
     * 
     */
    virtual void mouseWheelDownEvent(const std::string& eventId, int x, int y) = 0;
    /**
     * 
     */
    virtual void mouseMotionEvent(const std::string& eventId, int x, int y) = 0;

    /**
     * 
     */
    virtual void mouseClickEvent(const std::string& eventId, int x, int y, int button, int count) = 0;
    
  }; // end MouseListener

} // end gcn

#endif // end GCN_MOUSELISTENER_HPP
