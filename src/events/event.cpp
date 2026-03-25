// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2026 Fifengine contributors

#include "fifechan/events/event.hpp"

namespace fcn
{
    Event::Event(Widget* source) : mSource(source) { }

    Widget* Event::getSource() const
    {
        return mSource;
    }
} // namespace fcn
