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
    virtual ~MouseListener() { }

    /**
     * 
     */
    virtual void mouseIn() { }

    /**
     * 
     */
    virtual void mouseOut() { }

    /**
     * 
     */
    virtual void mousePress(int x, int y, int button) { }

    /**
     * 
     */
    virtual void mouseRelease(int x, int y, int button) { }

    /**
     * 
     */
    virtual void mouseWheelUp(int x, int y) { }

    /**
     * 
     */
    virtual void mouseWheelDown(int x, int y) { }

    /**
     * 
     */
    virtual void mouseMotion(int x, int y) { }

    /**
     * 
     */
    virtual void mouseClick(int x, int y, int button, int count) { }
    
  }; // end MouseListener

} // end gcn

#endif // end GCN_MOUSELISTENER_HPP
