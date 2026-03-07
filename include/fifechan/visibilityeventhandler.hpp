// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#ifndef INCLUDE_FIFECHAN_VISIBILITYEVENTHANDLER_HPP_
#define INCLUDE_FIFECHAN_VISIBILITYEVENTHANDLER_HPP_

#include "fifechan/widgetlistener.hpp"

namespace fcn
{
    class Event;
    class Gui;
    class Widget;

    /**
     * The `VisibilityEventHandler` class handles widget visibility changes.
     * It addresses input issues that arise when widgets are shown or hidden.
     */
    class FIFEGUI_API VisibilityEventHandler : public WidgetListener
    {
    public:
        explicit VisibilityEventHandler(Gui* gui);

        ~VisibilityEventHandler() override = default;

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

#endif // INCLUDE_FIFECHAN_VISIBILITYEVENTHANDLER_HPP_
