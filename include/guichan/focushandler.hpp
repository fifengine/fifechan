#ifndef GCN_FOCUSHANDLER_HPP
#define GCN_FOCUSHANDLER_HPP

#include <vector>

namespace gcn
{

  /**
   * 
   */
  class FocusHandler
  {
  public:
    /**
     * 
     */
    void requestFocus(Widget* widget);
    /**
     * 
     */
    Widget* getFocused();
    /**
     * 
     */
    void focusNext();
    /**
     * 
     */
    void focusPrevious();
    /**
     * 
     */
    bool hasFocus(Widget* widget);
    
  protected:
    std::vector<Widget*> mWidgets;
    Widget* mFocusedWidget;
    Widget* mPreviousFocusedWidget;
    
  }; // FocusHandler
  
} // end gcn

#endif // end GCN_FOCUSHANDLER_HPP
