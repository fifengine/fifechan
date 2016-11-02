/***************************************************************************
 *   Copyright (c) 2016 by the fifechan team                               *
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
 *
 * Per Larsson a.k.a finalman
 * Olof Naess�n a.k.a jansem/yakslem
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

#ifndef FCN_RECTANGLE_HPP
#define FCN_RECTANGLE_HPP

#include "fifechan/platform.hpp"

#include <iostream>

namespace fcn
{
    /**
     * Represents a rectangle.
     *
     */
    class FCN_CORE_DECLSPEC Rectangle
    {
    public:

        /**
         * Constructor. The default rectangle is an empty rectangle
         * at the coordinates (0,0).
         */
        Rectangle();

        /**
         * Constructor.
         *
         * @param x The x coordinate of the rectangle.
         * @param y The y coordinate of the rectangle.
         * @param width The width of the rectangle.
         * @param height The height of the rectangle.
         */
        Rectangle(int x, int y, int width, int height);

        /**
         * Sets the dimension of a rectangle.
         *
         * @param x The x coordinate of the rectangle.
         * @param y The y coordinate of the rectangle.
         * @param width The width of the rectangle.
         * @param height The height of the rectangle.
         */
        void setAll(int x, int y, int width, int height);

        /**
         * Checks if another rectangle intersects with the rectangle.
         *
         * @param rectangle Another rectangle to check for intersection.
         * @return True if the rectangles intersect, false otherwise.
         */
        bool isIntersecting(const Rectangle& rectangle) const;

        /**
         * Checks the rectangle contains a point.
         *
         * @param x The x coordinate of the point.
         * @param y The y coordinate of the point.
         * @return True if the rectangle contains the point,
         *         false otherwise.
         */
        bool isContaining(int x, int y) const;

        /**
         * Checks if the rectangle contains a rectangle.
         *
         * @param other The rectangle to check.
         * @return True if the rectangle contains the rectangle,
         *         false otherwise.
         */
        bool isContaining(const Rectangle& other) const;

        /**
         * Checks whether the rectangle is empty or not. A rectangle
         * is considered empty when it's width or height is either
         * zero or negative.
         *
         * @return True if the rectangle is empty, false otherwise.
         */
        bool isEmpty() const;

        /**
         * Adds a rectangle to this rectangle. The resulting rectangle
         * is the union of the two rectangles.
         *
         * @param rh The rectangle to add.
         * @return The union of the two rectangles.
         */
        Rectangle operator+(const Rectangle& rh) const;

        /**
         * Adds a rectangle to this rectangle. This rectangle will be
         * the union of the two rectangles.
         *
         * @param rh The rectangle to add.
         * @return A reference to this rectangle.
         */
        const Rectangle& operator+=(const Rectangle& rh);

        /**
         * Gets the intersection between two rectangles.
         *
         * @param rh The rectangle to calculate the intersection with.
         * @return The intersection between two rectangles.
         */
        Rectangle intersection(const Rectangle& rh) const;

        /**
         * Output operator for output.
         *
         * @param out The stream to output to.
         * @param rectangle The rectangle to output.
         */
        friend std::ostream& operator<<(std::ostream& out,
                                        const Rectangle& rectangle);

        /**
         * Holds the x coordinate of the rectangle.
         */
        int x;

        /**
         * Holds the x coordinate of the rectangle.
         */
        int y;

        /**
         * Holds the width of the rectangle.
         */
        int width;

        /**
         * Holds the height of the rectangle.
         */
        int height;
    };
}

#endif // end FCN_RECTANGEL_HPP
