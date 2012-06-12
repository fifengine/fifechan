#include "fifechan/sizeconstraint.hpp"
#include "fifechan/widgets/hbox.hpp"

namespace fcn
{
    HBox::HBox(fcn::SizeConstraint *sizeConstraint)
    :
    AxisLayoutContainer(sizeConstraint)
    {
    }
    
    HBox::~HBox()
    {
    }
    
    void HBox::relayoutChildren()
    {
        std::list<Widget*>::iterator currChild(mChildren.begin());
        std::list<Widget*>::iterator endChildren(mChildren.end());
        
        int width = 0;
        for(; currChild != endChildren; ++currChild)
        {
            Widget* child = (*currChild);
            
            if(child->isVisible())
            {
                child->setPosition(width, mChildrenOffset);
                width += child->getWidth() + mPadding;
            }
        }
    }
    
    void HBox::adjustSize()
    {
        int newWidth = getVisibleChildrenWidth() + getTotalPadding();
        
        int newHeight = getMaxVisibleChildHeight() + mChildrenOffset;
        
        setSize(newWidth, newHeight);
    }
    
    void HBox::layoutAddedChild(Widget* added)
    {
        int childrenWidth = getVisibleChildrenWidth() - added->getWidth();
        int totalPadding = getTotalPadding();
        int newChildPosition = childrenWidth + totalPadding;
        
        std::cerr << "New child at " << newChildPosition << ", " << mChildrenOffset << std::endl;
        
        added->setPosition(newChildPosition, mChildrenOffset);
    }

    void HBox::fitChildrenToAxis()
    {
        std::list<Widget*>::iterator currChild(mChildren.begin());
        std::list<Widget*>::iterator endChildren(mChildren.end());
        
        int containerHeight = getHeight() - mChildrenOffset;
        
        for(; currChild != endChildren; ++currChild)
        {
            (*currChild)->setHeight(containerHeight);
        }
    }
    
    int HBox::getVisibleChildrenWidth() const
    {
        std::list<Widget*>::const_iterator currChild(mChildren.begin());
        std::list<Widget*>::const_iterator endChildren(mChildren.end());
        
        int width = 0;
        
        for(; currChild != endChildren; ++currChild)
        {
            Widget* child = (*currChild);
            
            if(child->isVisible())
            {
                width += child->getWidth();
            }
        }
        
        return width;
    }
    
    int HBox::getMaxVisibleChildHeight() const
    {
        std::list<Widget*>::const_iterator currChild(mChildren.begin());
        std::list<Widget*>::const_iterator endChildren(mChildren.end());
        
        int max = 0;
        
        for(; currChild != endChildren; ++currChild)
        {
            Widget* child = (*currChild);
            
            if(child->isVisible())
            {
                max = std::max(child->getHeight(), max);
            }
        }
        
        return max;
    }
};