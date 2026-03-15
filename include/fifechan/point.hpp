// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#ifndef INCLUDE_FIFECHAN_POINT_HPP_
#define INCLUDE_FIFECHAN_POINT_HPP_

#include <array>
#include <cassert>
#include <iostream>
#include <vector>
#include <utility>

#include "fifechan/math.hpp"

namespace fcn
{

    /**
     * Represents a 2D coordinate (X, Y).
     *
     * This is a small helper class to aid in 2d vector arithmetics.
     *
     * @see Rect
     *
     * @ingroup utils
     */
    class Point
    {
    public:
        union
        {
            std::array<int, 2> val;
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
        explicit Point(int _x = 0, int _y = 0) : val{_x, _y} { }

        /**
         * Copy Constructor
         */
        Point(Point const & rhs) : val{rhs.val[0], rhs.val[1]} { }

        /**
         * Move Constructor
         */
        Point(Point&& rhs) noexcept : val{std::move(rhs.val)} { }

        /**
         * Copy assignment
         */
        Point& operator=(Point const & rhs)
        {
            if (this != &rhs) {
                val = rhs.val;
            }
            return *this;
        }

        /**
         * Move assignment
         */
        Point& operator=(Point&& rhs) noexcept
        {
            if (this != &rhs) {
                val = std::move(rhs.val);
            }
            return *this;
        }

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
            double const sq = (static_cast<double>(x) * x) + (static_cast<double>(y) * y);
            return static_cast<int>(Mathf::Sqrt(sq));
        }

        /**
         * Normalizes the point
         */
        void normalize()
        {
            float const len = length();

            if (len > Mathf::zeroTolerance()) {
                float const invLength = 1.0F / len;
                x                     = static_cast<int>(x * invLength);
                y                     = static_cast<int>(y * invLength);
            } else {
                x = 0;
                y = 0;
            }
        }

        /**
         * Rotates the point around the origin by angle degrees.
         */
        Point rotated(int angle) const
        {
            double const theta = static_cast<double>(angle) * Mathd::pi() / 180.0;
            double const c     = Mathd::Cos(theta);
            double const s     = Mathd::Sin(theta);

            return Point(
                static_cast<int>(std::round((c * x) - (s * y))), static_cast<int>(std::round((s * x) + (c * y))));
        }

        /**
         * Rotates the point around the origin by angle degrees.
         */
        void rotate(double angle)
        {
            double const theta = angle * Mathd::pi() / 180.0;

            double const costheta = Mathd::Cos(theta);
            double const sintheta = Mathd::Sin(theta);

            double const nx = static_cast<double>(x);
            double const ny = static_cast<double>(y);

            x = static_cast<int>((costheta * nx) - (sintheta * ny));
            y = static_cast<int>((sintheta * nx) + (costheta * ny));
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
            double const nx = static_cast<double>(x - origin.x);
            double const ny = static_cast<double>(y - origin.y);

            // 2. Rotate point by angle (in radians)
            double const theta = static_cast<double>(angle) * Mathd::pi() / 180.0;

            // 3. Calculate rotation using standard 2D rotation matrix
            double const costheta = Mathd::Cos(theta);
            double const sintheta = Mathd::Sin(theta);

            // 4. Apply rotation to point
            double const rx = (costheta * nx) - (sintheta * ny);
            double const ry = (sintheta * nx) + (costheta * ny);

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

        /**
         * Index accessor for the point components.
         *
         * @param ind Index 0 for X, 1 for Y. Asserts if out of range.
         * @return Reference to the requested component.
         */
        int& operator[](int ind)
        {
            assert(ind > -1 && ind < 2);
            return val[ind];
        }

        /**
         * Stream output operator for debug/logging.
         *
         * Formats the point as "(x:y)".
         */
        friend std::ostream& operator<<(std::ostream& os, Point const & p)
        {
            return os << "(" << p.x << ":" << p.y << ")";
        }
    };

    using PointVector = std::vector<Point>;
} // namespace fcn

#endif // INCLUDE_FIFECHAN_POINT_HPP_
