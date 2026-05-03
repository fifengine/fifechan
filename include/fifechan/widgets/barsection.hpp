// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2026 Fifengine contributors

#ifndef INCLUDE_FIFECHAN_WIDGETS_BARSECTION_HPP_
#define INCLUDE_FIFECHAN_WIDGETS_BARSECTION_HPP_

// Platform config include
#include "fifechan/platform.hpp"

// Project headers (subdirs before local)
#include "fifechan/widgets/container.hpp"

namespace fcn
{
    /**
     * A container section for use within HorizontalBar or VerticalBar widgets.
     *
     * BarSection provides alignment control for its children within a bar layout.
     * It supports Start (left), Center, and End (right) alignment.
     * It can optionally expand to fill available space.
     *
     * In constructor, sets appropriate layout policy (Horizontal by default).
     *
     * @ingroup widgets
     */
    class FIFEGUI_API BarSection : public Container
    {
        public:
            /**
             * Alignment options for children within the section.
             *
             * Start: Left-aligned (or top for vertical),
             * Center: Center-aligned,
             * End: Right-aligned (or bottom for vertical).
             */
            enum class Alignment : uint8_t
            {
                Start = 0,
                Center,
                End
            };

            /**
             * Constructor.
             *
             * Sets up the container with horizontal layout policy.
             */
            BarSection();

            ~BarSection() override;

            BarSection(BarSection const &)            = delete;
            BarSection& operator=(BarSection const &) = delete;
            BarSection(BarSection&&)                  = delete;
            BarSection& operator=(BarSection&&)       = delete;

            /**
             * Sets the alignment of children within the section.
             *
             * @param alignment The alignment to use.
             * @see getAlignment
             */
            void setAlignment(Alignment alignment);

            /**
             * Gets the alignment of children within the section.
             *
             * @return The current alignment.
             * @see setAlignment
             */
            Alignment getAlignment() const;

            /**
             * Sets whether the section expands to fill available space.
             *
             * @param expand True to expand the section.
             * @see isExpand
             */
            void setExpand(bool expand);

            /**
             * Checks if the section expands to fill available space.
             *
             * @return True if expanding.
             * @see setExpand
             */
            bool isExpand() const;

        protected:
            /**
             * Alignment of children within the section.
             */
            Alignment mAlignment{Alignment::Start};

            /**
             * Whether the section expands to fill available space.
             */
            bool mExpand{false};
    };
} // namespace fcn

#endif // INCLUDE_FIFECHAN_WIDGETS_BARSECTION_HPP_
