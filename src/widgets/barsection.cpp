// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2026 Fifengine contributors

// Corresponding header include
#include "fifechan/widgets/barsection.hpp"

namespace fcn
{
    BarSection::BarSection()
    {
        // Set horizontal layout policy by default
        setLayout(LayoutPolicy::Horizontal);
    }

    BarSection::~BarSection() = default;

    void BarSection::setAlignment(Alignment alignment)
    {
        mAlignment = alignment;
    }

    BarSection::Alignment BarSection::getAlignment() const
    {
        return mAlignment;
    }

    void BarSection::setExpand(bool expand)
    {
        mExpand = expand;
    }

    bool BarSection::isExpand() const
    {
        return mExpand;
    }
} // namespace fcn