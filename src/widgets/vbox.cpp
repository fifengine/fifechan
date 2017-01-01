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
 
#include "fifechan/sizeconstraint.hpp"
#include "fifechan/widgets/vbox.hpp"

#include <iostream>
#include <algorithm>

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
        std::list<Widget*>::iterator currChild(mAddedOrder.begin());
        std::list<Widget*>::iterator endChildren(mAddedOrder.end());
        
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
        std::list<Widget*>::const_iterator currChild(mAddedOrder.begin());
        std::list<Widget*>::const_iterator endChildren(mAddedOrder.end());
        
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