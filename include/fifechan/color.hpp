// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2026 Fifengine contributors

#ifndef INCLUDE_FIFECHAN_COLOR_HPP_
#define INCLUDE_FIFECHAN_COLOR_HPP_

#include <algorithm>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <ostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include "fifechan/platform.hpp"

namespace fcn
{
    /**
     * Color.
     *
     * Represents a color with red, green, blue and alpha components.
     * The color components are in the range 0-255.
     *
     * The alpha component is used for transparency.
     * A value of 0 means fully transparent, 255 is fully opaque.
     * The alpha component is 255 by default.
     *
     * The color can be constructed from a hexadecimal color code or an RGB color code.
     * The hexadecimal color code must be 6 characters long and prefixed with a '#'.
     * The RGB color code must be in the format "rgb(r,g,b)".
     *
     * The color can be lightened or darkened by a percentage.
     * The percentage should be in the range 0-1.
     * A percentage of 0.1 will return a color that is 10% lighter.
     * A percentage of -0.1 will return a color that is 10% darker.
     *
     * The color can be converted to grayscale.
     * The color can be blended with another color using alpha blending.
     *
     * The color can be output as a hexadecimal color code or an RGB color code.
     * The color can be compared with another color.
     * The color can be added, subtracted, multiplied, and divided with another color object.
     * The color can be added, subtracted, multiplied, and divided with a float value.
     * The color can be output to a stream.
     * The color can be converted to a string.
     * The color can be converted to a string in hexadecimal format.
     * The color can be converted to a string in RGB format.
     */
    class FIFEGUI_API Color
    {
    public:
        /**
         * Constructor. Initializes the color to black.
         */
        Color() = default;

        /**
         * Constructor.
         *
         * Constructs a color from the bytes in an integer.
         * The integer should be in the format 0xRRGGBB.
         * The alpha component is 255 by default.         *
         *
         * @param color The color to initialise the object with.
         */
        explicit Color(int hexColor);

        /**
         * Constructor.
         *
         * Constructs a color from a string.
         * The string can be a hexadecimal color code or an RGB color code.
         * The hexadecimal color code must be 6 characters long and prefixed with a '#'.
         * The RGB color code must be in the format "rgb(r,g,b)".
         * The alpha component is 255 by default.
         *
         * @param colorString The color to initialise the object with.
         */
        explicit Color(std::string const & colorString);

        /**
         * Constructor.
         *
         * Constructs a color from the red, green, blue and alpha components.
         * The default alpha value is 255.
         *
         * @param r Red color component (range 0-255).
         * @param g Green color component  (range 0-255).
         * @param b Blue color component (range 0-255).
         * @param a Alpha, used for transparency. A value of 0 means
         *          fully transparent, 255 is fully opaque.
         */
        Color(int r, int g, int b, int a);

        /**
         * Constructor.
         *
         * Constructs a color from the red, green, blue and alpha components.
         * The default alpha value is 255.
         *
         * @param r Red color component (range 0-255).
         * @param g Green color component  (range 0-255).
         * @param b Blue color component (range 0-255).
         * @param a Alpha, used for transparency. A value of 0 means
         *          fully transparent, 255 is fully opaque.
         */
        Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255);

        //
        // Arithmetic operators
        //

        /**
         * Adds the RGB values of two colors together. The values will be
         * clamped if they go out of range.
         *
         * WARNING: This function will reset the alpha value of the
         *          returned color to 255.
         *
         * @param color A color to add to this color.
         * @return The added colors with an alpha value set to 255.
         */
        Color operator+(Color const & color) const;

        /**
         * Subtracts the RGB values of one color from another.
         * The values will be clamped if they go out of range.
         *
         * WARNING: This function will reset the alpha value of the
         *          returned color to 255.
         *
         * @param color A color to subtract from this color.
         * @return The subtracted colors with an alpha value set to 255.
         */
        Color operator-(Color const & color) const;

        /**
         * Multiplies the RGB values of a color with a float value.
         * The values will be clamped if they go out of range.
         *
         * @param value The value to multiply the color with.
         * @return The multiplied colors. The alpha value will, unlike
         *         the add and subtract operations, be multiplied as
         *         well.
         */
        Color operator*(float value) const;

        /**
         * Adds a float value to the RGB values of a color.
         * The values will be clamped if they go out of range.
         * The alpha value will be kept as it is.
         *
         * @param value The value to add to the color.
         * @return The added colors with an alpha value set to 255.
         */
        Color operator+(float value) const;

        /**
         * Subtracts a float value from the RGB values of a color.
         * The values will be clamped if they go out of range.
         * The alpha value will be kept as it is.
         *
         * @param value The value to subtract from the color.
         * @return The subtracted colors with an alpha value set to 255.
         */
        Color operator-(float value) const;

        //
        // Object manipulation operators
        //

