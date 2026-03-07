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

    /**
     * A 2D Point
     *
     * This is a small helper class to aid in 2d vector arithmetics.
     *
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

        /**
         * Constructor
         *
         * Creates a with 0 as default values.
         */
        explicit Point(int _x = 0, int _y = 0) : x(_x), y(_y) { }

        /**
         * Copy Constructor
         */
        Point(Point const & rhs) : x(rhs.x), y(rhs.y) { }

        /**
         * Vector addition
         */
        Point operator+(Point const & p) const
        {
            return Point(x + p.x, y + p.y);
        }

        /**
         * Vector subtraction
         */
        Point operator-(Point const & p) const
        {
            return Point(x - p.x, y - p.y);
        }

        /**
         * Vector inplace addition
         */
        Point& operator+=(Point const & p)
        {
            x += p.x;
            y += p.y;
            return *this;
        }

        /**
         * Vector inplace subtraction
         */
        Point& operator-=(Point const & p)
        {
            x -= p.x;
            y -= p.y;
            return *this;
        }

        /**
         * Scalar multiplication with an integer value
         */
        Point operator*(int const & i) const
        {
            return Point(x * i, y * i);
        }

        /**
         * Scalar division with an integer value
         */
        Point operator/(int const & i) const
        {
            return Point(x / i, y / i);
        }

        /**
         * Equality comparision
         */
        bool operator==(Point const & p) const
        {
            return x == p.x && y == p.y;
        }

        /**
         * Equality comparision
         */
        bool operator!=(Point const & p) const
        {
            return x != p.x || y != p.y;
        }

        /**
         * Return length
         */
        int length() const
        {
            double sq;
            sq = x * x + y * y;
            return static_cast<int>(Mathf::Sqrt(sq));
        }

        /**
         * Normalizes the point
         */
        void normalize()
        {
            float len = length();

            // Check if length is significantly greater than zero
            if (len > Mathf::zeroTolerance()) {
                float const invLength = 1.0F / len;
                x                     = x * invLength;
                y                     = y * invLength;
            } else {
                // Vector is too small to normalize; set to zero vector
                x = 0;
                y = 0;
            }
        }

        /**
         * Rotates the point around the origin by angle degrees.
         */
        Point rotated(int angle) const
        {
            double theta = static_cast<double>(angle) * Mathd::pi() / 180.0;
            double c     = Mathd::Cos(theta);
            double s     = Mathd::Sin(theta);

            return Point(static_cast<int>(std::round(c * x - s * y)), static_cast<int>(std::round(s * x + c * y)));
        }

        /**
         * Rotates the point around the origin by angle degrees.
         */
        void rotate(double angle)
        {
            double theta = angle * Mathd::pi() / 180.0;

            // Compute trig values as doubles
            double costheta = Mathd::Cos(theta);
            double sintheta = Mathd::Sin(theta);

            double nx = static_cast<double>(x);
            double ny = static_cast<double>(y);

            // apply rotation matrix and cast back to int
            x = static_cast<int>(costheta * nx - sintheta * ny);
            y = static_cast<int>(sintheta * nx + costheta * ny);
        }

        /**
         * Rotates the point around a given origin by angle degrees
         *
         * Coordinates stored as int. Uses float math internally.
         * Results are rounded to nearest integer.
         */
        void rotate(Point const & origin, int angle)
        {
            // 1. Translate point to origin-relative coordinates (promote to double)
            double nx = static_cast<double>(x - origin.x);
            double ny = static_cast<double>(y - origin.y);

            // 2. Convert angle to radians (keep as double)
            double theta = static_cast<double>(angle) * Mathd::pi() / 180.0;

            // 3. Compute trig values as double (NO int casts!)
            double costheta = Mathd::Cos(theta);
            double sintheta = Mathd::Sin(theta);

            // 4. Apply rotation matrix in floating-point
            double rx = costheta * nx - sintheta * ny;
            double ry = sintheta * nx + costheta * ny;

            // 5. Translate back to world coordinates AND cast to int (with rounding)
            x = static_cast<int>(std::round(origin.x + rx));
            y = static_cast<int>(std::round(origin.y + ry));
        }

        /**
         * Sets the x and y coordinate of the 2D point
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
