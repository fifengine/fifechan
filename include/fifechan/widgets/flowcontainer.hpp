// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#ifndef INCLUDE_FIFECHAN_WIDGETS_FLOWCONTAINER_HPP_
#define INCLUDE_FIFECHAN_WIDGETS_FLOWCONTAINER_HPP_

#include "fifechan/platform.hpp"
#include "fifechan/widgets/container.hpp"

namespace fcn
{
    /**
     * An implementation of a flow container that can contain other widgets.
     * The widgets can be sorted vertical per row or horizontal per column. If the space in the container
     * is too small to put all the components in one row or column, it uses multiple rows or columns.
     */
    class FIFEGUI_API FlowContainer : public Container
    {
    public:
        /**
         * Alignments for widgets. Useful if widgets with different sizes
         * are in the same row or column. For horizontal layout top, center
         * and bottom can be used, for vertical left, center and right.
         */
        enum class Alignment : uint8_t
        {
            Left = 0,
            Right,
            Top,
            Bottom,
            Center
        };

        FlowContainer();

        ~FlowContainer() override;

        FlowContainer(FlowContainer const &)            = delete;
        FlowContainer& operator=(FlowContainer const &) = delete;
        FlowContainer(FlowContainer&&)                  = delete;
        FlowContainer& operator=(FlowContainer&&)       = delete;

        /**
         * Sets the alignment of the widgets.
         *
         * @param alignment The alignment of the widgets.
         * @see getAlignment
         */
        virtual void setAlignment(FlowContainer::Alignment alignment);

        /**
         * Gets the alignment of the widgets.
         *
         * @return The alignment of caption of the widgets.
         * @see setAlignment
         */
        virtual FlowContainer::Alignment getAlignment() const;

        /**
         * Sets the size of the container and sorts the children.
         * If no size is set the parent or max size is used.
         * This is also used by resizeToContent() and expandContent().
         */
        virtual void adjustContent();

        // Inherited from Container

        using Container::expandContent;
        using Container::resizeToContent;

        void setLayout(Container::LayoutPolicy policy) override;

        /**
         * Resize flow layout to fit its children.
         *
         * @param recursion If true, resize children recursively.
         */
        void resizeToContent(bool recursion) override;

        /**
         * Expand children according to flow layout rules.
         *
         * @param recursion If true, expand children recursively.
         */
        void expandContent(bool recursion) override;

    protected:
        /**
         * Current alignment used when laying out child widgets.
         */
        Alignment mAlignment{Alignment::Center};
    };
} // namespace fcn

#endif // INCLUDE_FIFECHAN_WIDGETS_FLOWCONTAINER_HPP_