        /**
         * Adds the RGB values of another color to this color.
         * The values will be clamped if they go out of range.
         * The alpha value will be kept as it is.
         *
         * @param color The color to add to this color.
         * @return The modified object (the current instance).
         */
        Color& operator+=(Color const & color);

        /**
         * Subtracts the RGB values of another color from this color.
         * The values will be clamped if they go out of range.
         * The alpha value will be kept as it is.
         *
         * @param color The color to subtract from this color.
         * @return The modified object (the current instance).
         */
        Color& operator-=(Color const & color);

        /**
         * Multiplies the RGB values of this color with a float value.
         * The values will be clamped if they go out of range.
         * The alpha value will be kept as it is.
         *
         * @param value The value to multiply the color with.
         * @return The modified object (the current instance).
         */
        Color& operator*=(float value);

        //
        // Comparison operators
        //

        /**
         * Compares two colors.
         *
         * @param color The color to compare with.
         * @return True, if both colors have the same RGBA components, false otherwise.
         */
        bool operator==(Color const & color) const;

        /**
         * Compares two colors.
         *
         * @param color The color to compare with.
         * @return True, if both colors have the same RGBA components, false otherwise.
         */
        bool operator!=(Color const & color) const;

        //
        // Color conversions
        //

        /**
         * Lightens the color by a percentage.
         * The alpha value will be kept as it is.
         * The color will be clamped if it goes out of range.
         * The percentage should be in the range 0-1.
         * A percentage of 0 will return the same color.
         * A percentage of 1 will return white.
         * A percentage of -1 will return black.
         * A percentage of 0.1 will return a color that is 10% lighter.
         * A percentage of -0.1 will return a color that is 10% darker.
         *
         * @param percentage The percentage to lighten the color by.
         * @return A new lightened color object.
         */
        Color lighten(float percentage) const;

        /**
         * Darkens the color by a percentage.
         * The alpha value will be kept as it is.
         * The color will be clamped if it goes out of range.
         * The percentage should be in the range 0-1.
         * A percentage of 0 will return the same color.
         * A percentage of 1 will return black.
         * A percentage of -1 will return white.
         * A percentage of 0.1 will return a color that is 10% darker.
         * A percentage of -0.1 will return a color that is 10% lighter.
         *
         * @param percentage The percentage to darken the color by.
         * @return A new darkened color object.
         */
        Color darken(float percentage) const;

        /**
         * Converts the color to grayscale.
         * The alpha value will be kept as it is.
         *
         * @return The grayscale color.
         */
        Color toGrayScale() const;

        /**
         * Blends the color with another color using alpha blending.
         * The alpha value changes.
         * The resulting color's alpha will be a combination of both colors' alpha values.
         *
         * @param other The color to blend with.
         * @return The blended color.
         */
        Color blendWith(Color const & other) const;

        //
        // to string and stream functions
        //

        std::string toHexString() const;
        std::string toRGBString() const;
        std::string toRGBAString() const;

        /**
         * Output operator for output.
         *
         * @param out The stream to output to.
         * @param color The color to output.
         */
        friend std::ostream& operator<<(std::ostream& out, Color const & color);

        /**
         * Red, green, blue, and alpha color components (range 0-255).
         * Alpha: 0 means fully transparent, 255 means fully opaque.
         */
        uint8_t r{0}, g{0}, b{0}, a{255};

    private:
        /**
         * Parses a hexadecimal color string.
         * The string must be 6 characters long and prefixed with a '#'.
         * The string must be in the format "#RRGGBB".
         * The alpha component is 255 by default.
         * The function will throw an exception if the string is invalid.
         *
         * @param hex The hexadecimal color string to parse.
         * @throws std::invalid_argument If the string is invalid.
         */
        void parseHex(std::string const & hex);

        /**
         * Parses an RGB color string.
         * The string must be in the format "rgb(r,g,b)".
         * The alpha component is 255 by default.
         * The function will throw an exception if the string is invalid.
         *
         * @param rgbString The RGB color string to parse.
         * @throws std::invalid_argument If the string is invalid.
         */
        void parseRGB(std::string const & rgbString);

        /**
         * Parses an RGBA color string.
         * The string must be in the format "rgba(r,g,b,a)".
         * The function will throw an exception if the string is invalid.
         *
         * @param rgbaString The RGBA color string to parse.
         * @throws std::invalid_argument If the string is invalid.
         */
        void parseRGBA(std::string const & rgbaString);

        /**
         * Parses a string of color components.
         * The string must be in the format "r,g,b" or "r,g,b,a".
         * The function will throw an exception if the string is invalid.
         * Static helper function used in the parseHex and parseRGB functions.
         *
         * @param colors The colors as a string to parse.
         * @param withAlpha True, if the string contains an alpha component.
         * @throws std::invalid_argument If the string is invalid.
         * @return The parsed color components.
         */
        static std::vector<int> parseColorComponents(std::string const & colors, bool withAlpha = false);
    };
} // namespace fcn

#endif // INCLUDE_FIFECHAN_COLOR_HPP_
