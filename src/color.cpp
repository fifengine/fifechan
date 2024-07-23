// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#include "fifechan/color.hpp"

#include <cstdint>
#include <ostream>

namespace fcn
{
    // Default constructor
    Color::Color() : r(0), g(0), b(0), a(255) { }

    // Constructor from a single integer color value
    Color::Color(int color) : r((color >> 16) & 0xFF), g((color >> 8) & 0xFF), b(color & 0xFF), a(255) { }

    // Constructor with int parameters for each color component
    Color::Color(int ar, int ag, int ab, int aa) :
        r(static_cast<std::uint8_t>(ar)),
        g(static_cast<std::uint8_t>(ag)),
        b(static_cast<std::uint8_t>(ab)),
        a(static_cast<std::uint8_t>(aa))
    {
    }

    // Constructor with uint8_t parameters for each color component
    Color::Color(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha) : r(red), g(green), b(blue), a(alpha) { }

    Color Color::operator+(Color const & color) const
    {
        Color result(r + color.r, g + color.g, b + color.b, 255);

        result.r = (result.r > 255 ? 255 : (result.r < 0 ? 0 : result.r));
        result.g = (result.g > 255 ? 255 : (result.g < 0 ? 0 : result.g));
        result.b = (result.b > 255 ? 255 : (result.b < 0 ? 0 : result.b));

        return result;
    }

    Color Color::operator-(Color const & color) const
    {
        Color result(r - color.r, g - color.g, b - color.b, 255);

        result.r = (result.r > 255 ? 255 : (result.r < 0 ? 0 : result.r));
        result.g = (result.g > 255 ? 255 : (result.g < 0 ? 0 : result.g));
        result.b = (result.b > 255 ? 255 : (result.b < 0 ? 0 : result.b));

        return result;
    }

    Color Color::operator*(float value) const
    {
        Color result(static_cast<int>(r * value), static_cast<int>(g * value), static_cast<int>(b * value), a);

        result.r = (result.r > 255 ? 255 : (result.r < 0 ? 0 : result.r));
        result.g = (result.g > 255 ? 255 : (result.g < 0 ? 0 : result.g));
        result.b = (result.b > 255 ? 255 : (result.b < 0 ? 0 : result.b));

        return result;
    }

    bool Color::operator==(Color const & color) const
    {
        return r == color.r && g == color.g && b == color.b && a == color.a;
    }

    bool Color::operator!=(Color const & color) const
    {
        return r != color.r || g != color.g || b != color.b || a != color.a;
    }

    std::ostream& operator<<(std::ostream& out, Color const & color)
    {
        out << "Color [r = " << color.r << ", g = " << color.g << ", b = " << color.b << ", a = " << color.a << "]";

        return out;
    }
} // namespace fcn
