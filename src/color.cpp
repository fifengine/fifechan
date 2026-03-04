// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2026 Fifengine contributors

#include "fifechan/color.hpp"

#include <algorithm>
#include <string>
#include <vector>

namespace fcn
{
    // Constructs a color from the bytes in an integer (int).
    Color::Color(int color) : r((color >> 16) & 0xFF), g((color >> 8) & 0xFF), b((color >> 0) & 0xFF) { }

    // Constructor with integer parameters for each color component (int RGBA).
    Color::Color(int red, int green, int blue, int alpha) :
        r(static_cast<std::uint8_t>(red)),
        g(static_cast<std::uint8_t>(green)),
        b(static_cast<std::uint8_t>(blue)),
        a(static_cast<std::uint8_t>(alpha))
    {
    }

    // Constructor with uint8_t parameters for each color component and an optional alpha component (uint8_t RGBA).
    Color::Color(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha) : r(red), g(green), b(blue), a(alpha) { }

    // Constructor accepting a string for hex or RGB.
    Color::Color(std::string const & colorString)
    {
        if (colorString[0] == '#') {
            // handle hex string, remove '#' and parse
            parseHex(colorString.substr(1));
        } else if (colorString.find("rgba") != std::string::npos) {
            // handle rgba string
            parseRGBA(colorString);
        } else {
            // handle rgb string
            parseRGB(colorString);
        }
    }

    void Color::parseHex(std::string const & hex)
    {
        if (hex.size() != 6) {
            throw std::invalid_argument("Hex code must be 6 characters long.");
        }

        r = static_cast<uint8_t>(std::stoi(hex.substr(0, 2), nullptr, 16));
        g = static_cast<uint8_t>(std::stoi(hex.substr(2, 2), nullptr, 16));
        b = static_cast<uint8_t>(std::stoi(hex.substr(4, 2), nullptr, 16));
        a = 255; // Default alpha
    }

    void Color::parseRGB(std::string const & rgbString)
    {
        // Ensure the string starts with 'rgb(' and ends with ')'
        if (!rgbString.starts_with("rgb(") || !rgbString.ends_with(")")) {
            throw std::invalid_argument("Invalid RGB format. Expected: rgb(r,g,b)");
        }

        // Extract the part inside "rgb()"
        std::string const values = rgbString.substr(4, rgbString.size() - 5); // Exclude "rgb(" and ")"

        // Parse the RGB components
        std::vector<int> components = parseColorComponents(values);
        if (components.size() != 3) {
            throw std::invalid_argument("Invalid RGB format. Expected 3 components (r,g,b)");
        }

        r = static_cast<uint8_t>(components[0]);
        g = static_cast<uint8_t>(components[1]);
        b = static_cast<uint8_t>(components[2]);
        a = 255; // Default alpha
    }

    void Color::parseRGBA(std::string const & rgbaString)
    {
        // Ensure the string starts with 'rgba(' and ends with ')'
        if (!rgbaString.starts_with("rgba(") || !rgbaString.ends_with(")")) {
            throw std::invalid_argument("Invalid RGBA format. Expected: rgba(r,g,b,a)");
        }

        // Extract the part inside "rgba()"
        std::string const values = rgbaString.substr(5, rgbaString.size() - 6); // Exclude "rgba(" and ")"

        // Parse the RGBA components
        std::vector<int> components = parseColorComponents(values, true);
        if (components.size() != 4) {
            throw std::invalid_argument("Invalid RGBA format. Expected 4 components (r,g,b,a)");
        }

        r = static_cast<uint8_t>(components[0]);
        g = static_cast<uint8_t>(components[1]);
        b = static_cast<uint8_t>(components[2]);
        a = static_cast<uint8_t>(components[3]);
    }

