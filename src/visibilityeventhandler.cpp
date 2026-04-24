// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2026 Fifengine contributors

// Corresponding header include
#include "fifechan/listeners/visibilityeventhandler.hpp"

// Project headers (subdirs before local)
#include "fifechan/events/event.hpp"
#include "fifechan/gui.hpp"

namespace fcn
{
    VisibilityEventHandler::VisibilityEventHandler(Gui* gui) : mGui(gui) { }

    void VisibilityEventHandler::widgetHidden(Event const & e)
    {
        mGui->addHiddenWidget(e.getSource());
    }

    void VisibilityEventHandler::widgetShown(Event const & e)
    {
        mGui->addShownWidget(e.getSource());
    }
}; // namespace fcn
