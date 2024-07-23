// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#ifndef INCLUDE_FIFECHAN_LISTMODEL_HPP_
#define INCLUDE_FIFECHAN_LISTMODEL_HPP_

#include <string>

#include "fifechan/platform.hpp"

namespace fcn
{
    /**
     * An interface for a model that represents a list. It is
     * used in certain widgets, like the ListBox, to handle a
     * lists with string elements. If you want to use widgets
     * like ListBox, make a derived class from this class that
     * represents your list.
     */
    class FIFEGUI_API ListModel
    {

    public:
        virtual ~ListModel() { }

        /**
         * Gets the number of elements in the list.
         *
         * @return The number of elements in the list
         */
        virtual int getNumberOfElements() = 0;

        /**
         * Gets an element at a certain index in the list.
         *
         * @param i An index in the list.
         * @return An element as a string at the a certain index.
         */
        virtual std::string getElementAt(int i) = 0;
    };
} // namespace fcn

#endif // INCLUDE_FIFECHAN_LISTMODEL_HPP_
