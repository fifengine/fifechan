// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2026 Fifengine contributors

#include "fifechan/listeners/visibilityeventhandler.hpp"

#include "fifechan/event.hpp"
#include "fifechan/gui.hpp"

namespace fcn
{
    VisibilityEventHandler::VisibilityEventHandler(Gui* gui) : mGui(gui) { }

    void VisibilityEventHandler::widgetHidden(Event const & e)
    {
        mGui->enqueueHiddenWidget(e.getSource());
    }

    void VisibilityEventHandler::widgetShown(Event const & e)
    {
        mGui->enqueueShownWidget(e.getSource());
    }
}; // namespace fcn
