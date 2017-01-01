/***************************************************************************
 *   Copyright (c) 2017 by the fifechan team                               *
 *   https://github.com/fifengine/fifechan                                 *
 *   This file is part of fifechan.                                        *
 *                                                                         *
 *   fifechan is free software; you can redistribute it and/or             *
 *   modify it under the terms of the GNU Lesser General Public            *
 *   License as published by the Free Software Foundation; either          *
 *   version 2.1 of the License, or (at your option) any later version.    *
 *                                                                         *
 *   This library is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU     *
 *   Lesser General Public License for more details.                       *
 *                                                                         *
 *   You should have received a copy of the GNU Lesser General Public      *
 *   License along with this library; if not, write to the                 *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA          *
 ***************************************************************************/

/*
 * For comments regarding functions please see the header file.
 */

#include "fifechan/exception.hpp"
#include "fifechan/widgets/axislayoutcontainer.hpp"

#include <algorithm>
#include <functional>

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
         
            mAddedOrder.push_back(added);
            
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
        AutoLayoutContainer::widgetRemoved(containerEvent);
        
        if(containerEvent.getContainer() == this)
        {
            Widget* removed = containerEvent.getSource();
            
            mAddedOrder.remove(removed);
            
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