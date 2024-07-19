// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#ifndef FCN_SELECTIONLISTENER_HPP
#define FCN_SELECTIONLISTENER_HPP

#include <string>

#include "fifechan/selectionevent.hpp"
#include "fifechan/platform.hpp"

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
    class FCN_CORE_DECLSPEC SelectionListener
    {
    public:
        /**
         * Destructor.
         */
        virtual ~SelectionListener() { }

        /**
         * Called when the value of a selection has been changed in a Widget.
         * It is used to be able to recieve a notification that a value has
         * been changed.
         *
         * @param event The event of the value change.
         */
        virtual void valueChanged(SelectionEvent const & event){};

    protected:
        /**
         * Constructor.
         *
         * You should not be able to make an instance of SelectionListener,
         * therefore its constructor is protected.
         */
        SelectionListener() { }
    };
} // namespace fcn

#endif // end FCN_SELECTIONLISTENER_HPP
