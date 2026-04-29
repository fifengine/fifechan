// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2026 Fifengine contributors

#ifndef INCLUDE_FIFECHAN_WIDGETS_HORIZONTALBAR_HPP_
#define INCLUDE_FIFECHAN_WIDGETS_HORIZONTALBAR_HPP_

// Platform config include
#include "fifechan/platform.hpp"

// Project headers (subdirs before local)
#include "fifechan/widgets/container.hpp"

namespace fcn
{
    /**
     * A horizontal bar container that arranges children from left to right.
     *
     * This is a base class for StatusBar and MenuBar. It enforces horizontal
     * layout policy and provides methods for controlling spacing and padding.
     *
     * @ingroup widgets
     */
    class FIFEGUI_API HorizontalBar : public Container
    {
    public:
        /**
         * Alignment options for children within the bar.
         *
         * Start: Left-aligned (default)
         * Center: Center-aligned
         * End: Right-aligned
         */
        enum class Alignment
        {
            Start,
            Center,
            End
        };

        /**
         * Constructor.
         *
         * Sets up the container with horizontal layout policy.
         */
        HorizontalBar();

        ~HorizontalBar() override = default;

        HorizontalBar(HorizontalBar const &)            = delete;
        HorizontalBar& operator=(HorizontalBar const &) = delete;
        HorizontalBar(HorizontalBar&&)                  = delete;
        HorizontalBar& operator=(HorizontalBar&&)       = delete;

        /**
         * Sets the spacing between children.
         *
         * @param spacing The spacing in pixels.
         * @see getSpacing
         */
        void setSpacing(unsigned int spacing);

        /**
         * Gets the spacing between children.
         *
         * @return The spacing in pixels.
         * @see setSpacing
         */
        unsigned int getSpacing() const;

        /**
         * Sets the padding around the bar content.
         *
         * @param padding The padding in pixels.
         * @see getPadding
         */
        // cppcheck-suppress duplInheritedMember
        void setPadding(unsigned int padding);

        /**
         * Gets the padding around the bar content.
         *
         * @return The padding in pixels.
         * @see setPadding
         */
        unsigned int getPadding() const;

        /**
         * Sets the default height of the bar.
         *
         * @param height The height in pixels, or 0 for content-driven.
         * @see getFixedHeight
         */
        void setFixedHeight(unsigned int height);

        /**
         * Gets the fixed height of the bar.
         *
         * @return The fixed height in pixels, or 0 if content-driven.
         * @see setFixedHeight
         */
        unsigned int getFixedHeight() const;

        /**
         * Sets the overflow policy.
         *
         * @param clip True to clip overflowing children, false to expand.
         * @see isClipping
         */
        void setClipping(bool clip);

        /**
         * Checks if overflowing children are clipped.
         *
         * @return True if clipping is enabled.
         * @see setClipping
         */
        bool isClipping() const;

        /**
         * Sets whether children should expand to fill available width.
         *
         * When true, children are sized to fill the container width equally.
         * When false (default), children keep their own size.
         *
         * @param expand True to expand children.
         * @see isExpandChildren
         */
        void setExpandChildren(bool expand);

        /**
         * Checks if children expand to fill available width.
         *
         * @return True if children expand.
         * @see setExpandChildren
         */
        bool isExpandChildren() const;

        // Inherited from Container

        void resizeToContent(bool recursion = true) override;
        void adjustSize() override;
        void draw(Graphics* graphics) override;

    protected:
        /**
         * Spacing between children.
         */
        unsigned int mSpacing{4};

        /**
         * Padding around the bar content.
         */
        unsigned int mPadding{4};

        /**
         * Fixed height (0 = content-driven).
         */
        unsigned int mFixedHeight{0};

        /**
         * Whether to clip overflowing content.
         */
        bool mClipping{true};

        /**
         * Whether children should expand to fill width.
         */
        bool mExpandChildren{false};
    };
} // namespace fcn

#endif // INCLUDE_FIFECHAN_WIDGETS_HORIZONTALBAR_HPP_
