// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2026 Fifengine contributors

// Corresponding header include
#include "fifechan/cliprectangle.hpp"

// Platform config include
#include "fifechan/platform.hpp"

// Project headers (subdirs before local)
#include "fifechan/rectangle.hpp"

namespace fcn
{
    ClipRectangle::ClipRectangle(int x, int y, int width, int height, int xOffset, int yOffset) :
        Rectangle(x, y, width, height), xOffset(xOffset), yOffset(yOffset)
    {
    }

    ClipRectangle& ClipRectangle::operator=(Rectangle const & other)
    {
        x      = other.x;
        y      = other.y;
        width  = other.width;
        height = other.height;

        return *this;
    }
} // namespace fcn
