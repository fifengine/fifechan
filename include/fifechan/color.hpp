// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#ifndef INCLUDE_FIFECHAN_COLOR_HPP_
#define INCLUDE_FIFECHAN_COLOR_HPP_

#include <cstdint>
#include <iostream>

#include "fifechan/platform.hpp"

namespace fcn
{
    /**
     * Represents a color with red, green, blue and alpha components.
     */
    class FIFEGUI_API Color
    {
    public:
        /**
         * Constructor. Initializes the color to black.
         */
        Color();

        /**
         * Constructor. Constructs a color from the bytes in an integer.
         * Call it with a hexadecimal constant for HTML-style color representation.
         * The alpha component is 255 by default.
         *
         * EXAMPLE: Color(0xff50a0) constructs a very nice pinkish color.
         *
         * NOTE: Because of this constructor, integers will be automatically
         *       casted to a color by your compiler.
         *
         * @param color The color to initialise the object with.
         */
        /*explicit*/ Color(int color); // do not mark explicit to allow implicit conversion from int

        /**
         * Constructor. The default alpha value is 255.
         *
         * @param r Red color component (range 0-255).
         * @param g Green color component  (range 0-255).
         * @param b Blue color component (range 0-255).
         * @param a Alpha, used for transparency. A value of 0 means
         *          fully transparent, 255 is fully opaque.
         */
        Color(int r, int g, int b, int a);
        Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a);

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
         * Compares two colors.
         *
         * @return True if the two colors have the same RGBA components
         *         false otherwise.
         */
        bool operator==(Color const & color) const;

        /**
         * Compares two colors.
         *
         * @return True if the two colors have different RGBA components,
         *         false otherwise.
         */
        bool operator!=(Color const & color) const;

        /**
         * Output operator for output.
         *
         * @param out The stream to output to.
         * @param color The color to output.
         */
        friend std::ostream& operator<<(std::ostream& out, Color const & Color);

        /**
         * Holds the red color component (range 0-255).
         */
        int r;

        /**
         *  Holds the green color component (range 0-255).
         */
        int g;

        /**
         *  Holds the blue color component (range 0-255).
         */
        int b;

        /**
         * Holds the alpha color component. A value of 0 means totally
         * transparent while a value of 255 is considered opaque.
         */
        int a;
    };
} // namespace fcn

#endif // INCLUDE_FIFECHAN_COLOR_HPP_
