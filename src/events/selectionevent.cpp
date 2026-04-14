// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2026 Fifengine contributors

// Corresponding header include
#include "fifechan/events/selectionevent.hpp"

// Platform config include
#include "fifechan/platform.hpp"

// Project headers (subdirs before local)
#include "fifechan/events/event.hpp"

namespace fcn
{
    SelectionEvent::SelectionEvent(Widget* source) : Event(source) { }

} // namespace fcn
