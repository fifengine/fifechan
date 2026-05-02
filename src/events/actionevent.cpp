// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2026 Fifengine contributors

// Corresponding header include
#include "fifechan/events/actionevent.hpp"

// Standard library includes
#include <cassert>
#include <string>
#include <utility>

// Project headers (subdirs before local)
#include "fifechan/events/event.hpp"

namespace fcn
{
    ActionEvent::ActionEvent(Widget* source, std::string id) : Event(source), mId(std::move(id))
    {
        assert("Source widget must not be null" && source != nullptr);
    }

    std::string const & ActionEvent::getId() const
    {
        return mId;
    }
} // namespace fcn
