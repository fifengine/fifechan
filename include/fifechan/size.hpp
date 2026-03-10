// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#ifndef INCLUDE_FIFECHAN_SIZE_HPP_
#define INCLUDE_FIFECHAN_SIZE_HPP_

#include "fifechan/platform.hpp"

namespace fcn
{
    /**
     * Represents the size of a GUI element, defined by its width and height.
     */
    class FIFEGUI_API Size
    {
    public:
        /**
         * Constructor initializes size values.
         *
         * @param width Width of size.
         * @param height Height of size.
         */
        explicit Size(int width = 0, int height = 0);

        ~Size() = default;

        /**
         * @return Width of size.
         */
        int getWidth() const;

        /**
         * @return Height of size.
         */
        int getHeight() const;

        /**
         * Sets the width of size.
         *
         * @param width Desired width.
         */
        void setWidth(int width);

        /**
         * Sets the height of size.
         *
         * @param height Desired height.
         */
        void setHeight(int height);

    private:
        // width of the size
        int mWidth = 0;
        // height of the size
        int mHeight = 0;
    };
}; // namespace fcn

#endif // INCLUDE_FIFECHAN_SIZE_HPP_
