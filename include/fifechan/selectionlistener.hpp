// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#ifndef INCLUDE_FIFECHAN_SELECTIONLISTENER_HPP_
#define INCLUDE_FIFECHAN_SELECTIONLISTENER_HPP_

#include <string>

#include "fifechan/platform.hpp"
#include "fifechan/selectionevent.hpp"

namespace fcn
{
    /**
     * Interface for listening for selection events from widgets.
     *
     * @see ListBox::addSelectionListener,
     *      ListBox::removeSelectionListener,
     *      DropDown::addSelectionListener,
     *      DropDown::removeSelectionListener
     */
    class FIFEGUI_API SelectionListener
    {
    public:
        virtual ~SelectionListener() = default;

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
         * You should not be able to make an instance of SelectionListener,
         * therefore its constructor is protected.
         */
        SelectionListener() = default;
    };
} // namespace fcn

#endif // INCLUDE_FIFECHAN_SELECTIONLISTENER_HPP_
