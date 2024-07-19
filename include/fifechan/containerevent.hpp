// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#ifndef FCN_CONTAINEREVENT_HPP
#define FCN_CONTAINEREVENT_HPP

#include "fifechan/event.hpp"
#include "fifechan/platform.hpp"

namespace fcn
{
    class Container;
    class Widget;

    /**
     * Represents a container event. A container event is an event
     * that can be fired by a container whenever a widget is added
     * or removed.
     *
     * Any object can listen for actions from a container by implementing
     * the ContainerListener interface.
     *
     * @see Container::addContainerListener, Container::removeContainerListener,
     *      Container::distributeContainerEvent
     */
    class FIFEGUI_API ContainerEvent : public Event
    {
    public:
        /**
         * Constructor.
         *
         * @param source The source widget of the event.
         * @param container The container the event concerns.
         */
        ContainerEvent(Widget* source, Container* container);

        /**
         * Destructor.
         */
        ~ContainerEvent() override;

        /**
         * Gets the container the event concerns.
         *
         * @return The container the event concerns.
         */
        Container* getContainer() const;

    protected:
        /**
         * Holds the container the event concerns.
         */
        Container* mContainer;
    };
} // namespace fcn

#endif // FCN_ACTIONEVENT_HPP
