// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#ifndef INCLUDE_FIFECHAN_POINT_HPP_
#define INCLUDE_FIFECHAN_POINT_HPP_

#include <cassert>
#include <iostream>
#include <vector>

#include "fifechan/math.hpp"

namespace fcn
{

    /** A 2D Point
     *
     * This is a small helper class to aid in 2d vector arithmetics.
     * @see Rect
     */
    class Point
    {
    public:
        union
        {
            int val[2];
            struct
            {
                int x, y;
            };
        };

        /** Constructor
         *
         * Creates a with 0 as default values.
         */
        explicit Point(int _x = 0, int _y = 0) : x(_x), y(_y) { }

        /** Copy Constructor
         */
        Point(Point const & rhs) : x(rhs.x), y(rhs.y) { }

        /** Vector addition
         */
        Point operator+(Point const & p) const
        {
            return Point(x + p.x, y + p.y);
        }

        /** Vector subtraction
         */
        Point operator-(Point const & p) const
        {
            return Point(x - p.x, y - p.y);
        }

        /** Vector inplace addition
         */
        Point& operator+=(Point const & p)
        {
            x += p.x;
            y += p.y;
            return *this;
        }

        /** Vector inplace subtraction
         */
        Point& operator-=(Point const & p)
        {
            x -= p.x;
            y -= p.y;
            return *this;
        }

        /** Scalar multiplication with an integer value
         */
        Point operator*(int const & i) const
        {
            return Point(x * i, y * i);
        }

        /** Scalar division with an integer value
         */
        Point operator/(int const & i) const
        {
            return Point(x / i, y / i);
        }

        /** Equality comparision
         */
        bool operator==(Point const & p) const
        {
            return x == p.x && y == p.y;
        }

        /** Equality comparision
         */
        bool operator!=(Point const & p) const
        {
            return x != p.x || y != p.y;
        }

        /** Return length
         */
        int length() const
        {
            double sq;
            sq = x * x + y * y;
            return static_cast<int>(Mathf::Sqrt(sq));
        }

        /** Normalizes the point
         */
        void normalize()
        {
            int invLength = static_cast<int>(1.0 / length());

            // TODO(someone): get rid of this static cast.
            if (invLength > static_cast<int>(Mathf::zeroTolerance())) {
                x = x * invLength;
                y = y * invLength;
            } else {
                x = 0;
                y = 0;
            }
        }

        /** Rotates the point around the origin
         */
        void rotate(int angle)
        {
            // TODO(someone): get rid of this static cast.
            int theta    = (angle * static_cast<int>(Mathd::pi())) / 180;
            int costheta = static_cast<int>(Mathd::Cos(theta));
            int sintheta = static_cast<int>(Mathd::Sin(theta));

            int nx = x;
            int ny = y;

            x = costheta * nx - sintheta * ny;
            y = sintheta * nx + costheta * ny;
        }

        /** Rotates the point around an origin
         */
        void rotate(Point const & origin, int angle)
        {
            // TODO(someone): get rid of this static cast.
            int theta    = (angle * static_cast<int>(Mathd::pi())) / 180;
            int costheta = static_cast<int>(Mathd::Cos(theta));
            int sintheta = static_cast<int>(Mathd::Sin(theta));

            int nx = x - origin.x;
            int ny = y - origin.y;

            x = costheta * nx - sintheta * ny;
            y = sintheta * nx + costheta * ny;
        }

        /** Sets the x and y coordinate of the 2D point
         */
        void set(int _x, int _y)
        {
            x = _x;
            y = _y;
        }

        inline int& operator[](int ind)
        {
            assert(ind > -1 && ind < 2);
            return val[ind];
        }

        friend std::ostream& operator<<(std::ostream& os, Point const & p)
        {
            return os << "(" << p.x << ":" << p.y << ")";
        }
    };

    using PointVector = std::vector<Point>;
} // namespace fcn

#endif // INCLUDE_FIFECHAN_POINT_HPP_
