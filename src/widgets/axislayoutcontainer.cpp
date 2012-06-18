#include "fifechan/exception.hpp"
#include "fifechan/widgets/axislayoutcontainer.hpp"

#include <algorithm>

static const int DEFAULT_PADDING = 2;
static const int DEFAULT_CHILDREN_OFFSET = 1;

namespace fcn
{
    AxisLayoutContainer::AxisLayoutContainer(SizeConstraint* sizeConstraint)
    :
    mPadding(DEFAULT_PADDING),
    mChildrenOffset(DEFAULT_CHILDREN_OFFSET)
    {
        setSizeConstraint(sizeConstraint);
    }
    
    AxisLayoutContainer::~AxisLayoutContainer()
    {
    }
   
    void AxisLayoutContainer::widgetAdded(const ContainerEvent& containerEvent)
    {
        AutoLayoutContainer::widgetAdded(containerEvent);
        
        if(containerEvent.getContainer() == this)
        {
            Widget* added = containerEvent.getSource();
         
            if(!added->hasSizeConstraint())
            {
                added->setSizeConstraint(mSizeConstraint->clone());
            }
            else
            {
                checkSizeConstraint(added->getSizeConstraint());
            }
            
            layoutAddedChild(added);
            adjustSize();
            fitChildrenToAxis();
        }
    }
    
    void AxisLayoutContainer::widgetRemoved(const ContainerEvent& containerEvent)
    {
        AutoLayoutContainer::widgetAdded(containerEvent);
        
        if(containerEvent.getContainer() == this)
        {
            relayout();
        }
    }
    
    void AxisLayoutContainer::widgetResized(const Event& event)
    {
        relayout();
    }
    
    void AxisLayoutContainer::widgetShown(const Event& event)
    {
        relayout();
    }
    
    void AxisLayoutContainer::widgetHidden(const Event& event)
    {
        relayout();
    }
    
    void AxisLayoutContainer::relayout()
    {
        relayoutChildren();
        adjustSize();
        fitChildrenToAxis();
    }
    
    void AxisLayoutContainer::setPadding(int padding)
    {
        mPadding = padding;
    }
    
    void AxisLayoutContainer::setChildrenOffset(int childrenOffset)
    {
        mChildrenOffset = childrenOffset;
    }
    
    void AxisLayoutContainer::checkSizeConstraint(SizeConstraint* checkAgainst)
    {
        int addedMaxWidth = checkAgainst->getMaxWidth();
        int addedMaxHeight = checkAgainst->getMaxHeight();
        
        SizeConstraint* mySizeConstraint = getSizeConstraint();
        int myMaxWidth = mySizeConstraint->getMaxWidth();
        int myMaxHeight = mySizeConstraint->getMaxHeight();
        
        if(addedMaxWidth > myMaxWidth || addedMaxHeight > myMaxHeight)
            throw FCN_EXCEPTION("Child widget cannot have a bigger size constraint than its axis auto-layouting parent in either width or height");        
    }
    
    int AxisLayoutContainer::getVisibleChildrenCount() const
    {
        return static_cast<int>(std::count_if(mChildren.begin(), mChildren.end(), std::mem_fun(&Widget::isVisible)));
    }
    
    int AxisLayoutContainer::getTotalPadding() const
    {
        int visibleChildren = getVisibleChildrenCount();
        
        return visibleChildren == 0 ? 0 : (visibleChildren - 1) * mPadding;
    }
};