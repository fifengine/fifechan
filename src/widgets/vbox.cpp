#include "fifechan/sizeconstraint.hpp"
#include "fifechan/widgets/vbox.hpp"

#include <iostream>

namespace fcn
{
    VBox::VBox(SizeConstraint* sizeConstraint)
    :
    AxisLayoutContainer(sizeConstraint)
    {
    }
    
    VBox::~VBox()
    {
    }
    
    void VBox::relayoutChildren()
    {
        std::list<Widget*>::iterator currChild(mChildren.begin());
        std::list<Widget*>::iterator endChildren(mChildren.end());
        
        int height = 0;
        
        for(; currChild != endChildren; ++currChild)
        {
            Widget* child = (*currChild);
            
            if(child->isVisible())
            {
                child->setPosition(mChildrenOffset, height);
                height += child->getHeight() + mPadding;
            }
        }
    }
    
    void VBox::adjustSize()
    {
        int newWidth = getMaxVisibleChildWidth() + mChildrenOffset;

        int newHeight = getVisibleChildrenHeight() + getTotalPadding();
        
        setSize(newWidth, newHeight);
    }
    
    void VBox::layoutAddedChild(Widget *added)
    {
        int childrenHeight = getVisibleChildrenHeight() - added->getHeight();
        int totalPadding = getTotalPadding();
        int newChildPosition = childrenHeight + totalPadding;
        
        added->setPosition(mChildrenOffset, newChildPosition);
    }
    
    void VBox::fitChildrenToAxis()
    {
        std::list<Widget*>::iterator currChild(mChildren.begin());
        std::list<Widget*>::iterator endChildren(mChildren.end());
        
        int containerWidth = getWidth() - mChildrenOffset;
        
        for(; currChild != endChildren; ++currChild)
        {
            (*currChild)->_setWidthWithoutNotifying(containerWidth);
        }
    }
    
    int VBox::getVisibleChildrenHeight() const 
    {
        std::list<Widget*>::const_iterator currChild(mChildren.begin());
        std::list<Widget*>::const_iterator endChildren(mChildren.end());
        
        int height = 0;
        
        for(; currChild != endChildren; ++currChild)
        {
            Widget *child = (*currChild);
            
            if(child->isVisible())
            {
                height += child->getHeight();
            }
        }
        
        return height;    
    }
    
    int VBox:: getMaxVisibleChildWidth() const
    {
        std::list<Widget*>::const_iterator currChild(mChildren.begin());
        std::list<Widget*>::const_iterator endChildren(mChildren.end());
        
        int max = 0;
        
        for(; currChild != endChildren; ++currChild)
        {
            Widget *child = (*currChild);
            
            if(child->isVisible())
            {
                max = std::max(child->getWidth(), max);
            }
        }
        
        return max;
    }
};