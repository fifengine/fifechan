// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2026 Fifechan contributors

// Corresponding header include
#include "fifechan/events/event.hpp"

// Standard library includes
#include <cassert>

// Platform config include
#include "fifechan/platform.hpp"

namespace fcn
{
    Event::Event(Widget* source) : mSource(source)
    {
    }

    Widget* Event::getSource() const
    {
        return mSource;
    }
} // namespace fcn
