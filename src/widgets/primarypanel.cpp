// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2026 Fifengine contributors

// Corresponding header include
#include "fifechan/widgets/primarypanel.hpp"

// Standard library includes
#include <string>

namespace fcn
{
    PrimaryPanel::PrimaryPanel(std::string const & title) : mDefaultWidth(250)
    {
        setTitle(title);
        setWidth(mDefaultWidth);
    }

    PrimaryPanel::~PrimaryPanel() { }

    void PrimaryPanel::setDefaultWidth(int width)
    {
        mDefaultWidth = width;
    }

    int PrimaryPanel::getDefaultWidth() const
    {
        return mDefaultWidth;
    }
} // namespace fcn
