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
 
#ifndef FCN_AXISLAYOUTINGCONTAINER_HPP
#define FCN_AXISLAYOUTINGCONTAINER_HPP

#include "fifechan/sizeconstraint.hpp"
#include "fifechan/widgets/autolayoutcontainer.hpp"

namespace fcn
{

    class FCN_CORE_DECLSPEC AxisLayoutContainer : public AutoLayoutContainer
   {
   public:
    
        /**
         * Constructor enforces a size constraint for these types of containers.
         */
        AxisLayoutContainer(SizeConstraint *sizeConstraint);
    
        ~AxisLayoutContainer();
    
        /**
         * Called when a widget is added to a container. If this object is the one
         * that fired the event, then the container should be relayouted.
         * 
         * @param containerEvent The container event.
         */
        virtual void widgetAdded(const ContainerEvent& containerEvent);
    
        /**
         * Called when a widget is removed from a container. If this object is the one
         * that fired the event, then the container should be relayouted.
         */
        virtual void widgetRemoved(const ContainerEvent& containerEvent);
    
        /**
         * Called when a widget is resized.
         * 
         * @param event The widget event.
         */
        virtual void widgetResized(const Event& event);
    
        /**
         * Called when a widget is shown.
         * 
         * @param event The widget event.
         */
        virtual void widgetShown(const Event& event);
    
        /**
         * Called when a widget is hidden.
         * 
         * @param event The widget event.
         */
        virtual void widgetHidden(const Event& event);
    
        /**
         * Relayout the container explicitly.
         */
        virtual void relayout();
        
        /**
         * Sets padding between children.
         * 
         * @param padding The desired padding.
         */
        void setPadding(int padding);
        
        /**
         * Sets starting position of children.
         * 
         * @param childrenOffset The desired children offset.
         */
        void setChildrenOffset(int childrenOffset);
    
    protected:
    
        /**
         * Layouts a child added to the container.
         * 
         * @param added The child added.
         */
        virtual void layoutAddedChild(Widget* added) = 0;
    
        /**
         * Relayouts all children in the container.
         */
        virtual void relayoutChildren() = 0;
    
        /**
         * Resizes the container to fit children.
         */
        virtual void adjustSize() = 0;
    
        /**
         * Sets all children to have the same size at the axis that the
         * container implements.
         */
        virtual void fitChildrenToAxis() = 0;
        
        /**
         * Checks if the size constraint passed is smaller than the
         * size of this containers size constraint in both width and
         * height.
         * 
         * @param checkAgainst Size constraint to do the check against.
         * @throw Exception If checkAgainst is bigger in width or height
         *                  than this container's size constraint.
         */
        void checkSizeConstraint(SizeConstraint* checkAgainst);
        
        /**
         * @return Count of visible children in this container.
         */
        int getVisibleChildrenCount() const;
        
        /**
         * @return Accumulated padding for all visible widgets.
         */
        int getTotalPadding() const;
        
        /**
         * Padding between children.
         */
        int mPadding;
        
        /**
         * Starting children position.
         */
        int mChildrenOffset;
        
        /**
         * A list of children in the order they were added.
         */
         std::list<Widget*> mAddedOrder;
};

}

#endif