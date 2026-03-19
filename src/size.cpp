// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2026 Fifengine contributors

#include "fifechan/size.hpp"

namespace fcn
{
    Size::Size(int width, int height) : mWidth(width), mHeight(height) { }

    int Size::getWidth() const
    {
        return mWidth;
    }

    int Size::getHeight() const
    {
        return mHeight;
    }

    void Size::setWidth(int width)
    {
        mWidth = width;
    }

    void Size::setHeight(int height)
    {
        mHeight = height;
    }
}; // namespace fcn
