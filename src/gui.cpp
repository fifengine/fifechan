#include "guichan/gui.hpp"
#include "guichan/focushandler.hpp"

namespace gcn
{

  Gui::Gui()
  {
    mTop = 0;
    mInput = 0;
    mGraphics = 0;
    mFocusHandler = new FocusHandler();
  }

  Gui::~Gui()
  {
    delete mFocusHandler;
  }
  
  /**
   * 
   */
  void Gui::setTop(Widget* top)
  {
    if (mTop)
    {
      mTop->_setFocusHandler(0);
    }
    if (top)
    {
      top->_setFocusHandler(mFocusHandler);
    }
    
    mTop = top;
  }

  /**
   * 
   */
  Widget* Gui::getTop() const
  {
    return mTop;
  }

  /**
   * 
   */
  void Gui::setGraphics(Graphics* graphics)
  {
    mGraphics = graphics;
  }

  /**
   *  
   */
  Graphics* Gui::getGraphics() const
  {
    return mGraphics;
  }

  /**
   *  
   */
  void Gui::setInput(Input* input)
  {
    mInput = input;
  }
  
  /**
   *  
   */
  Input* Gui::getInput() const
  {
    return mInput;
  }

  void Gui::logic()
  {
    if (!mTop)
    {
      //TODO Error error oh my god ERROR!
      return;
    }
    if(mInput)
    {
      //TODO: Handle input
    }
    mTop->logic();
  }

  void Gui::draw()
  {
    if (!mTop)
    {
      //TODO Error error oh my god ERROR!
      return;
    }
    if (!mGraphics)
    {
      //TODO Error error oh my god ERROR!
      return;
    }

    mGraphics->pushClipArea(mTop->getDimension());
    mTop->draw(mGraphics);
    mGraphics->popClipArea();
  }
  
} // end gcn
