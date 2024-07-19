// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#ifndef FCN_WIDGETLISTENER_HPP
#define FCN_WIDGETLISTENER_HPP

#include <string>

#include "fifechan/event.hpp"
#include "fifechan/platform.hpp"

namespace fcn
{
    /**
     * Interface for listening for events from widgets. When a widget's size,
     * location or visibility changes, the relevant method of the listener is
     * invoked.
     *
     * @see Widget::addWidgetListener, Widget::removeWidgetListener
     */
    class FIFEGUI_API WidgetListener
    {
    public:
        /**
         * Destructor.
         */
        virtual ~WidgetListener() = default;

        /**
         * Invoked when a widget changes its size.
         *
         * @param event Describes the event.
         */
        virtual void widgetResized(Event const & event) { }

        /**
         * Invoked when a widget is moved.
         *
         * @param event Describes the event.
         */
        virtual void widgetMoved(Event const & event) { }

        /**
         * Invoked when a widget is hidden, i.e it's set to be
         * not visible.
         *
         * @param event Describes the event.
         */
        virtual void widgetHidden(Event const & event) { }

        /**
         * Invoked when a widget is shown, i.e it's set to be
         * visible.
         *
         * @param event Describes the event.
         */
        virtual void widgetShown(Event const & event) { }

        /**
         * Invoked when an ancestor of a widget is moved.
         *
         * @Param event Describes the event.
         */
        virtual void ancestorMoved(Event const & event) { }

        /**
         * Invoked when an ancestor of a widget is hidden, i.e its
         * set to be not visible.
         *
         * @param event Describes the event.
         */
        virtual void ancestorHidden(Event const & event) { }

        /**
         * Invoked when an ancestor of a widget is shown, i.e its
         * set to be visible.
         *
         * @param event Describes the event.
         */
        virtual void ancestorShown(Event const & event) { }

    protected:
        /**
         * Constructor.
         *
         * You should not be able to make an instance of WidgetListener,
         * therefore its constructor is protected.
         */
        WidgetListener() { }
    };
} // namespace fcn

#endif // end FCN_WIDGETLISTENER_HPP
