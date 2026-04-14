// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2026 Fifengine contributors

// Corresponding header include
#include "fifechan/font.hpp"

// Standard library includes
#include <string>

// Platform config include
#include "fifechan/platform.hpp"

namespace fcn
{
    int Font::getStringIndexAt(std::string const & text, int x) const
    {
        for (unsigned int i = 0; i < text.size(); ++i) {
            if (getWidth(text.substr(0, i)) > x) {
                return i;
            }
        }
        return text.size();
    }
} // namespace fcn
