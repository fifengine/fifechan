// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2026 Fifengine contributors

#ifndef INCLUDE_FIFECHAN_SELECTIONLISTENER_HPP_
#define INCLUDE_FIFECHAN_SELECTIONLISTENER_HPP_

#include <string>

#include "fifechan/events/selectionevent.hpp"
#include "fifechan/platform.hpp"

namespace fcn
{
    /**
     * Interface for listening to selection change events.
     *
     * @see ListBox::addSelectionListener,
     *      ListBox::removeSelectionListener,
     *      DropDown::addSelectionListener,
     *      DropDown::removeSelectionListener
     *
     * @ingroup listeners
     */
    class FIFEGUI_API SelectionListener
    {
    public:
        virtual ~SelectionListener() = default;

        /** Copy constructor. */
        SelectionListener(SelectionListener const &) = default;
        /** Copy assignment operator. */
        SelectionListener& operator=(SelectionListener const &) = default;
        /** Move constructor. */
        SelectionListener(SelectionListener&&) = default;
        /** Move assignment operator. */
        SelectionListener& operator=(SelectionListener&&) = default;

        /**
         * Called when the value of a selection has been changed in a Widget.
         * It is used to be able to receive a notification that a value has
         * been changed.
         *
         * @param event The event of the value change.
         */
        virtual void valueChanged(SelectionEvent const & event) { }

    protected:
        /**
         * Constructor.
         *
         * Instances should not be created directly,
         * therefore the constructor is declared protected.
         */
        SelectionListener() = default;
    };
} // namespace fcn

#endif // INCLUDE_FIFECHAN_SELECTIONLISTENER_HPP_