    std::vector<int> Color::parseColorComponents(std::string const & colorString, bool withAlpha)
    {
        std::vector<int> colors;

        // Extract the RGB(A) values from the content string.
        size_t const firstComma = colorString.find(',');
        if (firstComma == std::string::npos) {
            throw std::invalid_argument("Invalid format. Expected comma after first value.");
        }

        size_t const secondComma = colorString.find(',', firstComma + 1);
        if (secondComma == std::string::npos) {
            throw std::invalid_argument("Invalid format. Expected comma after second value.");
        }

        size_t const thirdComma = colorString.find(',', secondComma + 1);
        if (withAlpha && thirdComma == std::string::npos) {
            throw std::invalid_argument("Invalid format. Expected comma after third value.");
        }

        // Extract components as substrings
        std::string const redStr   = colorString.substr(0, firstComma);
        std::string const greenStr = colorString.substr(firstComma + 1, secondComma - firstComma - 1);
        std::string const blueStr  = colorString.substr(secondComma + 1, thirdComma - secondComma - 1);
        // Use default alpha value, if not present
        std::string const alphaStr = withAlpha ? colorString.substr(thirdComma + 1) : "255";

        // Convert each component using std::strtol with error handling
        char* end = nullptr;

        int const red = std::strtol(redStr.c_str(), &end, 10);
        if (*end != '\0') {
            throw std::invalid_argument("Invalid red value.");
        }

        int const green = std::strtol(greenStr.c_str(), &end, 10);
        if (*end != '\0') {
            throw std::invalid_argument("Invalid green value.");
        }

        int const blue = std::strtol(blueStr.c_str(), &end, 10);
        if (*end != '\0') {
            throw std::invalid_argument("Invalid blue value.");
        }

        int const alpha = std::strtol(alphaStr.c_str(), &end, 10);
        if (withAlpha && *end != '\0') {
            throw std::invalid_argument("Invalid alpha value.");
        }

        // Add values to components vector
        colors.push_back(red);
        colors.push_back(green);
        colors.push_back(blue);
        if (withAlpha) {
            colors.push_back(alpha);
        }

        return colors;
    }

    // Returns the red component of the color.
    Color Color::operator+(Color const & color) const
    {
        return Color{
            static_cast<uint8_t>(std::clamp(r + color.r, 0, 255)),
            static_cast<uint8_t>(std::clamp(g + color.g, 0, 255)),
            static_cast<uint8_t>(std::clamp(b + color.b, 0, 255)),
            a // Keep the alpha as it is
        };
    }

    Color Color::operator-(Color const & color) const
    {
        return Color{
            static_cast<uint8_t>(std::max(0, r - color.r)),
            static_cast<uint8_t>(std::max(0, g - color.g)),
            static_cast<uint8_t>(std::max(0, b - color.b)),
            a // Keep the alpha as it is
        };
    }

    Color Color::operator+(float value) const
    {
        return Color{
            static_cast<uint8_t>(std::clamp(r + value, 0.0F, 255.0F)),
            static_cast<uint8_t>(std::clamp(g + value, 0.0F, 255.0F)),
            static_cast<uint8_t>(std::clamp(b + value, 0.0F, 255.0F)),
            a};
    }

    Color Color::operator-(float value) const
    {
        return Color{
            static_cast<uint8_t>(std::clamp(static_cast<int>(r - value), 0, 255)),
            static_cast<uint8_t>(std::clamp(static_cast<int>(g - value), 0, 255)),
            static_cast<uint8_t>(std::clamp(static_cast<int>(b - value), 0, 255)),
            a // Keep the alpha as it is
        };
    }

    Color Color::operator*(float value) const
    {
        return Color{
            static_cast<uint8_t>(std::clamp(static_cast<int>(r * value), 0, 255)),
            static_cast<uint8_t>(std::clamp(static_cast<int>(g * value), 0, 255)),
            static_cast<uint8_t>(std::clamp(static_cast<int>(b * value), 0, 255)),
            a // Keep the alpha as it is
        };
    }

    Color& Color::operator+=(Color const & color)
    {
        r = std::clamp(static_cast<int>(r) + static_cast<int>(color.r), 0, 255);
        g = std::clamp(static_cast<int>(g) + static_cast<int>(color.g), 0, 255);
        b = std::clamp(static_cast<int>(b) + static_cast<int>(color.b), 0, 255);
        // Alpha remains unchanged
        return *this; // Return the modified object (current instance)
    }

    Color& Color::operator-=(Color const & color)
    {
        r = std::max(0, static_cast<int>(r) - static_cast<int>(color.r));
        g = std::max(0, static_cast<int>(g) - static_cast<int>(color.g));
        b = std::max(0, static_cast<int>(b) - static_cast<int>(color.b));
        // Alpha remains unchanged
        return *this; // Return the modified object (current instance)
    }

    Color& Color::operator*=(float value)
    {
        r = std::clamp(static_cast<int>(r * value), 0, 255);
        g = std::clamp(static_cast<int>(g * value), 0, 255);
        b = std::clamp(static_cast<int>(b * value), 0, 255);
        // Alpha remains unchanged
        return *this; // Return the modified object (current instance)
    }

