/***************************************************************************
 *   Copyright (c) 2017-2019 by the fifechan team                               *
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

#ifndef FCN_VISIBILITYEVENTHANDLER_HPP
#define FCN_VISIBILITYEVENTHANDLER_HPP

#include "fifechan/widgetlistener.hpp"

namespace fcn
{
    class Event;
    class Gui;
    class Widget;

    /**
     * Class that handles Widget visiblity changes. Addresses input issues arised
     * when widgets are shown or hidden.
     */
    class FCN_CORE_DECLSPEC VisibilityEventHandler : public WidgetListener
    {
    public:
        /**
         * Constructor
         */
        explicit VisibilityEventHandler(Gui* gui);

        /**
         * Destructor
         */
        ~VisibilityEventHandler() = default;

        /**
         * Informs gui that a widget was hidden.
         */
        void widgetHidden(Event const & e) override;

        /**
         * Informs gui that a widget was shown.
         */
        void widgetShown(Event const & e) override;

    protected:
        Gui* mGui;
    };
} // namespace fcn

#endif // FCN_VISIBILITYEVENTHANDLER_HPP
