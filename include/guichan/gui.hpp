#ifndef GCN_GUI_HPP
#define GCN_GUI_HPP

#include <string>

namespace gcn
{

  class Gui
  {
  public:
    /**
     * 
     */
    void setTop(const Container* top);
    /**
     * 
     */
    Container* getTop();
    /**
     * 
     */
    
    void setGraphics(const Graphics* graphics);
    /**
     *  
     */
    Graphics* getGraphics();


    /**
     *  
     */
    void setInput(const Input* input);
    /**
     *  
     */
    Input* getInput();

    void addMouseType(const MouseType* mousetype);
    /**
     *  
     */
    void lockMouseType();
    /**
     *  
     */
    void unlockMouseType();
    /**
     *  
     */
    void removeMouseType(std::string mouseType);

    /**
     *  
     */
    void logic();
    /**
     *  
     */
    void draw();
    
  protected:
    Widget* mFocusedWidget;
    
  }; // end class Gui
  
} // end gcn

#endif // end GCN_GUI_HPP
