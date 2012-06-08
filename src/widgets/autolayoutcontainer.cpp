#include "fifechan/sizeconstraint.hpp"
#include "fifechan/widgets/autolayoutcontainer.hpp"

#include <algorithm>

namespace fcn
{
    AutoLayoutContainer::AutoLayoutContainer(SizeConstraint *sizeConstraint)
    {
        addContainerListener(this);
        setSizeConstraint(sizeConstraint);
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
            
            if(!added->hasSizeConstraint())
            {
                added->setSizeConstraint(mSizeConstraint->clone());
            }
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
    
    int AutoLayoutContainer::getVisibleChildrenWidth() const
    {
        std::list<Widget*>::const_iterator currChild(mChildren.begin());
        std::list<Widget*>::const_iterator endChildren(mChildren.end());
        
        int total = 0;
        
        for(; currChild != endChildren; ++currChild)
        {
            Widget* child = (*currChild);
            
            if(child->isVisible())
            {
                total += child->getWidth();
            }
        }
        
        return total;
    }
    
    int AutoLayoutContainer::getVisibleChildrenHeight() const
    {
        std::list<Widget*>::const_iterator currChild(mChildren.begin());
        std::list<Widget*>::const_iterator endChildren(mChildren.end());
        
        int total = 0;
        
        for(; currChild != endChildren; ++currChild)
        {
            Widget* child = (*currChild);
            
            if(child->isVisible())
            {
                total += child->getHeight();
            }
        }
        
        return total;
    }
    
    int AutoLayoutContainer::getVisibleChildrenCount() const
    {
        return static_cast<int>(std::count_if(mChildren.begin(), mChildren.end(), std::mem_fun(&Widget::isVisible)));
    }
};