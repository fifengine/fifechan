#include "fifechan/sizeconstraint.hpp"
#include "fifechan/widgets/vbox.hpp"

#include <iostream>

static const int DEFAULT_PADDING = 2;
static const int DEFAULT_CHILDREN_OFFSET = 1;

namespace fcn
{
    VBox::VBox(SizeConstraint* sizeConstraint)
    :
    AutoLayoutContainer(sizeConstraint),
    mPadding(DEFAULT_PADDING),
    mChildrenOffset(DEFAULT_CHILDREN_OFFSET)
    {
    }
    
    VBox::~VBox()
    {
    }
    
    void VBox::widgetAdded(const ContainerEvent& containerEvent)
    {
        AutoLayoutContainer::widgetAdded(containerEvent);
        
        if(containerEvent.getContainer() == this)
        {
            Widget *added = containerEvent.getSource();
            
            layoutAddedChild(added);
            
            adjustSize();
            
            fitChildrenToWidth();
        }
    }
    
    void VBox::widgetRemoved(const ContainerEvent& containerEvent)
    {
        AutoLayoutContainer::widgetRemoved(containerEvent);
        
        if(containerEvent.getContainer() == this)
        {  
            relayout();
        }
    }
    
    void VBox::widgetResized(const Event& widgetEvent)
    {
    }
    
    void VBox::widgetShown(const Event& widgetEvent)
    {
        relayoutChildren();
    }
    
    void VBox::widgetHidden(const Event& widgetEvent)
    {
        relayoutChildren();
    }
    
    void VBox::relayout()
    {
        relayoutChildren();
        adjustSize();
        fitChildrenToWidth();
    }
    
    void VBox::relayoutChildren()
    {
        std::list<Widget*>::iterator currChild(mChildren.begin());
        std::list<Widget*>::iterator endChildren(mChildren.end());
        
        int height = mPadding;
        
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
        int newWidth = getWidestVisibleChild()->getWidth();

        int newHeight = getVisibleChildrenHeight() + mPadding * getVisibleChildrenCount();
        
        setSize(newWidth, newHeight);
    }
    
    void VBox::layoutAddedChild(Widget *added)
    {
        int childrenHeight = getVisibleChildrenHeight() - added->getHeight();
        int totalPadding = getVisibleChildrenCount() * mPadding;
        int newChildHeight = childrenHeight + totalPadding;
        
        added->setPosition(mChildrenOffset, newChildHeight);
    }
    
    void VBox::fitChildrenToWidth()
    {
        std::list<Widget*>::iterator currChild(mChildren.begin());
        std::list<Widget*>::iterator endChildren(mChildren.end());
        
        int containerWidth = getWidth();
        
        for(; currChild != endChildren; ++currChild)
        {
            (*currChild)->setWidth(containerWidth);
        }
    }
    
    Widget* VBox::getWidestVisibleChild() const
    {
        std::list<Widget*>::const_iterator currChild(mChildren.begin());
        std::list<Widget*>::const_iterator endChildren(mChildren.end());
        
        int max = 0;
        Widget* widest = NULL;
        
        for(; currChild != endChildren; ++currChild)
        {
            Widget *child = (*currChild);
            
            if(child->isVisible())
            {
                max = std::max(child->getWidth(), max);
                widest = child;
            }
        }
        
        return widest;
    }
};