// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#include "fifechan/cliprectangle.hpp"

#include "fifechan/rectangle.hpp"

namespace fcn
{
    ClipRectangle::ClipRectangle()
    {
        x = y = width = height = xOffset = yOffset = 0;
    }

    ClipRectangle::ClipRectangle(int x, int y, int width, int height, int xOffset, int yOffset) :
        xOffset(xOffset), yOffset(yOffset)
    {
        this->x      = x;
        this->y      = y;
        this->width  = width;
        this->height = height;
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
