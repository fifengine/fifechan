// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#include "fifechan/rectangle.hpp"

#include <ostream>

namespace fcn
{
    Rectangle::Rectangle() : x(0), y(0), width(0), height(0) { }

    Rectangle::Rectangle(int x_, int y_, int width_, int height_) : x(x_), y(y_), width(width_), height(height_) { }

    void Rectangle::setAll(int x_, int y_, int width_, int height_)
    {
        x      = x_;
        y      = y_;
        width  = width_;
        height = height_;
    }

    bool Rectangle::isIntersecting(Rectangle const & rectangle) const
    {
        int x_      = x;
        int y_      = y;
        int width_  = width;
        int height_ = height;

        x_ -= rectangle.x;
        y_ -= rectangle.y;

        if (x_ < 0) {
            width_ += x_;
        } else if (x_ + width_ > rectangle.width) {
            width_ = rectangle.width - x_;
        }

        if (y_ < 0) {
            height_ += y_;
        } else if (y_ + height_ > rectangle.height) {
            height_ = rectangle.height - y_;
        }

        if (width_ <= 0 || height_ <= 0) {
            return false;
        }

        return true;
    }

    bool Rectangle::isContaining(int x_, int y_) const
    {
        if (isEmpty()) {
            return false;
        }

        return x_ >= x && y_ >= y && x_ < x + width && y_ < y + height;
    }

    bool Rectangle::isContaining(Rectangle const & other) const
    {
        if (other.isEmpty()) {
            return false;
        }

        return other.x >= x && other.y >= y && other.x + other.width <= x + width &&
               other.y + other.height <= y + height;
    }

    bool Rectangle::isEmpty() const
    {
        return width <= 0 || height <= 0;
    }

    Rectangle Rectangle::operator+(Rectangle const & rh) const
    {
        if (rh.isEmpty()) {
            return {x, y, width, height};
        }

        int const nx  = x < rh.x ? x : rh.x;
        int const ny  = y < rh.y ? y : rh.y;
        int const nx2 = x + width > rh.x + rh.width ? x + width : rh.x + rh.width;
        int const ny2 = y + height > rh.y + rh.height ? y + height : rh.y + rh.height;
        return {nx, ny, nx2 - nx, ny2 - ny};
    }

    Rectangle const & Rectangle::operator+=(Rectangle const & rh)
    {
        if (rh.isEmpty()) {
            return *(this);
        }

        if (isEmpty()) {
            *this = rh; // avoid use-after-free, assign instead of returning rh
            return *this;
        }

        x            = x < rh.x ? x : rh.x;
        y            = y < rh.y ? y : rh.y;
        int const x2 = x + width > rh.x + rh.width ? x + width : rh.x + rh.width;
        int const y2 = y + height > rh.y + rh.height ? y + height : rh.y + rh.height;
        width        = x2 - x;
        height       = y2 - y;
        return *(this);
    }

    Rectangle Rectangle::intersection(Rectangle const & rh) const
    {
        int const nx = x > rh.x ? x : rh.x;
        int const ny = y > rh.y ? y : rh.y;

        if (rh.isEmpty() || isEmpty()) {
            return {nx, ny, 0, 0};
        }

        int const nx2 = x + width < rh.x + rh.width ? x + width : rh.x + rh.width;
        int const ny2 = y + height < rh.y + rh.height ? y + height : rh.y + rh.height;
        Rectangle result(nx, ny, nx2 - nx, ny2 - ny);

        if (result.isEmpty()) {
            return {nx, ny, 0, 0};
        }

        return result;
    }

    std::ostream& operator<<(std::ostream& out, Rectangle const & rectangle)
    {
        out << "Rectangle [x = " << rectangle.x << ", y = " << rectangle.y << ", width = " << rectangle.width
            << ", height = " << rectangle.height << "]";

        return out;
    }
} // namespace fcn
