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

#ifndef FCN_AUTOLAYOUTCONTAINER_HPP
#define FCN_AUTOLAYOUTCONTAINER_HPP

#include "fifechan/containerlistener.hpp"
#include "fifechan/widgetlistener.hpp"
#include "fifechan/widgets/container.hpp"

namespace fcn
{
    /**
     * Base class for container widgets that layout their children automatically.
     */
    class FCN_CORE_DECLSPEC AutoLayoutContainer : public Container,
                                                   public ContainerListener,
                                                   public WidgetListener
    {
    public:
        
        AutoLayoutContainer();
        
        virtual ~AutoLayoutContainer();
        
        /**
         * Sets this container as a widget event listener for the added
         * widget.
         * 
         * @WARNING Must be called from child classes re-implementing this.
         *
         * @param containerEvent The container event.
         */
        virtual void widgetAdded(const ContainerEvent& containerEvent);
        
        /**
         * Removes this container from the removed widget's listeners.
         * 
         * @WARNING Must be called from  child classes re-implementing this.
         * 
         * @param containerEvent The container event.
         */
        virtual void widgetRemoved(const ContainerEvent& containerEvent);
        
        /**
         * Relayouts this container. Call this if you want to explicitly relayout
         * the container.
         */
        virtual void relayout() = 0;
    };
};

#endif