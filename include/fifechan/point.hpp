/***************************************************************************
 *   Copyright (C) 2014 by the fifechan team                               *
 *   http://fifechan.github.com/fifechan                                   *
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


#ifndef FCN_POINT_H
#define FCN_POINT_H

// Standard C++ library includes
#include <iostream>
#include <cassert>
#include <vector>

// Platform specific includes

// 3rd party library includes

// FCN includes
// These includes are split up in two parts, separated by one empty line
// First block: files included from the FCN root src directory
// Second block: files included from the same folder
#include "fifechan/util/fcn_math.hpp"

namespace fcn {

    /** A 2D Point
     *
     * This is a small helper class to aid in 2d vector arithmetics.
     * @see Rect
     */
    class Point {
    public:
        union {
            int val[2];
            struct {
                int x,y;
            };
        };

        /** Constructor
         *
         * Creates a with 0 as default values.
         */
        explicit Point(int _x = 0, int _y = 0): x(_x), y(_y) {
        }

        /** Copy Constructor
         */
        Point(const Point& rhs): x(rhs.x), y(rhs.y) {
        }

        /** Vector addition
         */
        Point operator+(const Point& p) const {
            return Point(x + p.x, y + p.y);
        }

        /** Vector substraction
         */
        Point operator-(const Point& p) const {
            return Point(x - p.x, y - p.y);
        }

        /** Vector inplace addition
         */
        Point& operator+=(const Point& p) {
            x += p.x;
            y += p.y;
            return *this;
        }

        /** Vector inplace substraction
         */
        Point& operator-=(const Point& p) {
            x -= p.x;
            y -= p.y;
            return *this;
        }

        /** Scalar multiplication with an integer value
         */
        Point operator*(const int& i) const {
            return Point(x * i, y * i);
        }

        /** Scalar division with an integer value
         */
        Point operator/(const int& i) const {
            return Point(x / i, y / i);
        }

        /** Equality comparision
         */
        bool operator==(const Point& p) const {
            return x == p.x && y == p.y;
        }

        /** Equality comparision
         */
        bool operator!=(const Point& p) const {
            return !(x == p.x && y == p.y);
        }

        /** Return length
         */
        int length() const {
            double sq;
            sq = x*x + y*y;
            return static_cast<int>(Mathf::Sqrt(sq));
        }

        /** Normalizes the point
         */
        void normalize() {
            int invLength = static_cast<int>(1.0/length());

            //TODO: get rid of this static cast
            if (invLength > static_cast<int>(Mathf::zeroTolerance())) {
                x = x * invLength;
                y = y * invLength;
            }
            else {
                x = 0;
                y = 0;
            }
        }

        /** Rotates the point around the origin
         */
        void rotate(int angle){
            //TODO: get rid of this static cast
            int theta = (angle * static_cast<int>(Mathd::pi()))/180;
            int costheta = static_cast<int>(Mathd::Cos(theta));
            int sintheta = static_cast<int>(Mathd::Sin(theta));

            int nx = x;
            int ny = y;

            x = costheta * nx - sintheta * ny;
            y = sintheta * nx + costheta * ny;
        }

        /** Rotates the point around an origin
         */
        void rotate(const Point& origin, int angle){
            //TODO: get rid of this static cast
            int theta = (angle * static_cast<int>(Mathd::pi()))/180;
            int costheta = static_cast<int>(Mathd::Cos(theta));
            int sintheta = static_cast<int>(Mathd::Sin(theta));

            int nx = x - origin.x;
            int ny = y - origin.y;

            x = costheta * nx - sintheta * ny;
            y = sintheta * nx + costheta * ny;
        }

        /** Sets the x and y coordinate of the 2D point
         */
        void set(int _x, int _y) {
            x = _x;
            y = _y;
        }

        inline int& operator[] (int ind) {
            assert(ind > -1 && ind < 2);
            return val[ind];
        }

        friend std::ostream& operator<<(std::ostream& os, const Point& p) {
            return os << "(" << p.x << ":" << p.y << ")";
        }
    };

    typedef std::vector<Point> PointVector;
}

#endif
