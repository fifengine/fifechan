#ifndef GCN_KEYLISTENER_HPP
#define GCN_KEYLISTENER_HPP

#include <string>

namespace gcn
{
  /**
   * A KeyListener listens for key events on a widget. When a
   * widget recives a key event, the corresponding function
   * in all its key listeners will be called. Only focused
   * widgets will generate key events.
   *
   * None of the functions in this class does anything at all,
   * it is up to you to overload them.
   *
   * @see Widget::addKeyListener
   */
  class KeyListener
  {
  public:

    /**
     * Destructor
     */
    virtual ~KeyListener() { }
    
    /**
     * This function is called if a key is pressed when
     * the widget has keyboard focus.
     *
     * If a key is held down the widget will generate multiple
     * key presses.
     *
     * @param key the key pressed
     * @see Key
     */
    virtual void keyPress(const Key& key) { }

    /**
     * This function is called if a key is released when
     * the widget has keyboard focus.
     *
     * @param key the key released
     * @see Key
     */
    virtual void keyRelease(const Key& key) { }
    
  }; // end KeyListener

} // end gcn

#endif // end GCN_KEYLISTENER_HPP
