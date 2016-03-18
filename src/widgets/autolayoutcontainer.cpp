#include "fifechan/sizeconstraint.hpp"
#include "fifechan/widgets/autolayoutcontainer.hpp"

#include <algorithm>

namespace fcn
{
    AutoLayoutContainer::AutoLayoutContainer()
    {
        addContainerListener(this);
    }
    
    AutoLayoutContainer::~AutoLayoutContainer()
    {
        std::list<Widget*>::iterator currChild(mChildren.begin());
        std::list<Widget*>::iterator endChildren(mChildren.end());
        
        for(; currChild != endChildren; ++currChild)
        {
            (*currChild)->removeWidgetListener(this);
        }
    }
    
    void AutoLayoutContainer::widgetAdded(const ContainerEvent& containerEvent)
    {
        if(containerEvent.getContainer() == this)
        {
            Widget *added = containerEvent.getSource(); 
            added->addWidgetListener(this);
        }
    }
    
    void AutoLayoutContainer::widgetRemoved(const ContainerEvent& containerEvent)
    {
        if(containerEvent.getContainer() == this)
        {
            Widget *removed = containerEvent.getSource();
            removed->removeWidgetListener(this);
        }
    }
};