#ifndef GCN_GUI_HPP
#define GCN_GUI_HPP

#include <string>
#include "guichan/container.hpp"
#include "guichan/input.hpp"
#include "guichan/mousetype.hpp"
#include "guichan/widget.hpp"

namespace gcn
{

  class Gui
  {
  public:

    /**
     * @todo Fix mousemessage to focused widget and offset.
     */
    Gui();

    /**
     * 
     */
    ~Gui();

    /**
     * 
     */
    void setTop(Widget* top);

    /**
     * 
     */
    Widget* getTop() const;

    /**
     * 
     */
    void setGraphics(Graphics* graphics);

    /**
     *  
     */
    Graphics* getGraphics() const;

    /**
     *  
     */
    void setInput(Input* input);

    /**
     *  
     */
    Input* getInput() const;
    
    /**
     *  
     */
    void logic();

    /**
     *  
     */
    void draw();
    
  protected:
    Widget* mTop;
    Graphics* mGraphics;
    Input* mInput;
    FocusHandler* mFocusHandler;
    
  }; // end class Gui
  
} // end gcn

#endif // end GCN_GUI_HPP
