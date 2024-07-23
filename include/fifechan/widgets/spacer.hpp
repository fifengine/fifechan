// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#ifndef INCLUDE_FIFECHAN_WIDGETS_SPACER_HPP_
#define INCLUDE_FIFECHAN_WIDGETS_SPACER_HPP_

#include "fifechan/widget.hpp"

namespace fcn
{
    class Graphics;

    class FIFEGUI_API Spacer : public Widget
    {
    public:
        /**
         * Initializes the spacer.
         */
        Spacer();

        ~Spacer();

        // Inherited from Widget

        virtual void resizeToContent(bool recursiv = true);

        /**
         * Overrides this so that the spacer doesn't draw nothing.
         *
         * @param graphics A graphics object to draw with.
         */
        virtual void draw(Graphics* graphics);
    };
}; // namespace fcn

#endif // INCLUDE_FIFECHAN_WIDGETS_SPACER_HPP_
