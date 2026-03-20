// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2026 Fifengine contributors

#include "fifechan/containerevent.hpp"

#include "fifechan/event.hpp"

namespace fcn
{
    ContainerEvent::ContainerEvent(Widget* source, Container* container) : Event(source), mContainer(container) { }

    Container* ContainerEvent::getContainer() const
    {
        return mContainer;
    }
} // namespace fcn
