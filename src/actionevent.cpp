// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2026 Fifengine contributors

#include "fifechan/actionevent.hpp"

#include <string>
#include <utility>

#include "fifechan/event.hpp"

namespace fcn
{
    ActionEvent::ActionEvent(Widget* source, std::string id) : Event(source), mId(std::move(id)) { }

    std::string const & ActionEvent::getId() const
    {
        return mId;
    }
} // namespace fcn
