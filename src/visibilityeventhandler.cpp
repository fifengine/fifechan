#include "fifechan/visibilityeventhandler.hpp"

#include "fifechan/event.hpp"
#include "fifechan/gui.hpp"

#include <list>

namespace fcn
{
    VisibilityEventHandler::VisibilityEventHandler(Gui *gui)
    :
    mGui(gui)
    {
    }
    
    void VisibilityEventHandler::widgetHidden(const Event& e)
    {
        mGui->enqueueHiddenWidget(e.getSource());
    }
      
    void VisibilityEventHandler::widgetShown(const Event& e)
    {
        mGui->enqueueShownWidget(e.getSource());
    }    
};