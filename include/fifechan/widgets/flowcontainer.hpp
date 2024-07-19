// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#ifndef FCN_FLOWCONTAINER_HPP
#define FCN_FLOWCONTAINER_HPP

#include "fifechan/platform.hpp"
#include "fifechan/widgets/container.hpp"

namespace fcn
{
    /**
     * An implementation of a flow container that can contain other widgets.
     * The widgets can be sorted vertical per row or horizontal per column. If the space in the container
     * is too small to put all the components in one row or column, it uses multiple rows or columns.
     */
    class FCN_CORE_DECLSPEC FlowContainer : public Container
    {
    public:
        /**
         * Alignments for widgets. Useful if widgets with different sizes
         * are in the same row or column. For horizontal layout top, center
         * and bottom can be used, for vertical left, center and right.
         */
        enum Alignment
        {
            Left = 0,
            Right,
            Top,
            Bottom,
            Center
        };

        /**
         * Constructor.
         */
        FlowContainer();

        /**
         * Destructor.
         */
        virtual ~FlowContainer();

        /**
         * Sets the alignment of the widgets.
         *
         * @param alignemnt The alignment of the widgets.
         * @see getAlignment
         */
        virtual void setAlignment(FlowContainer::Alignment alignment);

        /**
         * Gets the alignment of the widgets.
         *
         * @return The alignment of caption of the widgets.
         * @see setAlignmentm
         */
        virtual FlowContainer::Alignment getAlignment() const;

        /**
         * Sets the size of the container and sorts the children.
         * If no size is set the parent or max size is used.
         * This is also used by resizeToContent() and expandContent().
         */
        virtual void adjustContent();

        // Inherited from Container

        virtual void setLayout(Container::LayoutPolicy policy);
        virtual void resizeToContent(bool recursiv = true);
        virtual void expandContent(bool recursiv = true);

    protected:
        Alignment mAlignment;
    };
} // namespace fcn

#endif // end FCN_FLOWCONTAINER_HPP