    // Lighten the color by a percentage
    Color Color::lighten(float percentage) const
    {
        return Color{
            static_cast<uint8_t>(std::min(255, static_cast<int>(r + (r * percentage)))),
            static_cast<uint8_t>(std::min(255, static_cast<int>(g + (g * percentage)))),
            static_cast<uint8_t>(std::min(255, static_cast<int>(b + (b * percentage)))),
            a};
    }

    // Darken the color by a percentage
    Color Color::darken(float percentage) const
    {
        return Color{
            static_cast<uint8_t>(std::max(0, static_cast<int>(r - (r * percentage)))),
            static_cast<uint8_t>(std::max(0, static_cast<int>(g - (g * percentage)))),
            static_cast<uint8_t>(std::max(0, static_cast<int>(b - (b * percentage)))),
            a};
    }

    // Convert the current color to grayscale.
    Color Color::toGrayScale() const
    {
        auto const gray = static_cast<uint8_t>((r * 0.3) + (g * 0.59) + (b * 0.11));
        return Color{gray, gray, gray, a};
    }

    // Blend the current color with another color.
    Color Color::blendWith(Color const & other) const
    {
        auto blendChannel = [](uint8_t c1, uint8_t c2, uint8_t a1, uint8_t a2) {
            // This is the formula for blending two colors with alpha values:
            // (c1 * a1 * (255 - a2) + c2 * a2 * 255) / (255 * 255)
            // 1. First color contribution: c1 * a1 * (255 - a2)
            //    - This scales color 1 (c1) by its alpha (a1) and the inverse of color 2's alpha (255 - a2) to account
            //    for transparency.
            // 2. Second color contribution: c2 * a2 * 255
            //    - This scales color 2 (c2) by its alpha (a2), treating it as fully opaque (255) for maximum
            //    contribution.
            // 3. Normalization: The sum of both color contributions is divided by 255 * 255
            //    - This ensures the result remains within the 0-255 range, preventing overflow and maintaining valid
            //    color values.

            int const maxAlpha  = 255;                 // Maximum alpha value for normalization
            int const inverseA2 = maxAlpha - a2;       // Inverse of color 2's alpha
            int const scaledC1  = c1 * a1 * inverseA2; // Scaled contribution of color 1
            int const scaledC2  = c2 * a2 * maxAlpha;  // Scaled contribution of color 2

            return static_cast<uint8_t>((scaledC1 + scaledC2) / (maxAlpha * maxAlpha));
        };

        auto const blendedAlpha = static_cast<uint8_t>(a + (other.a * (255 - a) / 255));
        auto const blendedRed   = blendChannel(r, other.r, a, other.a);
        auto const blendedGreen = blendChannel(g, other.g, a, other.a);
        auto const blendedBlue  = blendChannel(b, other.b, a, other.a);

        return Color{blendedRed, blendedGreen, blendedBlue, blendedAlpha};
    }

    bool Color::operator==(Color const & color) const
    {
        return r == color.r && g == color.g && b == color.b && a == color.a;
    }

    bool Color::operator!=(Color const & color) const
    {
        return !(*this == color);
    }

    std::ostream& operator<<(std::ostream& out, Color const & color)
    {
        out << "Color [r = " << static_cast<int>(color.r) << ", g = " << static_cast<int>(color.g)
            << ", b = " << static_cast<int>(color.b) << ", a = " << static_cast<int>(color.a) << "]";
        return out;
    }

    std::string Color::toHexString() const
    {
        std::stringstream ss;
        ss << "#" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(r) << std::setw(2)
           << std::setfill('0') << static_cast<int>(g) << std::setw(2) << std::setfill('0') << static_cast<int>(b);
        return ss.str();
    }

    std::string Color::toRGBString() const
    {
        std::stringstream ss;
        ss << "rgb(" << static_cast<int>(r) << "," << static_cast<int>(g) << "," << static_cast<int>(b) << ")";
        return ss.str();
    }

    std::string Color::toRGBAString() const
    {
        std::stringstream ss;
        ss << "rgba(" << static_cast<int>(r) << "," << static_cast<int>(g) << "," << static_cast<int>(b) << ","
           << static_cast<int>(a) << ")";
        return ss.str();
    }
} // namespace fcn
