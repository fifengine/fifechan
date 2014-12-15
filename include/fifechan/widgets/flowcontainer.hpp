/***************************************************************************
 *   Copyright (C) 2012 by the fifechan team                               *
 *   http://fifechan.github.com/fifechan                                   *
 *   This file is part of fifechan.                                        *
 *                                                                         *
 *   fifechan is free software; you can redistribute it and/or             *
 *   modify it under the terms of the GNU Lesser General Public            *
 *   License as published by the Free Software Foundation; either          *
 *   version 2.1 of the License, or (at your option) any later version.    *
 *                                                                         *
 *   This library is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU     *
 *   Lesser General Public License for more details.                       *
 *                                                                         *
 *   You should have received a copy of the GNU Lesser General Public      *
 *   License along with this library; if not, write to the                 *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA          *
 ***************************************************************************/


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
    class FCN_CORE_DECLSPEC FlowContainer : public Container {
    public:
        /**
         * Alignments for widgets. Useful if widgets with different sizes
         * are in the same row or column. For horizontal layout top, center
         * and bottom can be used, for vertical left, center and right.
         */
        enum Alignment {
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
        virtual void resizeToContent(bool recursiv=true);
        virtual void expandContent(bool recursiv=true);

    protected:
        Alignment mAlignment;
    };
}

#endif // end FCN_FLOWCONTAINER_HPP
