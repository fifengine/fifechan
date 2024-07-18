/***************************************************************************
 *   Copyright (c) 2017-2019 by the fifechan team                               *
 *   https://github.com/fifengine/fifechan                                 *
 *   This file is part of fifechan.                                        *
 *                                                                         *
 *   fifechan is free software; you can redistribute it and/or             *
 *   modify it under the terms of the GNU Lesser General Public            *
 *   License as published by the Free Software Foundation; either          *
 *   version 2.1 of the License, or (at your option) any later version.    *
 *                                                                         *
 *   This library is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU     *
 *   Lesser General Public License for more details.                       *
 *                                                                         *
 *   You should have received a copy of the GNU Lesser General Public      *
 *   License along with this library; if not, write to the                 *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA          *
 ***************************************************************************/

/*      _______   __   __   __   ______   __   __   _______   __   __
 *     / _____/\ / /\ / /\ / /\ / ____/\ / /\ / /\ / ___  /\ /  |\/ /\
 *    / /\____\// / // / // / // /\___\// /_// / // /\_/ / // , |/ / /
 *   / / /__   / / // / // / // / /    / ___  / // ___  / // /| ' / /
 *  / /_// /\ / /_// / // / // /_/_   / / // / // /\_/ / // / |  / /
 * /______/ //______/ //_/ //_____/\ /_/ //_/ //_/ //_/ //_/ /|_/ /
 * \______\/ \______\/ \_\/ \_____\/ \_\/ \_\/ \_\/ \_\/ \_\/ \_\/
 *
 * Copyright (c) 2004 - 2008 Olof Naess�n and Per Larsson
 *
 * Visit: http://guichan.sourceforge.net
 *
 * License: (BSD)
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name of Guichan nor the names of its contributors may
 *    be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 * TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

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
            x_ = 0;
        } else if (x_ + width_ > rectangle.width) {
            width_ = rectangle.width - x_;
        }

        if (y_ < 0) {
            height_ += y_;
            y_ = 0;
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
            return rh;
        }

        x      = x < rh.x ? x : rh.x;
        y      = y < rh.y ? y : rh.y;
        int const x2 = x + width > rh.x + rh.width ? x + width : rh.x + rh.width;
        int const y2 = y + height > rh.y + rh.height ? y + height : rh.y + rh.height;
        width  = x2 - x;
        height = y2 - y;
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
