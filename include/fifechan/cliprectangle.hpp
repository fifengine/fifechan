// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2026 Fifengine contributors

#ifndef INCLUDE_FIFECHAN_CLIPRECTANGLE_HPP_
#define INCLUDE_FIFECHAN_CLIPRECTANGLE_HPP_

#include "fifechan/platform.hpp"
#include "fifechan/rectangle.hpp"

namespace fcn
{
    /**
     * A rectangle specifically used for clipping rendering regions.
     *
     * A clip rectangle is a regular rectangle extended with variables
     * for x offsets and y offsets. The offsets are used for calculations
     * from relative screen coordinates to actual screen coordinates.
     *
     * @ingroup utils
     */
    class FIFEGUI_API ClipRectangle : public Rectangle
    {
    public:
        ClipRectangle() = default;

        /**
         * Constructor.
         *
         * @param x The rectangle x coordinate.
         * @param y The rectangle y coordinate.
         * @param width The rectangle width.
         * @param height The rectangle height.
         * @param xOffset The offset of the x coordinate. Used to for
         *                calculating the actual screen coordinate from
         *                the relative screen coordinate.
         * @param yOffset The offset of the y coordinate. Used to for
         *                calculating the actual screen coordinate from
         *                the relative screen coordinate.
         */
        ClipRectangle(int x, int y, int width, int height, int xOffset, int yOffset);

        /**
         * Copy constructor. Copies x, y, width and height
         * field from a rectangle to a clip rectangle.
         *
         * @param other The rectangle to copy data from.
         * @returns A clip rectangle with data copied from a rectangle.
         */
        ClipRectangle& operator=(Rectangle const & other);

        /**
         * Holds the x offset of the x coordinate.
         */
        int xOffset{0};

        /**
         * Holds the y offset of the y coordinate.
         */
        int yOffset{0};
    };
} // namespace fcn

#endif // INCLUDE_FIFECHAN_CLIPRECTANGLE_HPP_
