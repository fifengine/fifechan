// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#include "fifechan/font.hpp"

#include <string>

namespace fcn
{
    int Font::getStringIndexAt(std::string const & text, int x) const
    {
        unsigned int i = 0;
        int size       = 0;

        for (i = 0; i < text.size(); ++i) {
            size = getWidth(text.substr(0, i));

            if (size > x) {
                return i;
            }
        }

        return text.size();
    }
} // namespace fcn
