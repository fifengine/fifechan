// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2026 Fifengine contributors

// Corresponding header include
#include "fifechan/cliprectangle.hpp"

// Platform config include
#include "fifechan/platform.hpp"

// Project headers (subdirs before local)
#include <cassert>

#include "fifechan/rectangle.hpp"

namespace fcn
{
    ClipRectangle::ClipRectangle(int x, int y, int width, int height, int xOffset, int yOffset) :
        Rectangle(x, y, width, height), xOffset(xOffset), yOffset(yOffset)
    {
        assert("Width must be non-negative" && width >= 0);
        assert("Height must be non-negative" && height >= 0);
    }

    ClipRectangle& ClipRectangle::operator=(Rectangle const & other)
    {
        assert("Rectangle width must be non-negative" && other.width >= 0);
        assert("Rectangle height must be non-negative" && other.height >= 0);

        x      = other.x;
        y      = other.y;
        width  = other.width;
        height = other.height;

        return *this;
    }
} // namespace fcn
