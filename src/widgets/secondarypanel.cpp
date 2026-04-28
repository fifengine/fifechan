// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2026 Fifengine contributors

// Corresponding header include
#include "fifechan/widgets/secondarypanel.hpp"

// Standard library includes
#include <string>

namespace fcn
{
    SecondaryPanel::SecondaryPanel(std::string const & title) : mDefaultWidth(250)
    {
        setTitle(title);
        setWidth(mDefaultWidth);
    }

    SecondaryPanel::~SecondaryPanel() { }

    void SecondaryPanel::setDefaultWidth(int width)
    {
        mDefaultWidth = width;
    }

    int SecondaryPanel::getDefaultWidth() const
    {
        return mDefaultWidth;
    }
} // namespace fcn
