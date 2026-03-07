// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#include "fifechan/widgets/spacer.hpp"

#include "fifechan/exception.hpp"

namespace fcn
{
    Spacer::Spacer()
    {
        setVerticalExpand(true);
        setHorizontalExpand(true);
    }

    Spacer::~Spacer() = default;

    void Spacer::resizeToContent(bool /*recursion*/)
    {
        setSize(getMinSize().getWidth(), getMinSize().getHeight());
    }

    void Spacer::draw(Graphics* graphics) { }
}; // namespace fcn
