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
 
#ifndef FCN_VBOX_HPP
#define FCN_VBOX_HPP

#include "fifechan/sizeconstraint.hpp"
#include "fifechan/widgets/axislayoutcontainer.hpp"

namespace fcn
{
    class FCN_CORE_DECLSPEC VBox : public AxisLayoutContainer
    {
    public:
        
        VBox(SizeConstraint *sizeConstraint);
        
        virtual ~VBox();
        
    protected:
                 
        /**
         * Relayouts all children.
         */
        virtual void relayoutChildren();
        
        /**
         * Expands or shrinks the container based on content.
         */
        virtual void adjustSize();
        
        /**
         * Sets position of the added child in this container.
         * 
         * @param added The added child.
         */
        virtual void layoutAddedChild(Widget* added);
        
        /**
         * Sets all children to have the same width as this container.
         */
        virtual void fitChildrenToAxis();
        
        /**
         * @return Total height for all visible children.
         */
        int getVisibleChildrenHeight() const;
        
        /**
         * @return Width of widest child.
         */
        int getMaxVisibleChildWidth() const;
    };
};

#endif