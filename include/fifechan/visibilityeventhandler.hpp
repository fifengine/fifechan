// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

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
