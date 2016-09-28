/***************************************************************************
 *   Copyright (c) 2016 by the fifechan team                               *
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

#include "fifechan/sizeconstraint.hpp"
#include "fifechan/widgets/hbox.hpp"

#include <algorithm>

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
        std::list<Widget*>::iterator currChild(mAddedOrder.begin());
        std::list<Widget*>::iterator endChildren(mAddedOrder.end());
        
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
            
        added->setPosition(newChildPosition, mChildrenOffset);
    }

    void HBox::fitChildrenToAxis()
    {
        std::list<Widget*>::iterator currChild(mChildren.begin());
        std::list<Widget*>::iterator endChildren(mChildren.end());
        
        int containerHeight = getHeight() - mChildrenOffset;
        
        for(; currChild != endChildren; ++currChild)
        {
            (*currChild)->_setHeightWithoutNotifying(containerHeight);
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
        std::list<Widget*>::const_iterator currChild(mAddedOrder.begin());
        std::list<Widget*>::const_iterator endChildren(mAddedOrder.end());
        
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