// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#ifndef FCN_CONTAINERLISTENER_HPP
#define FCN_CONTAINERLISTENER_HPP

#include "fifechan/containerevent.hpp"
#include "fifechan/platform.hpp"

namespace fcn
{
    /**
     * Interface for listening for action from a container, such as BasicContainer.
     *
     * @see BasicContainer::addContainerListener, BasicContainer::removeContainerListener,
     *      ContainerEvent
     */
    class FIFEGUI_API ContainerListener
    {
    public:
        /**
         * Destructor.
         */
        virtual ~ContainerListener() { }

        /**
         * Called when a widget is added to a container.
         *
         * @param containerEvent The event of the action.
         */
        virtual void widgetAdded(ContainerEvent const & containerEvent) = 0;

        /**
         * Called when a widget is removed from a container.
         *
         * @param containerEvent The event of the action.
         */
        virtual void widgetRemoved(ContainerEvent const & containerEvent) = 0;

    protected:
        /**
         * Constructor.
         *
         * You should not be able to make an instance of ContainerListener,
         * therefore its constructor is protected.
         */
        ContainerListener() { }
    };
} // namespace fcn

#endif // end FCN_ACTIONLISTENER_HPP
