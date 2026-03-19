// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2026 Fifengine contributors

#ifndef INCLUDE_FIFECHAN_RECTANGLE_HPP_
#define INCLUDE_FIFECHAN_RECTANGLE_HPP_

#include <iostream>

#include "fifechan/platform.hpp"

namespace fcn
{
    /**
     * Represents a rectangular area (X, Y, Width, Height).
     *
     * @ingroup utils
     */
    class FIFEGUI_API Rectangle
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
        bool isIntersecting(Rectangle const & rectangle) const;

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
        bool isContaining(Rectangle const & other) const;

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
        Rectangle operator+(Rectangle const & rh) const;

        /**
         * Adds a rectangle to this rectangle. This rectangle will be
         * the union of the two rectangles.
         *
         * @param rh The rectangle to add.
         * @return A reference to this rectangle.
         */
        Rectangle const & operator+=(Rectangle const & rh);

        /**
         * Gets the intersection between two rectangles.
         *
         * @param rh The rectangle to calculate the intersection with.
         * @return The intersection between two rectangles.
         */
        Rectangle intersection(Rectangle const & rh) const;

        /**
         * Output operator for output.
         *
         * @param out The stream to output to.
         * @param rectangle The rectangle to output.
         */
        friend std::ostream& operator<<(std::ostream& out, Rectangle const & rectangle);

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
} // namespace fcn

#endif // INCLUDE_FIFECHAN_RECTANGLE_HPP_
