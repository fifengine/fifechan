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