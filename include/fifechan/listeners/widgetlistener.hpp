// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2026 Fifengine contributors

#ifndef INCLUDE_FIFECHAN_WIDGETLISTENER_HPP_
#define INCLUDE_FIFECHAN_WIDGETLISTENER_HPP_

#include <string>

#include "fifechan/events/event.hpp"
#include "fifechan/platform.hpp"

namespace fcn
{
    /**
     * Interface for receiving generic events from widgets.
     *
     * When a widget's size, location or visibility changes,
     * the relevant method of the listener is invoked.
     *
     * @see Widget::addWidgetListener, Widget::removeWidgetListener
     *
     * @ingroup listeners
     */
    class FIFEGUI_API WidgetListener
    {
    public:
        virtual ~WidgetListener() = default;

        /** Copy constructor. */
        WidgetListener(WidgetListener const &) = default;
        /** Copy assignment operator. */
        WidgetListener& operator=(WidgetListener const &) = default;
        /** Move constructor. */
        WidgetListener(WidgetListener&&) = default;
        /** Move assignment operator. */
        WidgetListener& operator=(WidgetListener&&) = default;

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
         * @param event Describes the event.
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
         * Instances should not be created directly,
         * therefore the constructor is declared protected.
         */
        WidgetListener() = default;
    };
} // namespace fcn

#endif // INCLUDE_FIFECHAN_WIDGETLISTENER_HPP_
