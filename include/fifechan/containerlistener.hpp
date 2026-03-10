// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#ifndef INCLUDE_FIFECHAN_CONTAINERLISTENER_HPP_
#define INCLUDE_FIFECHAN_CONTAINERLISTENER_HPP_

#include "fifechan/containerevent.hpp"
#include "fifechan/platform.hpp"

namespace fcn
{
    /**
     * Interface for listening for action from a container, such as Container.
     *
     * @see Container::addContainerListener, Container::removeContainerListener,
     *      ContainerEvent
     */
    class FIFEGUI_API ContainerListener
    {
    public:
        virtual ~ContainerListener() = default;

        ContainerListener(ContainerListener const &)            = default;
        ContainerListener& operator=(ContainerListener const &) = default;
        ContainerListener(ContainerListener&&)                  = default;
        ContainerListener& operator=(ContainerListener&&)       = default;

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
         * Instances should not be created directly,
         * therefore the constructor is declared protected.
         */
        ContainerListener() = default;
    };
} // namespace fcn

#endif // INCLUDE_FIFECHAN_CONTAINERLISTENER_HPP_
